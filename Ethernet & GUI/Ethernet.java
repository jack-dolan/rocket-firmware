/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package olympus.ethernet;

/**
 *
 * @author abagheri
 */
public abstract class Ethernet extends Thread {
    abstract public void terminate();
    abstract public boolean isRunning();
}
