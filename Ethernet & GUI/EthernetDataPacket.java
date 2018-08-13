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

import static olympus.modules.daq.DataPacket.PACKET_ID_SEED;

/**
 * @author Drew Kelley Date Authored: Aug 3, 2014 Class: DataPacket
 *
 *
 */
public class EthernetDataPacket {

    private final byte[] data;
    private final byte packetID;
    public static byte PACKET_ID_SEED = 0;
    private final DAQManager manager;

    public EthernetDataPacket(DAQManager man, String msg) {
        synchronized (this) {
            if (PACKET_ID_SEED + 1 < 0) {
                PACKET_ID_SEED = 0;
            }
            packetID = PACKET_ID_SEED++;
        }
        data = new byte[msg.length() + 4];
        manager = man;
        data[0] = Byte.parseByte(manager.get("packet_start"));
        data[1] = packetID;
        data[2] = (byte)(msg.length());
        System.arraycopy(msg.getBytes(), 0, data, 3, msg.length());
        data[data.length - 1] = Byte.parseByte(manager.get("packet_end"));
    }

    public EthernetDataPacket(DAQManager man, byte[] bytes) {
        synchronized (this) {
            packetID = PACKET_ID_SEED++;
        }
        data = new byte[bytes.length + 4];
        manager = man;
        data[0] = Byte.parseByte(manager.get("packet_start"));
        data[1] = (byte) (bytes.length);
        data[2] = packetID;
        System.arraycopy(data, 3, bytes, 0, bytes.length);
        data[data.length - 1] = Byte.parseByte(manager.get("packet_end_byte"));
    }

    public byte[] getData() {
        return data;
    }

    public int getID() {
        return packetID;
    }
}
