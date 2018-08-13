/**
 * ********* Boston University Rocket Propulsion Group **********
 */
/* 
 * This software was designed and written by members of the Boston Univ.
 * Rocket Propulsion group for the sole purpose of providing ground control
 * to BURPG rocket motors. Do not use or distribute this or any other part of this
 * software for any reason other than its intended application and without express
 * permission from the BU Rocket Propulsion Group. 
 * 
 * Boston University Rocket Propulsion Group
 * Ground Control Software Version 3.0
 * Designed by Drew Kelley
 * 
 */
package olympus.modules.daq;

import java.io.File;
import java.io.FileNotFoundException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortList;
import olympus.OlympusMain;
import olympus.modules.ModuleManager;

/**
 * @author Drew Kelley Date Authored: Aug 1, 2014 Class: DAQManager
 *
 *
 */
public class DAQManager extends Thread implements ModuleManager {

    public static final int MODULE_ID = 3;
    private static final int CONFIG_VALID = 0;
    private static final int NO_PACKET_START_BYTE = -1;
    private static final int NO_PACKET_END_BYTE = -2;
    private static final int INVALID_START_BYTE = -3;
    private static final int INVALID_END_BYTE = -4;

    private static final int NO_ACK_BYTE = -6;
    private static final int INVALID_ACK_BYTE = -7;
    private static final int NO_TELEMETRY_PACKET_TYPE_ID = -8;
    private static final int INVALID_TELEMETRY_PACKET_TYPE_ID = -9;
    private static final int NO_SERIAL_PACKET_TYPE_ID = -10;
    private static final int INVALID_SERIAL_PACKET_TYPE_ID = -11;
    private static final int NO_HOUSEKEEPING_PACKET_TYPE_ID = -12;
    private static final int INVALID_HOUSEKEEPING_PACKET_TYPE_ID = -13;
    private static final int NO_DATA_TIMER = -14;
    private static final int INVALID_DATA_TIMER = -15;

    private final OlympusMain system;
    private final String cfgfilename;
    private final HashMap<String, BoardManager> boardlist;
    private final HashMap<String, String> daqconfigs;
    private final ArrayList<BoardChannel> serialconnections;
    private final ArrayList<String> connectionRequests;
    private final HashMap<String, TelemeteredData> telems;
    private final HashMap<Byte, TelemeteredData> telemIDs;
    private volatile boolean started = false;

    private boolean isLogging;
    private boolean healthRunning;
    private boolean isStreaming;

    public DAQManager(OlympusMain sys, String cfg) {
        system = sys;
        cfgfilename = System.getenv("IRISCONFIGDIR") + "/Olympus/" + cfg;
        boardlist = new HashMap<>();
        daqconfigs = new HashMap<>();
        serialconnections = new ArrayList<>();
        connectionRequests = new ArrayList<>();
        telems = new HashMap<>();
        telemIDs = new HashMap<>();
    }

    public void closeOutChannel(BoardChannel bc) {
        BoardManager board = null;
        for (String name : boardlist.keySet()) {
            if (boardlist.get(name).getChannel() == bc) {
                board = boardlist.get(name);
                break;
            }
        }
        bc.closeChannel();
        serialconnections.remove(bc);
        if (board != null) {
            board.setChannel(null);
            system.network().commandServer().sendSysInfo("BOARD DISCONNECTED " + board.getName());
        }
    }

    @Override
    public int configIsValid() {
        if (daqconfigs.get("packet_start") == null) {
            return NO_PACKET_START_BYTE;
        } else if (daqconfigs.get("packet_end") == null) {
            return NO_PACKET_END_BYTE;
        } else if (daqconfigs.get("ack_start") == null) {
            return NO_ACK_BYTE;
        } else if (daqconfigs.get("telem_packet_id") == null) {
            return NO_TELEMETRY_PACKET_TYPE_ID;
        } else if (daqconfigs.get("serial_packet_id") == null) {
            return NO_SERIAL_PACKET_TYPE_ID;
        } else if (daqconfigs.get("housekeeping_packet_id") == null) {
            return NO_HOUSEKEEPING_PACKET_TYPE_ID;
        } else if (daqconfigs.get("data_timer") == null) {
            return NO_DATA_TIMER;
        }
        try {
            Byte.parseByte(daqconfigs.get("telem_packet_id"));
        } catch (NumberFormatException ex) {
            return INVALID_TELEMETRY_PACKET_TYPE_ID;
        }
        try {
            Byte.parseByte(daqconfigs.get("serial_packet_id"));
        } catch (NumberFormatException ex) {
            return INVALID_SERIAL_PACKET_TYPE_ID;
        }
        try {
            Byte.parseByte(daqconfigs.get("housekeeping_packet_id"));
        } catch (NumberFormatException ex) {
            return INVALID_HOUSEKEEPING_PACKET_TYPE_ID;
        }
        try {
            Byte.parseByte(daqconfigs.get("ack_start"));
        } catch (NumberFormatException ex) {
            return INVALID_ACK_BYTE;
        }
        try {
            Byte.parseByte(daqconfigs.get("packet_start"));
        } catch (NumberFormatException ex) {
            return INVALID_START_BYTE;
        }
        try {
            Byte.parseByte(daqconfigs.get("packet_end"));
        } catch (NumberFormatException ex) {
            return INVALID_END_BYTE;
        }
        try {
            Integer.parseInt(daqconfigs.get("data_timer"));
        } catch (NumberFormatException ex) {
            return INVALID_DATA_TIMER;
        }
        return CONFIG_VALID;
    }

    public void connectBoard(String name) {
        BoardManager bc = getBoardManager(name);
        int conn_result = BoardConnectionProcess.connectBoard(system, this, bc);
        switch (conn_result) {
            case BoardConnectionProcess.NOBAUD_ERR:
                system.network().commandServer().sendMessage(name + " has no BAUD rate specified.");
                break;
            case BoardConnectionProcess.NOCFG_ERR:
                system.network().commandServer().sendMessage(name + " does not exist in loaded configuration files.");
                break;
            case BoardConnectionProcess.NOSERIAL_ERR:
                system.network().commandServer().sendMessage(name + " has no SERIAL specified.");
                boardlist.get(name).setChannel(null);
                break;
            default:
                system.logstream("daq", "Connection request for " + name + " saved.");
                system.network().commandServer().sendMessage("Connection request for " + name + " has been sent. Awaiting board reply.");
                connectionRequests.add(name);
        }
    }

    public String get(String key) {
        return daqconfigs.get(key);
    }

    public BoardManager getBoardManager(String name) {
        return boardlist.get(name);
    }

    @Override
    public int getID() {
        return MODULE_ID;
    }

    public ArrayList<BoardChannel> getSerialConnections() {
        return serialconnections;
    }

    @Override
    public int loadConfigs() {
        File cfg_file = new File(cfgfilename);
        if (cfgfilename == null) {
            system.log("No daqcfg file specified. Cannot start DAQ Manager.");
            return NO_CFG_FILE_SPECIFIED;
        }
        if (!cfg_file.exists()) {
            system.log("Specified daqcfg file does not exist :" + cfgfilename);
            return CFG_FILE_DOES_NOT_EXIST;
        }
        try {
            Scanner sc = new Scanner(cfg_file);
            while (sc.hasNextLine()) {
                String line = sc.nextLine();
                String[] splits = line.split("\\s+");
                if (splits[0].equals("board")) {
                    String boardcfgfile = System.getenv("IRISCONFIGDIR") + "/Olympus/" + splits[1];
                    BoardManager bc = new BoardManager(system, boardcfgfile, this);
                    int boardinit = bc.initialize();
                    if (boardinit == BoardManager.INIT_SUCCESS) {
                        system.logstream("daq", "Loaded " + bc.getName() + " configuration file.");
                        boardlist.put(bc.getName(), bc);
                    } else {
                        system.logstream("daq", "Failed to load " + bc.getName() + " configuration file. Board init error: " + boardinit);
                    }
                } else {
                    daqconfigs.put(splits[0], splits[1]);
                }
            }
        } catch (FileNotFoundException ex) {
            system.log("Error loading DAQ configs.", ex);
            return MISC_ERR;
        }
        int validation = configIsValid();
        if (validation < 0) {
            system.log("Invalid DAQ configuration. DAQ error: " + validation);
            return CONFIG_VALIDATION_FAILED;
        }
        return CONFIG_LOAD_SUCCESS;
    }

    private boolean portInUse(String port) {
        for (String str : boardlist.keySet()) {
            BoardChannel bc = boardlist.get(str).getChannel();
            if (bc != null && bc.getPortName().equals(port)) {
                return true;
            }
        }
        return false;
    }

    public final void refreshPortList() {
        //Will want to make sure we don't close an active board. Should improve this soon-ish.
        for (int i = 0; i < serialconnections.size(); i++) {
            BoardChannel bc = serialconnections.get(i);
            if (bc.getManager() == null) {
                closeOutChannel(bc);
                i--;
            }
        }
        String[] portlist = SerialPortList.getPortNames();
        for (String str : portlist) {
            if (!portInUse(str)) {
                try {
                    SerialPort sp = new SerialPort(str);
                    sp.openPort();
                    sp.setParams(9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
                    BoardChannel temp = new BoardChannel(system, this, sp);
                    serialconnections.add(temp);
                } catch (SerialPortException ex) {
                    system.logstream("daq", "Port error.", ex);
                }
            }
        }
        system.logstream("daq", "Port list refreshed: " + serialconnections);
    }

    public void reloadConfigs() {

    }

    void registerBoard(String serial, BoardChannel bchan) {
        String boardname = null;
        for (String s : connectionRequests) {
            BoardManager bman = getBoardManager(s);
            if (bman.getConfig("serial").equals(serial)) {
                boardname = s;
                bman.setChannel(bchan);
                bchan.setBoardManager(bman);
                system.network().commandServer().sendMessage(s + " connected successfully!");
                system.network().commandServer().sendSysInfo("BOARD CONNECTED " + s);
                system.logstream("daq", s + " registered.");
                break;
            }
        }
        if (boardname != null) {
            connectionRequests.remove(boardname);
        }
    }

    @Override
    public void run() {
        system.log("Starting DAQ Manager...");
        int load_result = loadConfigs();
        if (load_result == CONFIG_LOAD_SUCCESS) {
            refreshPortList();
            started = true;
            system.log("DAQ Manager Started...");
            system.setLight(true, "daq");
        } else {
            system.log("Error starting DAQ Module. Module code: " + load_result);
        }
        new EthernetReceiver(system, this, boardlist.get("LAC")).start();
    }

    @Override
    public void shutdown() {
        stopStreaming();
        BoardManager board = null;
        for (int i = 0; i < serialconnections.size(); i++) {
            BoardChannel bc = serialconnections.get(0);
            for (String name : boardlist.keySet()) {
                if (boardlist.get(name).getChannel() == bc) {
                    board = boardlist.get(name);
                    break;
                }
            }
            bc.closeChannel();
            serialconnections.remove(bc);
            i--;
            if (board != null) {
                board.startHealthTimer();
                writeCmd(board.getName(), "stop_stream");
                board.setChannel(null);
            }
        }

        stopLogging();
        system.setLight(false, "dtws");
        system.setLight(false, "saving");
        system.setLight(false, "healthpackets");
        system.setLight(false, "streaming");
    }

    public void writeCmd(String boardname, String commandname) {
        BoardManager man = boardlist.get(boardname);
        if (man == null) {
            system.logstream("daq", "Bad request for board (" + boardname + "). Board does not exist.");
            return;
        }
        if ((man.getCommand(commandname) == null) && (!"LAC".equals(boardname))) {
            system.logstream("daq", "Bad command name for (" + boardname + "). (" + commandname + ") does not exist.");
            system.network().commandServer().sendMessage("Bad command for (" + boardname + "). (" + commandname + ") does not exist");
            return;
        }
        if (commandname.contains("POS")) {
          EthernetDataPacket pack = new EthernetDataPacket(this, commandname);
            sendEthernetDataPacket(man, pack);  
        } else if ("ethernet".equals(man.getConfig("type"))) {
            EthernetDataPacket pack = new EthernetDataPacket(this, man.getCommand(commandname));
            sendEthernetDataPacket(man, pack);
        } else if ("serial".equals(man.getConfig("type"))) {
            DataPacket pack = new DataPacket(this, man.getCommand(commandname));
            sendDataPacket(man, pack);
        }
    }

    public void writeCmd(String boardname, String commandname, boolean noLog) {
        BoardManager man = boardlist.get(boardname);
        if (man == null) {
            system.logstream("daq", "Bad request for board (" + boardname + "). Board does not exist.");
            return;
        }
        if ((man.getCommand(commandname) == null) && (!"LAC".equals(boardname))) {
            system.logstream("daq", "Bad command name for (" + boardname + "). (" + commandname + ") does not exist.");
            system.network().commandServer().sendMessage("Bad command for (" + boardname + "). (" + commandname + ") does not exist");
            return;
        }
        if ("ethernet".equals(man.getConfig("type"))) {
            EthernetDataPacket pack = new EthernetDataPacket(this, man.getCommand(commandname));
            sendEthernetDataPacket(man, pack);
        } else if ("serial".equals(man.getConfig("type"))) {
            DataPacket pack = new DataPacket(this, man.getCommand(commandname));
            sendDataPacket(man, pack);
        }
    }

    private void sendDataPacket(BoardManager board, DataPacket packet) {
        new TransmitWorker(system, this, board, packet).start();
    }
    
    private void sendEthernetDataPacket(BoardManager board, EthernetDataPacket packet) {
        new EthernetTransmitWorker(board, packet).start();
    }

    private void sendDataPacket(BoardManager board, DataPacket packet, boolean noLog) {
        new TransmitWorker(system, this, board, packet, noLog).start();
    }

    void writePacket(BoardChannel bchan, DataPacket temp) {
        byte[] data = temp.getData();
        bchan.writeBytes(data);
    }

    public void addTelem(TelemeteredData temp) {
        telems.put(temp.getName(), temp);
        telemIDs.put(temp.getPacketID(), temp);
        system.network().addDTW(temp);

    }

    public TelemeteredData getTelem(String name) {
        return telems.get(name);
    }

    public TelemeteredData getTelem(byte id) {
        for (String s : telems.keySet()) {
            if (telems.get(s).getPacketID() == id) {
                return telems.get(s);
            }
        }
        return null;
    }

    public ArrayList<BoardManager> getLoadedBoards() {
        ArrayList<BoardManager> temp = new ArrayList<>();
        for (String s : boardlist.keySet()) {
            temp.add(boardlist.get(s));
        }
        return temp;
    }

    @Override
    public boolean isStarted() {
        return started;
    }

    public void startLogging() {
        if (isLogging) {
            return;
        }
        isLogging = true;
        system.setLight(true, "saving");
        system.network().commandServer().sendSysInfo("LOGGING STATE ACTIVE");
    }

    public void stopLogging() {
        if (!isLogging) {
            return;
        }
        isLogging = false;
        system.setLight(false, "saving");
        system.network().commandServer().sendSysInfo("LOGGING STATE INACTIVE");
    }

    public void startHealth() {
        healthRunning = true;
        for (String s : boardlist.keySet()) {
            boardlist.get(s).startHealthTimer();
        }
        system.setLight(true, "healthpackets");
        system.network().commandServer().sendSysInfo("HEALTH STATE ACTIVE");
    }

    public void stopHealth() {
        if (!healthRunning) {
            return;
        }
        healthRunning = false;
        for (String s : boardlist.keySet()) {
            boardlist.get(s).stopHealthTimer();
        }
        system.setLight(false, "healthpackets");
        system.network().commandServer().sendSysInfo("HEALTH STATE INACTIVE");
    }

    boolean isLogging() {
        return isLogging;
    }

    public void startStreaming() {
        if (isStreaming) {
            return;
        }
        isStreaming = true;
        for (String str : boardlist.keySet()) {
            writeCmd(str, "start_stream");
        }
        system.setLight(true, "streaming");
        system.network().commandServer().sendSysInfo("STREAMING STATE ACTIVE");
    }

    public void stopStreaming() {
        if (!isStreaming) {
            return;
        }
        isStreaming = false;
        for (String str : boardlist.keySet()) {
            writeCmd(str, "stop_stream");
        }
        system.setLight(false, "streaming");
        system.network().commandServer().sendSysInfo("STREAMING STATE INACTIVE");
    }

    public String dataSavingActive() {
        return isLogging ? "ACTIVE" : "INACTIVE";
    }

    public String streamingActive() {
        return isStreaming ? "ACTIVE" : "INACTIVE";
    }

    public String healthPacketsActive() {
        return healthRunning ? "ACTIVE" : "INACTIVE";
    }

    public void sendBoardStatus() {
        for (BoardManager bman : system.getDAQ().getLoadedBoards()) {
            if (bman.getChannel() != null && bman.getChannel().isConnected()) {
                system.network().commandServer().sendSysInfo("BOARD CONNECTED " + bman.getName());
            } else {
                system.network().commandServer().sendSysInfo("BOARD DISCONNECTED " + bman.getName());
            }
        }
    }

    public void sendSerialStatus() {
        String message = "";
        for (int i = 0; i < serialconnections.size(); i++) {
            message += serialconnections.get(i).toString();
            if (i < serialconnections.size() - 1) {
                message += "^";
            }
        }
        system.network().commandServer().sendSysInfo("SERIAL " + message);
    }

    public void makeSafe() {
        for (BoardManager man : getLoadedBoards()) {
            writeCmd(man.getName(), "SAFE");
        }
    }

    public void saveLogs(String savedir) {
        boolean logging = false;
        if (isLogging) {
            logging = true;
        }
        if (logging) {
            stopLogging();
        }
        File logdir = new File("Data Logs/");
        File newdir = new File("Data Logs/" + savedir);
        newdir.mkdir();
        system.logstream("daq", "Saving old log files.");
        for (File f : logdir.listFiles()) {
            if (f.isFile()) {
                f.renameTo(new File(newdir.getAbsolutePath() + "/" + f.getName()));
            }
        }
        system.logstream("daq", "Old logs saved.");
        if (logging) {
            startLogging();
        }

    }

    public OlympusMain getSystem() {
        return system;
    }

}
