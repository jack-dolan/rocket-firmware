/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package olympus.modules.daq;

import olympus.OlympusMain;
import java.io.IOException;
import java.net.*;

 
public class EthernetReceiver extends Thread {
    private final DAQManager man;
    private final BoardManager board;
    private final OlympusMain system;

    byte a;
    int b = 0;
    
    public EthernetReceiver(OlympusMain sys, DAQManager man, BoardManager brd) {
        this.man = man;
        board = brd;
        system = sys;
    }
     
    @Override
    public void run() {
//        String port = board.getConfig("port_tel");
        String port = "2005";
        try {
            DatagramSocket serverSocket = new DatagramSocket(Integer.parseInt(port));
            byte[] receiveData = new byte[32];
            System.out.printf("Listening on UDP:%s:%s%n",
                    InetAddress.getLocalHost().getHostAddress(), port);
            DatagramPacket receivePacket = new DatagramPacket(receiveData,
                           receiveData.length);
        while(true)
        {
              serverSocket.receive(receivePacket);
              //String sentence = new String(receivePacket.getData(), 0, receivePacket.getLength());
	      //System.out.println("RECEIVED: " + sentence);
              parser(receivePacket);           
        }
      } catch (IOException e) {
              System.out.println(e);
      }
    }
    
    private void parser(DatagramPacket packet) {
        byte[] data = packet.getData();
        if (system.getConfig("verbose_mode") != null && system.getConfig("verbose_mode").equals("true")) {
            System.out.println("Packet from " + packet.getAddress() + ":" + packet.getPort() + ": " + java.util.Arrays.toString(data));
        }
        if (data[0] == Byte.parseByte(man.get("packet_start"))) {
            if (data[2] == Byte.parseByte(man.get("telem_packet_id"))) {
                byte telem_id = data[3];
                if (man.getTelem(telem_id) == null) {
                    return;
                }
                if (man.getTelem(telem_id).isFloat()) {
                    int temp = ((int) (data[7] & 0xFF) << 24) + ((int) (data[6] & 0xFF) << 16) + ((int) (data[5] & 0xFF) << 8) + ((int) (data[4] & 0xFF));
                    int counter = ((int) (data[8] & 0xFF) << 24) + ((int) (data[9] & 0xFF) << 16) + ((int) (data[10] & 0xFF) << 8) + ((int) (data[11] & 0xFF));
                    man.getTelem(telem_id).setData(Float.intBitsToFloat(temp), counter);
                } else if (man.getTelem(telem_id).isInt()) {
                    int temp = ((int) (data[7] & 0xFF) << 24) + ((int) (data[6] & 0xFF) << 16) + ((int) (data[5] & 0xFF) << 8) + ((int) (data[4] & 0xFF));
                    int counter = ((int) (data[8] & 0xFF) << 24) + ((int) (data[9] & 0xFF) << 16) + ((int) (data[10] & 0xFF) << 8) + ((int) (data[11] & 0xFF));
                    man.getTelem(telem_id).setData(temp, counter);
                } else if (man.getTelem(telem_id).isString()) {
                    String str = "";
                    for (int i = 4; i < ((int) data[1]) - 4; i++) {
                        str += (char) data[i];
                    }
                    man.getTelem(telem_id).setData(str);
                }
            }
        }
    }
}
