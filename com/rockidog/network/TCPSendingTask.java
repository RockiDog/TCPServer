package com.rockidog.network;

import java.io.IOException;
import java.net.Socket;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.io.DataOutputStream;

public class TCPSendingTask extends Thread {

  private Socket mSender;
  private byte[] mData;

  public TCPSendingTask(InetAddress addr, int port, byte[] data) {
    try {
      mSender = new Socket(addr.getHostName(), port + 1);
    } catch (UnknownHostException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    }
    mData = data.clone();
  }

  @Override
  public void run() {
    try {
      DataOutputStream dos = new DataOutputStream(mSender.getOutputStream());
      dos.writeInt(mData.length);
      dos.write(mData);
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
