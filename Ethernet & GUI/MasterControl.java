package gaia.gui;

import gaia.autosequences.AutosequenceRunnable;
import gaia.autosequences.SequenceClock;
import gaia.boards.BoardDescriptor;
import gaia.boards.BoardTable;
import gaia.boards.BoardTableRenderer;
import gaia.calcs.CalcsManager;
import gaia.fdir.FaultCondition;
import gaia.fdir.FaultManager;
import gaia.networking.NetworkProcess;
import gaia.serial.SerialDescriptor;
import gaia.serial.SerialTable;
import gaia.serial.SerialTableRenderer;
import gaia.telemetry.TelemeteredData;
import java.awt.event.WindowEvent;
import java.awt.event.WindowFocusListener;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.TreeSet;
import javax.swing.ImageIcon;
import javax.swing.JOptionPane;

/**
 * MasterControl is the main class for Gaia. It renders the main window's GUI,
 * starts up all of the "Managers", and initiates contact with Olympus
 *
 * @author Drew Kelly
 */
public final class MasterControl extends javax.swing.JFrame implements WindowListener, WindowFocusListener {

    public static final ImageIcon INACTIVE = new ImageIcon(MasterControl.class.getResource("/Images/InactiveLight.fw.png"));
    public static final ImageIcon ACTIVE = new ImageIcon(MasterControl.class.getResource("/Images/ActiveLight.fw.png"));

    public final HashMap<String, javax.swing.JLabel> lights;
    private final HashMap<String, String> configs;
    private final HashMap<String, String> persistence_settings;

    private NetworkProcess commandNetwork;
    private Thread commandThread;
    private NetworkProcess telemetryNetwork;
    private Thread telemetryThread;

    private final FaultManager faultman;
    private final Thread faultthread;
    private boolean conditionalFlag;

    private final CalcsManager calcman;
    private final Thread calcthread;

    private final LogConsole log_console;
    private final ManualCmdEntry cmd_entry;
    private AutosequenceDisplay autosdisplay;
    private TelemetryViewer telemview;
    private SystemOverview sysview;
    private ValveServoControl vscontrol;
    private AutoclaveControl accontrol;
    private LACControl laccontrol;
    private ControllerWindow contWindow;

    private final TreeSet<BoardDescriptor> boardlist;
    private final BoardTable btable;

    private ArrayList<SerialDescriptor> serialdescriptors;
    private final SerialTable stable;

    private final SequenceClock sysclock;

    private final HashMap<String, IndicatorWindow> indicatorWindows;

    /**
     * Creates new form MasterControl
     */
    public MasterControl() {
        if (!new File("Gaia Logs").exists() && !new File("Gaia Logs").isDirectory()) {
            new File("Gaia Logs").mkdir();
        }
        try {
            String name = "Gaia Logs/SystemLog";
            int index = 1;
            while (new File(name + ".log").exists()) {
                name = "Gaia Logs/SystemLog" + (index++);
            }
            System.setErr(new PrintStream(new File(name + ".log")));
        } catch (FileNotFoundException ex) {

        }
        indicatorWindows = new HashMap<>();
        indicatorWindows.put("cmd_indicator", new IndicatorWindow(this, 9000, "CMD", false));
        indicatorWindows.put("tlm_indicator", new IndicatorWindow(this, 9001, "TLM", false));
        indicatorWindows.put("dtws", new IndicatorWindow(this, 9002, "Network Streaming", false));
        indicatorWindows.put("datasaving", new IndicatorWindow(this, 9003, "Data Recording", false));
        indicatorWindows.put("streaming", new IndicatorWindow(this, 9004, "Board Streaming", false));
        indicatorWindows.put("healthpackets", new IndicatorWindow(this, 9005, "Board Health", false));
        indicatorWindows.put("faultmonitor", new IndicatorWindow(this, 9006, "Fault Monitor", false));
        indicatorWindows.put("calcsengine", new IndicatorWindow(this, 9007, "Calcs Engine", false));
        indicatorWindows.put("conditionalflag", new IndicatorWindow(this, 9008, "Conditional Flag", false));
        initComponents();
        lights = new HashMap<>();
        lights.put("cmd_indicator", cmdlight);
        lights.put("tlm_indicator", tlmlight);
        lights.put("dtws", dtwlight);
        lights.put("datasaving", datasavinglight);
        lights.put("streaming", streaminglight);
        lights.put("healthpackets", boardhealthlight);
        lights.put("faultmonitor", faultmonitorlight);
        lights.put("calcsengine", calcslight);
        lights.put("conditionalflag", conditionallight);

        configs = new HashMap<>();
        persistence_settings = new HashMap<>();
        commandNetwork = new NetworkProcess(this, true);
        commandThread = new Thread(commandNetwork);
        telemetryNetwork = new NetworkProcess(this, false);
        telemetryThread = new Thread(telemetryNetwork);
        log_console = new LogConsole(this);
        log_console.setVisible(false);
        cmd_entry = new ManualCmdEntry(this);
        cmd_entry.setVisible(false);

        boardlist = new TreeSet<>();
        btable = new BoardTable(this, boardtable);
        boardtable.setModel(btable);
        boardtable.addMouseListener(btable);
        boardtable.setDefaultRenderer(Object.class, new BoardTableRenderer());

        faultman = new FaultManager(this);
        faultthread = new Thread(faultman);

        calcman = new CalcsManager(this);
        calcthread = new Thread(calcman);

        log("GAIA Online");
        log("Intializing system...");
        loadPersistnace();
        int config_result = loadConfigs();
        if (config_result == Errors.CONFIG_LOADED) {
            log("Configuration data loaded successfully.");
            initializeSystem();
        } else {
            log("Error loading configuration data. System cannot initialize. Error code: " + config_result);
        }

        serialdescriptors = new ArrayList<>();
        stable = new SerialTable(this, serialtable);
        serialtable.setModel(stable);
        serialtable.setDefaultRenderer(Object.class, new SerialTableRenderer());
        sysclock = new SequenceClock(this);

    }

    /**
     * Create all of the child windows and start all necessary threads.
     */
    private void initializeSystem() {
        autosdisplay = new AutosequenceDisplay(this);
        autosdisplay.setVisible(false);
        telemview = new TelemetryViewer(this);
        telemview.setVisible(false);
        sysview = new SystemOverview(this);
        sysview.setVisible(false);
        vscontrol = new ValveServoControl(this);
        accontrol = new AutoclaveControl(this);
        laccontrol = new LACControl(this);
        contWindow = new ControllerWindow(this);
        commandNetwork.initialize();
//        commandNetwork.initConnection();
        telemetryNetwork.initialize();
//        telemetryNetwork.initConnection();
        commandThread.start();
        telemetryThread.start();
        faultman.initialize();
        faultthread.start();
        calcman.initialize();
        calcthread.start();

        addWindowFocusListener(this);
        log("GAIA intialized.");
    }

    private void loadPersistnace() {
        try {
            Scanner sc = new Scanner(new File("persistence.cfg"));
            log("Loading Persistence data...");
            while (sc.hasNextLine()) {
                String[] line = sc.nextLine().split("\\s+");
                log("Loaded persistence setting for: " + line[0]);
                persistence_settings.put(line[0], line[1].replaceAll("_", "\\s"));
            }
            log("Persistence data loaded.");
        } catch (FileNotFoundException ex) {
            log("No persistence file found... skipping");
        }
    }

    /**
     * Verifies the very basic system configuration
     *
     * @return an integer corresponding to an Error in Errors.java, or the
     * success code
     */
    private int configIsValid() {
        if (configs.get("ip_address") == null) {
            return Errors.NO_IP_ADDRESS_SPECIFIED;
        } else if (configs.get("cmd_port") == null) {
            return Errors.NO_CMD_PORT_SPECIFIED;
        } else if (configs.get("telem_port") == null) {
            return Errors.NO_TELEM_PORT_SPECIFIED;
        } else if (configs.get("telem_file") == null) {
            return Errors.NO_TELEM_CONFIG;
        } else if (configs.get("graph_points") == null) {
            return Errors.NO_GRAPH_POINT_COUNT_SPECIFIED;
        } else if (configs.get("packet_start") == null) {
            return Errors.NO_START_BYTE;
        } else if (configs.get("packet_end") == null) {
            return Errors.NO_END_BYTE;
        } else if (configs.get("ack_start") == null) {
            return Errors.NO_ACK_BYTE;
        } else if (configs.get("type_telem") == null) {
            return Errors.NO_TELEM_TYPE;
        } else if (configs.get("type_msg") == null) {
            return Errors.NO_MSG_TYPE;
        } else if (configs.get("type_sysinfo") == null) {
            return Errors.NO_SYSINFO_TYPE;
        } else if (configs.get("fidr_file") == null) {
            return Errors.NO_FDIR_FILE;
        }
        try {
            Byte.parseByte(configs.get("type_sysinfo"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_SYSINFO_TYPE;
        }
        try {
            Byte.parseByte(configs.get("packet_start"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_START_BYTE;
        }
        try {
            Byte.parseByte(configs.get("packet_end"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_END_BYTE;
        }
        try {
            Byte.parseByte(configs.get("ack_start"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_ACK_BYTE;
        }
        try {
            Byte.parseByte(configs.get("type_telem"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_TELEM_TYPE;
        }
        try {
            Byte.parseByte(configs.get("type_msg"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_MSG_TYPE;
        }
        try {
            Integer.parseInt(configs.get("graph_points"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_GRAPH_POINT_COUNT;
        }
        try {
            java.net.InetAddress.getByName(configs.get("ip_address"));
        } catch (UnknownHostException e) {
            return Errors.INVALID_IP_ADDRESS;
        }
        try {
            Integer.parseInt(configs.get("cmd_port"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_CMD_PORT;
        }
        try {
            Integer.parseInt(configs.get("telem_port"));
        } catch (NumberFormatException ex) {
            return Errors.INVALID_TELEM_PORT;
        }
        return Errors.CONFIG_VALID;
    }

    /**
     * Read and process the system.cfg
     *
     * @return the output from configIsValid()
     */
    private int loadConfigs() {
        File cfgfile;
        if (persistence_settings.get("cfgfile") != null && new File(persistence_settings.get("cfgfile")).exists()) {
            cfgfile = new File(persistence_settings.get("cfgfile"));
        } else {
            cfgfile = new File(System.getenv("IRISCONFIGDIR") + "/Gaia/system.cfg");
        }

        try {
            Scanner sc = new Scanner(cfgfile);
            log("Loading configuration data from: " + cfgfile);
            while (sc.hasNextLine()) {
                String[] line = sc.nextLine().split("\\s+");
                configs.put(line[0], line[1]);
            }
        } catch (FileNotFoundException ex) {
            return Errors.NO_SYSTEM_CONFIG_FILE_FOUND;
        }
        return configIsValid();
    }

    /**
     * Controls the status light for specific feature (such as Data Logging,
     * FaultManager, etc)
     *
     * @param name the name of the specified feature as it is set in the
     * 'lights' hashmap
     * @param on true for light on (green), false for off (red)
     */
    public void setIndicator(String name, boolean on) {
        if (name.startsWith("controller.")) {
            contWindow.setControllerStatus(name, on);
            return;
        }
        if (lights.get(name) == null) {
            System.out.println("Invalid name: " + name);
            return;
        }
        if (on) {
            lights.get(name).setIcon(ACTIVE);
        } else {
            lights.get(name).setIcon(INACTIVE);
        }
        indicatorWindows.get(name).setStatus(on);
        repaint();
    }

    public TelemetryViewer telemManager() {
        return telemview;
    }

    public SystemOverview sysView() {
        return sysview;
    }

    public boolean getConditionalFlag() {
        return conditionalFlag;
    }

    public void setConditionalFlag(boolean conditionalFlag) {
        this.conditionalFlag = conditionalFlag;
        setIndicator("conditionalflag", conditionalFlag);
    }

    /**
     * Run the standby autosequence (usually closes all valves)
     *
     * @author Anthony Byrne
     */
    public void standby() {
        getAutoManager().stopAllAutos();
        getAutoManager().setVisible(true);
        getAutoManager().setSelectedAuto("standby");
        new Thread(new AutosequenceRunnable(this, autosdisplay, getAutoManager().getAuto("standby"))).start();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenu2 = new javax.swing.JMenu();
        jMenuItem9 = new javax.swing.JMenuItem();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        jLabel6 = new javax.swing.JLabel();
        jButton3 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jLabel14 = new javax.swing.JLabel();
        jButton6 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();
        jLabel15 = new javax.swing.JLabel();
        jButton7 = new javax.swing.JButton();
        jButton8 = new javax.swing.JButton();
        jLabel16 = new javax.swing.JLabel();
        jButton9 = new javax.swing.JButton();
        jButton10 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton11 = new javax.swing.JButton();
        jButton12 = new javax.swing.JButton();
        jLabel20 = new javax.swing.JLabel();
        jButton13 = new javax.swing.JButton();
        jButton14 = new javax.swing.JButton();
        jButton1 = new javax.swing.JButton();
        cmdlight = new javax.swing.JLabel();
        tlmlight = new javax.swing.JLabel();
        jLabel7 = new javax.swing.JLabel();
        jLabel8 = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        boardtable = new javax.swing.JTable();
        jLabel9 = new javax.swing.JLabel();
        dtwlight = new javax.swing.JLabel();
        streaminglight = new javax.swing.JLabel();
        datasavinglight = new javax.swing.JLabel();
        boardhealthlight = new javax.swing.JLabel();
        jLabel10 = new javax.swing.JLabel();
        jLabel11 = new javax.swing.JLabel();
        jLabel12 = new javax.swing.JLabel();
        jLabel13 = new javax.swing.JLabel();
        jScrollPane2 = new javax.swing.JScrollPane();
        serialtable = new javax.swing.JTable();
        jLabel17 = new javax.swing.JLabel();
        jLabel18 = new javax.swing.JLabel();
        faultmonitorlight = new javax.swing.JLabel();
        sysclockpanel = new gaia.gui.SystemClockDisplay();
        calcslight = new javax.swing.JLabel();
        jLabel19 = new javax.swing.JLabel();
        jLabel21 = new javax.swing.JLabel();
        conditionallight = new javax.swing.JLabel();
        jMenuBar2 = new javax.swing.JMenuBar();
        jMenu3 = new javax.swing.JMenu();
        jMenuItem1 = new javax.swing.JMenuItem();
        jMenuItem12 = new javax.swing.JMenuItem();
        jMenuItem3 = new javax.swing.JMenuItem();
        jMenuItem5 = new javax.swing.JMenuItem();
        jMenuItem6 = new javax.swing.JMenuItem();
        jMenuItem8 = new javax.swing.JMenuItem();
        jMenuItem13 = new javax.swing.JMenuItem();
        jMenuItem11 = new javax.swing.JMenuItem();
        jMenuItem7 = new javax.swing.JMenuItem();
        jMenu4 = new javax.swing.JMenu();
        jMenuItem2 = new javax.swing.JMenuItem();
        jMenuItem4 = new javax.swing.JMenuItem();
        jMenuItem10 = new javax.swing.JMenuItem();

        jMenu1.setText("File");
        jMenuBar1.add(jMenu1);

        jMenu2.setText("Edit");
        jMenuBar1.add(jMenu2);

        jMenuItem9.setText("jMenuItem9");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("GAIA - IRIS Control Software - BU Rocket Propulsion Group");
        setBackground(new java.awt.Color(255, 255, 255));
        setResizable(false);

        jLabel2.setText("Network Connection");

        jLabel3.setFont(new java.awt.Font("Consolas", 0, 10)); // NOI18N
        jLabel3.setText("CMD");
        jLabel3.setMaximumSize(new java.awt.Dimension(20, 20));
        jLabel3.setMinimumSize(new java.awt.Dimension(20, 20));
        jLabel3.setPreferredSize(new java.awt.Dimension(20, 20));

        jLabel4.setFont(new java.awt.Font("Consolas", 0, 10)); // NOI18N
        jLabel4.setText("TLM");
        jLabel4.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jLabel4.setPreferredSize(new java.awt.Dimension(20, 10));

        jPanel2.setBorder(new javax.swing.border.SoftBevelBorder(javax.swing.border.BevelBorder.RAISED));

        jLabel6.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel6.setText("Network Stream");
        jLabel6.setPreferredSize(new java.awt.Dimension(88, 16));

        jButton3.setText("On");
        jButton3.setPreferredSize(new java.awt.Dimension(55, 25));
        jButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton3ActionPerformed(evt);
            }
        });

        jButton5.setText("Off");
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton5ActionPerformed(evt);
            }
        });

        jLabel14.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel14.setText("Board Stream");

        jButton6.setText("Off");
        jButton6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton6ActionPerformed(evt);
            }
        });

        jButton4.setText("On");
        jButton4.setPreferredSize(new java.awt.Dimension(55, 25));
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton4ActionPerformed(evt);
            }
        });

        jLabel15.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel15.setText("Data Recording");
        jLabel15.setToolTipText("");
        jLabel15.setPreferredSize(new java.awt.Dimension(88, 16));

        jButton7.setText("Off");
        jButton7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton7ActionPerformed(evt);
            }
        });

        jButton8.setText("On");
        jButton8.setPreferredSize(new java.awt.Dimension(55, 25));
        jButton8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton8ActionPerformed(evt);
            }
        });

        jLabel16.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel16.setText("Health Packets");
        jLabel16.setPreferredSize(new java.awt.Dimension(88, 16));

        jButton9.setText("Off");
        jButton9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton9ActionPerformed(evt);
            }
        });

        jButton10.setText("On");
        jButton10.setPreferredSize(new java.awt.Dimension(55, 25));
        jButton10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton10ActionPerformed(evt);
            }
        });

        jButton2.setText("Refresh Board/Port List");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        jButton11.setBackground(new java.awt.Color(153, 0, 0));
        jButton11.setText("SYSTEM SAFE (ABORT)");
        jButton11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton11ActionPerformed(evt);
            }
        });

        jButton12.setText("Save Logs");
        jButton12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton12ActionPerformed(evt);
            }
        });

        jLabel20.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel20.setText("Conditional Flag");
        jLabel20.setPreferredSize(new java.awt.Dimension(88, 16));

        jButton13.setText("On");
        jButton13.setPreferredSize(new java.awt.Dimension(55, 25));
        jButton13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton13ActionPerformed(evt);
            }
        });

        jButton14.setText("Off");
        jButton14.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton14ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jButton3, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jButton5, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(jLabel6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .addGap(30, 30, 30)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jButton6, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(jLabel14, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                    .addComponent(jButton2))
                .addGap(30, 30, 30)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jButton12, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                        .addComponent(jButton8, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton7, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jLabel15, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(30, 30, 30)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jLabel16, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jButton10, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jButton9, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(30, 30, 30)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jLabel20, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jButton13, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jButton14, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addContainerGap())
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 123, Short.MAX_VALUE)
                        .addComponent(jButton11))))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(5, 5, 5)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel14)
                            .addComponent(jLabel15, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jButton3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton5, javax.swing.GroupLayout.PREFERRED_SIZE, 25, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton6, javax.swing.GroupLayout.PREFERRED_SIZE, 25, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton8, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton7, javax.swing.GroupLayout.PREFERRED_SIZE, 25, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jLabel20, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jButton13, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jButton14, javax.swing.GroupLayout.PREFERRED_SIZE, 25, javax.swing.GroupLayout.PREFERRED_SIZE)))
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addComponent(jLabel16, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jButton10, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jButton9, javax.swing.GroupLayout.PREFERRED_SIZE, 25, javax.swing.GroupLayout.PREFERRED_SIZE))))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(jButton2))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButton12)
                            .addComponent(jButton11))
                        .addGap(0, 0, Short.MAX_VALUE))))
        );

        jButton1.setText("Connect");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        cmdlight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        cmdlight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                cmdlightMouseClicked(evt);
            }
        });

        tlmlight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        tlmlight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                tlmlightMouseClicked(evt);
            }
        });

        jLabel7.setText("Olympus System Status");

        jLabel8.setText("System Controls");

        boardtable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null}
            },
            new String [] {
                "Title 1", "Title 2", "Title 3", "Title 4"
            }
        ));
        jScrollPane1.setViewportView(boardtable);

        jLabel9.setText("Boards");

        dtwlight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        dtwlight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                dtwlightMouseClicked(evt);
            }
        });

        streaminglight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        streaminglight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                streaminglightMouseClicked(evt);
            }
        });

        datasavinglight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        datasavinglight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                datasavinglightMouseClicked(evt);
            }
        });

        boardhealthlight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        boardhealthlight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                boardhealthlightMouseClicked(evt);
            }
        });

        jLabel10.setText("Network Streaming");

        jLabel11.setText("Board Streaming");

        jLabel12.setText("Data Recording");

        jLabel13.setText("Board Health Beacons");

        serialtable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null}
            },
            new String [] {
                "Title 1", "Title 2", "Title 3", "Title 4"
            }
        ));
        jScrollPane2.setViewportView(serialtable);

        jLabel17.setText("Serial Ports");

        jLabel18.setText("Fault Monitor");

        faultmonitorlight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        faultmonitorlight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                faultmonitorlightMouseClicked(evt);
            }
        });

        calcslight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        calcslight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                calcslightMouseClicked(evt);
            }
        });

        jLabel19.setText("Calcs Engine");

        jLabel21.setText("Conditional Flag");

        conditionallight.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Images/InactiveLight.fw.png"))); // NOI18N
        conditionallight.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                conditionallightMouseClicked(evt);
            }
        });

        jMenuBar2.setBorder(javax.swing.BorderFactory.createBevelBorder(javax.swing.border.BevelBorder.RAISED, null, null, null, new java.awt.Color(153, 153, 153)));

        jMenu3.setText("View");

        jMenuItem1.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem1.setText("View Console");
        jMenuItem1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem1ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem1);

        jMenuItem12.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_U, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem12.setText("Session Manager");
        jMenuItem12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem12ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem12);

        jMenuItem3.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_D, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem3.setText("Manual Command Entry");
        jMenuItem3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem3ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem3);

        jMenuItem5.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_E, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem5.setText("Autosequences");
        jMenuItem5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem5ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem5);

        jMenuItem6.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_A, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem6.setText("Telemetry");
        jMenuItem6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem6ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem6);

        jMenuItem8.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_N, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem8.setText("Valve Control");
        jMenuItem8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem8ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem8);

        jMenuItem13.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_C, java.awt.event.InputEvent.SHIFT_MASK | java.awt.event.InputEvent.CTRL_MASK));
        jMenuItem13.setText("Controllers");
        jMenuItem13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem13ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem13);

        jMenuItem11.setText("Autoclave Control");
        jMenuItem11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem11ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem11);

        jMenuItem7.setText("LAC Control");
        jMenuItem7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem7ActionPerformed(evt);
            }
        });
        jMenu3.add(jMenuItem7);

        jMenuBar2.add(jMenu3);

        jMenu4.setText("Clock Options");

        jMenuItem2.setText("Start Clock");
        jMenuItem2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem2ActionPerformed(evt);
            }
        });
        jMenu4.add(jMenuItem2);

        jMenuItem4.setText("Stop Clock");
        jMenuItem4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem4ActionPerformed(evt);
            }
        });
        jMenu4.add(jMenuItem4);

        jMenuItem10.setText("Set Clock");
        jMenuItem10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem10ActionPerformed(evt);
            }
        });
        jMenu4.add(jMenuItem10);

        jMenuBar2.add(jMenu4);

        setJMenuBar(jMenuBar2);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(cmdlight)
                            .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 29, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(tlmlight)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jButton1)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 63, Short.MAX_VALUE)
                        .addComponent(sysclockpanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(jLabel11)
                                            .addComponent(jLabel10)
                                            .addComponent(jLabel12)
                                            .addComponent(jLabel13, javax.swing.GroupLayout.Alignment.TRAILING))
                                        .addGap(10, 10, 10)
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(datasavinglight)
                                            .addComponent(boardhealthlight)
                                            .addComponent(streaminglight)
                                            .addComponent(dtwlight)))
                                    .addComponent(jLabel7)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(jLabel18)
                                            .addComponent(jLabel19)
                                            .addComponent(jLabel21))
                                        .addGap(52, 52, 52)
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(calcslight)
                                            .addComponent(faultmonitorlight)
                                            .addComponent(conditionallight))))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel17, javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(jLabel9, javax.swing.GroupLayout.Alignment.TRAILING)))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel8)
                                .addGap(0, 0, Short.MAX_VALUE)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 367, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 367, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addGap(10, 10, 10))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(cmdlight)
                            .addComponent(tlmlight)
                            .addComponent(jButton1))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(29, 29, 29))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(sysclockpanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 97, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(14, 14, 14)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 97, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(jLabel11)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                            .addGroup(layout.createSequentialGroup()
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                                    .addComponent(jLabel7)
                                                    .addComponent(jLabel9))
                                                .addGap(39, 39, 39))
                                            .addComponent(jLabel10))
                                        .addGap(26, 26, 26)))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jLabel12)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(14, 14, 14)
                                        .addComponent(jLabel13))
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(4, 4, 4)
                                        .addComponent(jLabel17))))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(dtwlight)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(streaminglight)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(datasavinglight)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(boardhealthlight)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel18)
                            .addComponent(faultmonitorlight))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel19)
                            .addComponent(calcslight))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(conditionallight)
                            .addComponent(jLabel21))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 11, Short.MAX_VALUE)
                        .addComponent(jLabel8)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(10, 10, 10))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jMenuItem1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem1ActionPerformed
        log_console.setVisible(true);
    }//GEN-LAST:event_jMenuItem1ActionPerformed

    private void jMenuItem3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem3ActionPerformed
        cmd_entry.setVisible(true);
    }//GEN-LAST:event_jMenuItem3ActionPerformed

    private void jMenuItem5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem5ActionPerformed
        autosdisplay.setVisible(true);
    }//GEN-LAST:event_jMenuItem5ActionPerformed

    private void jMenuItem6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem6ActionPerformed
        telemview.setVisible(true);
    }//GEN-LAST:event_jMenuItem6ActionPerformed

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        if (!getCommandNetwork().isConnected()) {
            commandNetwork = new NetworkProcess(this, true);
            commandNetwork.initialize();
            commandThread = new Thread(commandNetwork);
            commandThread.start();
        }
        if (!getTelemetryNetwork().isConnected()) {
            telemetryNetwork = new NetworkProcess(this, false);
            telemetryNetwork.initialize();
            telemetryThread = new Thread(telemetryNetwork);
            telemetryThread.start();
        }
    }//GEN-LAST:event_jButton1ActionPerformed

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        getCommandNetwork().sendMessage("DAQ REFRESH");
        getCommandNetwork().sendMessage("DAQ LIST");
        getCommandNetwork().sendMessage("DAQ CONNECTED?");
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jMenuItem8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem8ActionPerformed
        vscontrol.setVisible(true);
    }//GEN-LAST:event_jMenuItem8ActionPerformed

    private void jButton3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton3ActionPerformed
        getCommandNetwork().sendMessage("NETWORK DTW START");
    }//GEN-LAST:event_jButton3ActionPerformed

    private void jButton5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton5ActionPerformed
        getCommandNetwork().sendMessage("NETWORK DTW STOP");
    }//GEN-LAST:event_jButton5ActionPerformed

    private void jButton6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton6ActionPerformed
        getCommandNetwork().sendMessage("DAQ STREAMING STOP");
    }//GEN-LAST:event_jButton6ActionPerformed

    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton4ActionPerformed
        getCommandNetwork().sendMessage("DAQ STREAMING START");
    }//GEN-LAST:event_jButton4ActionPerformed

    private void jButton7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton7ActionPerformed
        getCommandNetwork().sendMessage("DAQ LOGGING STOP");
    }//GEN-LAST:event_jButton7ActionPerformed

    private void jButton8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton8ActionPerformed
        getCommandNetwork().sendMessage("DAQ LOGGING START");
    }//GEN-LAST:event_jButton8ActionPerformed

    private void jButton9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton9ActionPerformed
        getCommandNetwork().sendMessage("DAQ HEALTH STOP");
    }//GEN-LAST:event_jButton9ActionPerformed

    private void jButton10ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton10ActionPerformed
        getCommandNetwork().sendMessage("DAQ HEALTH START");
    }//GEN-LAST:event_jButton10ActionPerformed

    private void jButton11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton11ActionPerformed
        getAutoManager().stopAllAutos();
        getAutoManager().setVisible(true);
        getAutoManager().setSelectedAuto("abort_auto");
        new Thread(new AutosequenceRunnable(this, autosdisplay, getAutoManager().getAuto("abort_auto"))).start();
        JOptionPane.showMessageDialog(this, "Abort Executed", "Abort", JOptionPane.OK_OPTION);
    }//GEN-LAST:event_jButton11ActionPerformed

    private void jButton12ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton12ActionPerformed
        String str = (String) (JOptionPane.showInputDialog(this, "Test Name:", "Save old logs", JOptionPane.PLAIN_MESSAGE, null, null, null));
        if (str != null) {
            if (str.matches("(\\s+)") || str.length() == 0) {
                JOptionPane.showMessageDialog(this, "Invalid directory name.", "Cannot create new logs", JOptionPane.ERROR_MESSAGE);
                return;
            }
            getCommandNetwork().sendMessage("DAQ LOGGING NEW " + str);
        }
    }//GEN-LAST:event_jButton12ActionPerformed

    private void jMenuItem2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem2ActionPerformed
        log("Starting clock");
        sysclock.start();
        updateClock(sysclock.getTime());

    }//GEN-LAST:event_jMenuItem2ActionPerformed

    private void jMenuItem4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem4ActionPerformed
        log("Stopping clock");
        sysclock.stop();
        updateClock(sysclock.getTime());
    }//GEN-LAST:event_jMenuItem4ActionPerformed

    private void jMenuItem10ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem10ActionPerformed
        String newvalue = JOptionPane.showInputDialog(this, "Enter new clock value in MS", "New Value");
        if (newvalue == null) {
            return;
        }
        try {
            int res = Integer.parseInt(newvalue);
            log("Updating clock to " + res + " milliseconds");
            sysclock.setTime(res);
            updateClock(sysclock.getTime());
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "Invalid value", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }//GEN-LAST:event_jMenuItem10ActionPerformed

    private void jMenuItem11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem11ActionPerformed
        accontrol.setVisible(true);
    }//GEN-LAST:event_jMenuItem11ActionPerformed

    private void datasavinglightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_datasavinglightMouseClicked
        indicatorWindows.get("datasaving").setVisible(true);
    }//GEN-LAST:event_datasavinglightMouseClicked

    private void dtwlightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_dtwlightMouseClicked
        indicatorWindows.get("dtws").setVisible(true);
    }//GEN-LAST:event_dtwlightMouseClicked

    private void streaminglightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_streaminglightMouseClicked
        indicatorWindows.get("streaming").setVisible(true);
    }//GEN-LAST:event_streaminglightMouseClicked

    private void boardhealthlightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_boardhealthlightMouseClicked
        indicatorWindows.get("healthpackets").setVisible(true);
    }//GEN-LAST:event_boardhealthlightMouseClicked

    private void faultmonitorlightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_faultmonitorlightMouseClicked
        indicatorWindows.get("faultmonitor").setVisible(true);
    }//GEN-LAST:event_faultmonitorlightMouseClicked

    private void calcslightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_calcslightMouseClicked
        indicatorWindows.get("calcsengine").setVisible(true);
    }//GEN-LAST:event_calcslightMouseClicked

    private void cmdlightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_cmdlightMouseClicked
        indicatorWindows.get("cmd_indicator").setVisible(true);
    }//GEN-LAST:event_cmdlightMouseClicked

    private void tlmlightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_tlmlightMouseClicked
        indicatorWindows.get("tlm_indicator").setVisible(true);
    }//GEN-LAST:event_tlmlightMouseClicked

    private void jMenuItem12ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem12ActionPerformed
        telemview.showSessionManager();
    }//GEN-LAST:event_jMenuItem12ActionPerformed

    private void jButton13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton13ActionPerformed
        setConditionalFlag(true);
    }//GEN-LAST:event_jButton13ActionPerformed

    private void jButton14ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton14ActionPerformed
        setConditionalFlag(false);
    }//GEN-LAST:event_jButton14ActionPerformed

    private void conditionallightMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_conditionallightMouseClicked
        indicatorWindows.get("conditionalflag").setVisible(true);
    }//GEN-LAST:event_conditionallightMouseClicked

    private void jMenuItem13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem13ActionPerformed
        contWindow.setVisible(true);
    }//GEN-LAST:event_jMenuItem13ActionPerformed

    private void jMenuItem7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem7ActionPerformed
        laccontrol.setVisible(true);
    }//GEN-LAST:event_jMenuItem7ActionPerformed

    public TelemeteredData getTelem(String telem) {
        return telemview.getTelem(telem);
    }

    public IndicatorWindow[] getIndicatorWindowArray() {
        IndicatorWindow[] returnArray = new IndicatorWindow[indicatorWindows.size()];

        returnArray[0] = indicatorWindows.get("datasaving");
        returnArray[1] = indicatorWindows.get("dtws");
        returnArray[2] = indicatorWindows.get("streaming");
        returnArray[3] = indicatorWindows.get("healthpackets");
        returnArray[4] = indicatorWindows.get("faultmonitor");
        returnArray[5] = indicatorWindows.get("calcsengine");
        returnArray[6] = indicatorWindows.get("cmd_indicator");
        returnArray[7] = indicatorWindows.get("tlm_indicator");
        returnArray[8] = indicatorWindows.get("conditionalflag");

//        int i = 0;
//        
//        
//        for(IndicatorWindow window : indicatorWindows.values()){
//            returnArray[i] = window;
//            i++;
//        }
        return returnArray;
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) "> 
        try {
            javax.swing.UIManager.setLookAndFeel("com.jtattoo.plaf.hifi.HiFiLookAndFeel");

        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(MasterControl.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(MasterControl.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(MasterControl.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(MasterControl.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MasterControl().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel boardhealthlight;
    private javax.swing.JTable boardtable;
    private javax.swing.JLabel calcslight;
    private javax.swing.JLabel cmdlight;
    private javax.swing.JLabel conditionallight;
    private javax.swing.JLabel datasavinglight;
    private javax.swing.JLabel dtwlight;
    private javax.swing.JLabel faultmonitorlight;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton10;
    private javax.swing.JButton jButton11;
    private javax.swing.JButton jButton12;
    private javax.swing.JButton jButton13;
    private javax.swing.JButton jButton14;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JButton jButton6;
    private javax.swing.JButton jButton7;
    private javax.swing.JButton jButton8;
    private javax.swing.JButton jButton9;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel12;
    private javax.swing.JLabel jLabel13;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel16;
    private javax.swing.JLabel jLabel17;
    private javax.swing.JLabel jLabel18;
    private javax.swing.JLabel jLabel19;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel20;
    private javax.swing.JLabel jLabel21;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenu jMenu2;
    private javax.swing.JMenu jMenu3;
    private javax.swing.JMenu jMenu4;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuBar jMenuBar2;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JMenuItem jMenuItem10;
    private javax.swing.JMenuItem jMenuItem11;
    private javax.swing.JMenuItem jMenuItem12;
    private javax.swing.JMenuItem jMenuItem13;
    private javax.swing.JMenuItem jMenuItem2;
    private javax.swing.JMenuItem jMenuItem3;
    private javax.swing.JMenuItem jMenuItem4;
    private javax.swing.JMenuItem jMenuItem5;
    private javax.swing.JMenuItem jMenuItem6;
    private javax.swing.JMenuItem jMenuItem7;
    private javax.swing.JMenuItem jMenuItem8;
    private javax.swing.JMenuItem jMenuItem9;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTable serialtable;
    private javax.swing.JLabel streaminglight;
    private gaia.gui.SystemClockDisplay sysclockpanel;
    private javax.swing.JLabel tlmlight;
    // End of variables declaration//GEN-END:variables

    public String getConfig(String key) {
        return configs.get(key);
    }

    /**
     * Simply adds a string to the log with no fanfare 
     * @param str the string being added
     */
    public void log(String str) {
        log_console.addText(str);
    }

    /**
     * Adds a string to the log and launches a DammitKamWindow that explains the specified exception
     * @param str the string being added to the log
     * @param ex the exception being passed to DammitKamWindow
     */
    public void log(String str, Exception ex) {
        str = "Error: " + str + ". " + ex.getMessage();
        log_console.addText(str);
        new DammitKamWindow(str, ex).setVisible(true);
    }

    /**
     * Adds a string to the log and launches a DammitKamWindow that explains the specified fault
     * @param str the string being added to the log
     * @param fc the FaultCondition being passed to DammitKamWindow
     */
    public void log(String str, FaultCondition fc) {
        str = "Error: " + str + ". " + fc.toString();
        log_console.addText(str);
        new DammitKamWindow(str, fc).setVisible(true);
    }

    public void setConfig(String key, String text) {
        configs.put(key, text);
    }

    public NetworkProcess getCommandNetwork() {
        return commandNetwork;
    }

    public NetworkProcess getTelemetryNetwork() {
        return telemetryNetwork;
    }

    @Override
    public void windowOpened(WindowEvent we) {
    }

    @Override
    public void windowClosing(WindowEvent we) {
        shutdown();
    }

    @Override
    public void windowClosed(WindowEvent we) {
    }

    @Override
    public void windowIconified(WindowEvent we) {
    }
        //JOptionPane.showMessageDialog(this, "Standby Executed", "Standby", JOptionPane.WARNING_MESSAGE);
    @Override
    public void windowDeiconified(WindowEvent we) {
    }

    @Override
    public void windowActivated(WindowEvent we) {
    }

    @Override
    public void windowDeactivated(WindowEvent we) {
    }

    /**
     * Cleanly kill all threads.
     */
    private void shutdown() {
        commandThread.interrupt();
        telemetryThread.interrupt();
        calcthread.interrupt();
    }

    private boolean focused;

    @Override
    public void windowGainedFocus(WindowEvent we) {
        if (!focused) {
            if (log_console.isVisible()) {
                log_console.setVisible(true);
            }
            if (telemview.isVisible()) {
                telemview.setVisible(true);
            }
            if (sysview.isVisible()) {
                sysview.setVisible(true);
            }
            if (autosdisplay.isVisible()) {
                autosdisplay.setVisible(true);
            }
            if (cmd_entry.isVisible()) {
                cmd_entry.setVisible(true);
            }
            setVisible(true);
            focused = true;
        } else {
            focused = false;
        }
    }

    @Override
    public void windowLostFocus(WindowEvent we) {
    }

    public void addBoard(String descrip) {
        synchronized (boardlist) {
            boardlist.add(new BoardDescriptor(this, descrip));
            btable.update();
            boardtable.repaint();
            repaint();
        }
    }

    public void markConnected(String name) {
        for (BoardDescriptor bd : boardlist) {
            if (bd.getName().equals(name)) {
                bd.setConnected(true);
            }
        }
        btable.update();
        boardtable.repaint();
        repaint();
    }

    public void unmarkConnected(String name) {
        for (BoardDescriptor bd : boardlist) {
            if (bd.getName().equals(name)) {
                bd.setConnected(false);
            }
        }
        btable.update();
        boardtable.repaint();
        repaint();
    }

    public TreeSet<BoardDescriptor> getBoards() {
        return boardlist;
    }

    public void updateSerialDescriptors(ArrayList<SerialDescriptor> newList) {
        serialdescriptors = newList;
        stable.update();
    }

    public ArrayList<SerialDescriptor> getSerialDescriptors() {
        return serialdescriptors;
    }

    public AutosequenceDisplay getAutoManager() {
        return autosdisplay;
    }

    public void updateClock(int time) {
        sysclockpanel.setActive(sysclock.isRunning());
        sysclockpanel.updateTime(time);
    }

    public SequenceClock clock() {
        return sysclock;
    }
}
