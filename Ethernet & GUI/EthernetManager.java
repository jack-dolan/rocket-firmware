package olympus.ethernet;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Properties;
import java.util.Scanner;
import olympus.OlympusMain;
import olympus.modules.ModuleManager;
import static olympus.modules.ModuleManager.CFG_FILE_DOES_NOT_EXIST;
import static olympus.modules.ModuleManager.CONFIG_LOAD_SUCCESS;
import static olympus.modules.ModuleManager.CONFIG_VALIDATION_FAILED;
import static olympus.modules.ModuleManager.MISC_ERR;
import static olympus.modules.ModuleManager.NO_CFG_FILE_SPECIFIED;

/**
 *
 * @author abagheri
 *
 * EthernetManager manages the Ethernet module, which will send telemetry data
 * from Olympus to Gaia, and will receive commands from Gaia.
 * 
 */

public class EthernetManager extends Thread implements ModuleManager {
    
    private HashMap<String, Ethernet> boards;
    private HashMap<String, Properties> ethernetProps;
//    private final HashMap<String, BoardManager> boardlist;
   // private final HashMap<String, String> ethconfigs;
    
    private final OlympusMain system;
    private final String cfgfilename;
    private volatile boolean started = false;
    
    public EthernetManager(OlympusMain sys, String cfg) {
        system = sys;
        cfgfilename = System.getenv("IRISCONFIGDIR") + "/Olympus/" + cfg;
        //boardlist = new HashMap<>();
    }
    
    @Override
    public void run() {
        system.log("Starting Ethernet Manager...");
        int load_result = loadConfigs();
        if (load_result == CONFIG_LOAD_SUCCESS) {
            started = true;
            system.log("Ethernet Manager Started.");
            system.setLight(true, "ethernet");
        } else {
            system.log("Error starting Ethernet Module. Module code: " + load_result);
        }
    }
    
    @Override
    public int loadConfigs() {
        File cfg_file = new File(cfgfilename);
        if (cfgfilename == null) {
            system.log("No ethernetcfg directory specified. Cannot start Ethernet Manager.");
            return NO_CFG_FILE_SPECIFIED;
        }
        if (!cfg_file.exists()) {
            system.log("Specified ethernetcfg directory does not exist :" + cfgfilename);
            return CFG_FILE_DOES_NOT_EXIST;
        }
        try {
            Scanner sc = new Scanner(cfg_file);
            /*while (sc.hasNextLine()) {
                String line = sc.nextLine();
                String[] splits = line.split("\\s+");
                if (splits[0].equals("board")) {
                    String boardcfgfile = System.getenv("IRISCONFIGDIR") + "/Olympus/" + splits[1];
                    BoardManager bc = new BoardManager(system, boardcfgfile, this);
                    int boardinit = bc.initialize();
                    if (boardinit == system.getDAQ().getBoardManager(line) BoardManager.INIT_SUCCESS) {
                        system.logstream("Ethernet", "Loaded " + bc.getName() + " configuration file.");
                        boardlist.put(bc.getName(), bc);
                    } else {
                        system.logstream("Ethernet", "Failed to load " + bc.getName() + " configuration file. Board init error: " + boardinit);
                    }
                } else {
                    ethconfigs.put(splits[0], splits[1]);
                }
            }*/
        } catch (FileNotFoundException ex) {
            system.log("Error loading Ethernet configs.", ex);
            return MISC_ERR;
        }
        int validation = configIsValid();
        if (validation < 0) {
            system.log("Invalid Ethernet configuration. Ethernet error: " + validation);
            return CONFIG_VALIDATION_FAILED;
        }
        return CONFIG_LOAD_SUCCESS;
    }
    
    // Not used, but still required
    @Override
    public int getID() {
        return 5;
    }
    
    @Override
    public int configIsValid() {
        return 0;
    }
    
    @Override
    public void shutdown() {
        system.log("Ethernet module has closed.");
        this.interrupt();
    }
    
    @Override
    public boolean isStarted() {
        return started;
    }
}
    