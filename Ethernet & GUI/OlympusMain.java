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
package olympus;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;
import java.util.Scanner;
import java.util.TreeMap;
import olympus.controllers.ControllerManager;
import olympus.ethernet.EthernetManager;
import olympus.gui.MainGUI;
import olympus.modules.daq.DAQManager;
import olympus.modules.logging.LoggingManager;
import olympus.modules.networking.NetworkManager;

/**
 * @author Drew Kelley Date Authored: Class: OlympusMain
 *
 * OlympusMain is the main class of the software. It is run when the program
 * starts. OlympusMain is responsible for loading overarching system configs,
 * starting the various modules, launching the GUI and logging system-level
 * information.
 *
 * Additionally it serves as a bridge between various parts of the software,
 * allowing the easy flow of data and commands from, for instance, the
 * Communication Manager to the GUI.
 *
 */
public class OlympusMain {

    private final Map<String, String> configuration;
    File configuration_file;
    private final MainGUI GUI;
    private NetworkManager nm;
    private DAQManager dm;
    private LoggingManager lm;
    private ControllerManager cm;
    private EthernetManager em;

    /**
     * @param args the command line arguments Main - run when the program
     * starts. Creates a new OlympusMain object.
     */
    public static void main(String[] args) {
        OlympusMain olympusMain = new OlympusMain();
    }

    /**
     * OlympusMain constructor. Creates the GUI and begins the process of
     * loading config data and starting the. modules.
     */
    public OlympusMain() {
        //Store a reference to the configuration file.

        configuration_file = new File(System.getenv("IRISCONFIGDIR")+"/Olympus/system.cfg");
        //The global configuration settings.
        configuration = new TreeMap<>();

        //Launch the GUI.
        GUI = new MainGUI(this);
        GUI.setVisible(true);
        //Load configs
        loadConfigs();
        //Start system level error logging.
        startLogs();
        log("Boston University Rocket Propulsion Group");
        log("IRIS Control Software - OLYMPUS module");
        log("For more information, check the Rocket Docs");
        //Starts the modules.
        loadModules();
    }

    /**
     * DateTime - returns a string that is the current date and time.
     *
     * @return - String
     */
    public static String DateTime() {
        SimpleDateFormat sdf = new SimpleDateFormat("MM/dd - HH:mm:ss.SSS");
        return sdf.format(new Date());
    }

    public void findBoard(String boardname) {
        dm.connectBoard(boardname);
    }

    /**
     * getConfig - gets the configuration value for a given key.
     *
     * @param key
     * @return - configuration.get(key). Null if no value in config.
     */
    public String getConfig(String key) {
        if (configuration.get(key) == null) {
            log("Invalid configuration request: " + key);
            return null;
        }
        return configuration.get(key);
    }

    /**
     * GUISetIndicator - triggers a status light on the GUI.
     *
     * @param active - On or off?
     * @param ID - The light ID.
     */
    public void setLight(boolean active, String name) {
        GUI.SetIndicator(active, name);
    }

    /**
     * LoadModules Creates an object of each of the modules, adds them to the
     * global modules list and starts them.
     */
    private void loadModules() {
        nm = new NetworkManager(this, getConfig("networkcfg"));
        lm = new LoggingManager(this, getConfig("logcfg"));
        dm = new DAQManager(this, getConfig("daqcfg"));
        em = new EthernetManager(this, getConfig("ethernetcfg"));
        if (getConfig("controllerdir") != null){
            cm = new ControllerManager(this, getConfig("controllerdir"));
        }
        lm.start();
        while (!lm.isStarted()) {
        }
        nm.start();
        while (!nm.isStarted()) {
        }
        dm.start();
        while (!dm.isStarted()) {
        }
        em.start();
        while (!em.isStarted()) {
        }
        cm.start();
        while (!cm.isStarted()) {
        }
    }

    /**
     * LoadConfig Opens the configuration file and beings reading in
     * configuration data. LoadConfig also verifies many of the options against
     * acceptable parameters and defaults several things if they aren't
     * included.
     *
     */
    private void loadConfigs() {
        System.out.println(OlympusMain.DateTime() + ": Loading cofiguration file...");
        if (!configuration_file.exists()) {
            System.out.println("Failed to load configuration file: " + configuration_file);
            System.out.println("System will now exit...");
            System.exit(0);
        }
        try {
            try (Scanner config_scan = new Scanner(configuration_file)) {
                while (config_scan.hasNextLine()) {
                    String line = config_scan.nextLine();
                    String[] split = line.split("\\s+");
                    if(split.length<2){
                        continue;
                    }
                    configuration.put(split[0], split[1]);
                }
                if (configuration.get("logdir") == null) {
                    System.out.println("No log directory specified in configuration, defaulting to ./logs/");
                    configuration.put("logdir", "logs");
                }
                if (configuration.get("stderr") == null) {
                    System.out.println("No standard error stream specified, defaulting to [logdir]/err.log");
                    configuration.put("stderr", "err");
                }
                if (configuration.get("cmd_port") == null) {
                    System.out.println("No system port specified, defaulting to 4041");
                    configuration.put("cmd_port", "4041");
                }
                if (configuration.get("telem_port") == null) {
                    System.out.println("No telemetry port specified, defaulting to 4042");
                    configuration.put("telem_port", "4042");
                }
                if (configuration.get("daqcfg") == null) {
                    System.out.println("No DAQ configuration file specified. System will now exit...");
                    System.exit(0);
                }
                if (configuration.get("logcfg") == null) {
                    System.out.println("No Logging configuration file specified. System will now exit...");
                    System.exit(0);
                }
                if (configuration.get("controllerdir") == null){
                    System.out.println("No controller directory specified. ControllerManager will not be started...");
                }
                if (configuration.get("ethernetcfg") == null){
                    System.out.println("No ethernet configuration file specified. System will now exit...");
                    System.exit(0);
                }
            }
            System.out.println("DONE");
        } catch (FileNotFoundException e) {
            System.out.println("Error while loading the configuration data.\nSystem will now exit...");
            System.exit(0);
        }
    }

    /**
     * log - logs the specified text
     *
     * @param text - the message to log.
     */
    public final void log(String text) {
        if (text.length() > 0 && text.charAt(text.length() - 1) == '\n') {
            System.out.print(DateTime() + ": " + text);
            System.err.print(DateTime() + ": " + text);
        } else {
            System.out.println(DateTime() + ": " + text);
            System.err.println(DateTime() + ": " + text);
        }
    }

    /**
     * log - logs the specified text AND the exception that was thrown
     * somewhere.
     *
     * @param text - the message saying what was happening.
     * @param e - the Exception that was thrown.
     */
    public void log(String text, Exception e) {
        System.out.println(DateTime() + ": " + text);
        System.err.println(DateTime() + ": " + text);
        System.out.println(e);
        for (StackTraceElement ste : e.getStackTrace()) {
            System.out.println(ste);
            System.err.println(ste);
        }
    }

    /**
     * Passes a log message to a particular log stream as well as the system
     * log.
     *
     * @param stream - The log stream to use.
     * @param msg - The message to log.
     */
    public void logstream(String stream, String msg) {
        synchronized (this) {
            log(stream + ": " + msg);
            msg = OlympusMain.DateTime() + "---" + msg;
            lm.addtostream(stream, msg);
        }
    }

    /**
     * Passes a log message to a particular lost stream as well as the system
     * log. Includes an exception.
     *
     * @param stream - The log stream to use.
     * @param msg - the message to log.
     * @param e - The exception.
     */
    public void logstream(String stream, String msg, Exception e) {
        log(stream + ": " + msg, e);
        logstream(stream, msg);
    }

    public void refreshBoardConnections() {
        dm.refreshPortList();
    }

    public void reloadCfgs() {
        dm.reloadConfigs();
    }

    /**
     * Shutdown cleanly closes all modules and shutdowns down the GUI.
     */
    public void shutdown() {
        log("Shutting down...");
        if (nm != null) {
            nm.shutdown();
        }
        if (dm != null) {
            dm.shutdown();
        }
        if (lm != null) {
            lm.shutdown();
        }
        if (cm != null) {
            cm.shutdown();
        }
        if (em != null) {
            em.shutdown();
        }
        log("All modules shutdown");
        GUI.setVisible(false);
        GUI.dispose();
        System.exit(0);
    }

    /**
     * Starts the system level logging in the specified log files.
     */
    private void startLogs() {
        File logdirectory = new File(configuration.get("logdir"));
        File stderr = new File(configuration.get("logdir") + "/" + configuration.get("stderr") + ".log");
        if (!logdirectory.exists()) {
            if (!logdirectory.mkdir()) {
                System.out.println("Failed to create log directory. System will now exit");
                System.exit(0);
            }
        }
        int index = 0;
        String base = configuration.get("stderr");
        while (stderr.exists()) {
            configuration.put("stderr", base + index);
            index++;
            stderr = new File(configuration.get("logdir") + "/" + configuration.get("stderr") + ".log");
        }
        try {
            System.setErr(new PrintStream(stderr));
            System.out.println("Logs created.");
        } catch (FileNotFoundException e) {
            System.out.println("Error creating the logs. System will now exit...");
            System.exit(0);
        }
    }

    public NetworkManager network() {
        return nm;
    }

    public DAQManager getDAQ() {
        return dm;
    }

    public ControllerManager getControllerManager() {
        return cm;
    }
    
    public EthernetManager getEthernet() {
        return em;
    }
    
}
