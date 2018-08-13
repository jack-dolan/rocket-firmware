/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package olympus.modules.daq;

import java.io.*;
import java.net.*;
import java.util.Arrays;

public class EthernetTransmitWorker extends Thread {

    private final BoardManager board;
    private final EthernetDataPacket packet;

    public EthernetTransmitWorker(BoardManager brd, EthernetDataPacket pckt) {
        board = brd;
        packet = pckt;
    }

    @Override
    public void run() {
        try {
            String ip = board.getConfig("ip");
            String port = board.getConfig("port_cmd");

            System.out.printf("Sending data to TCP:%s:%s\n", ip, port);
            System.out.printf(Arrays.toString(packet.getData()));

            DatagramSocket serverSocket;
            serverSocket = new DatagramSocket();

            DatagramPacket sendPacket = new DatagramPacket(packet.getData(), 
                    packet.getData().length, InetAddress.getByName(ip), Integer.parseInt(port));

            serverSocket.send(sendPacket);
            serverSocket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
