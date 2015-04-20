package com.rockidog.network;

import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPListener implements Runnable {
  private static final int INTERVAL = 100;

  private volatile boolean mStop = false;

  private int mListeningPort;
  private ServerSocket mServer;
  private Socket mClient;
  private InetAddress mClientAddr;
  private int mClientPort;

  public TCPListener(int port) {
    mListeningPort = port;
    try {
      mServer = new ServerSocket(mListeningPort);
      mClient = mServer.accept();
      mClientAddr = mClient.getInetAddress();
      mClientPort = mClient.getPort();
    } catch (IOException e) {
      e.printStackTrace();
    }
    Thread listening = new Thread(this);
    listening.start();
  }

  public InetAddress getClientAddress() {
    return mClientAddr;
  }

  @Override
  public void run() {
    while (mStop == false) {
      synchronized (this) {
        try {
          DataInputStream dis = new DataInputStream(mClient.getInputStream());
          if (dis.available() >= 4) {
            byte[] headerBuffer = new byte[4];
            dis.read(headerBuffer);
            System.out.println("****************************************");
            System.out.println("size:  " + byteArrayToString(headerBuffer, 0, 4));
            
            int size = byteArrayToInteger(headerBuffer);
            System.out.println("size:  " + size);
            
            byte[] buffer = new byte[size];
            for (int offset = 0; offset < size;) {
              int count = dis.read(buffer, offset, size - offset);
              if (count == -1)
                break;
              offset += count;
            }
            
            System.out.println("image head: " + byteArrayToString(buffer, 0, 4));
            System.out.println("image tail: " + byteArrayToString(buffer, size - 4, size));
            
            String path = "/home/soap/Desktop/";
            String filename = Long.toString(System.currentTimeMillis());
            String extname = ".jpeg";
            FileOutputStream fos = new FileOutputStream(path + filename + extname);
            fos.write(buffer);
            fos.close();
            
            byte[] sendingbuffer = new String("Hello my fucking ").getBytes();
            new TCPSendingTask(mClientAddr, mClientPort, sendingbuffer);
          }
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
      
      try {
        Thread.sleep(INTERVAL);
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
  }

  public synchronized void stop() {
    mStop = true;
  }

  private static String byteToHexString(byte b) {
    int v = (int)b;
    v <<= 24;
    v >>>= 24;
    return Integer.toHexString(v);
  }

  private static String byteArrayToString(byte[] b, int offset, int length) {
    String ret = "";
    for (int i = offset; i < length; ++i) {
      ret = ret + byteToHexString(b[i]) + " ";
      if ((i - offset) % 16 == 0 && i != offset)
        ret += "\n";
    }
    return ret;
  }

  private static int byteArrayToInteger(byte[] b) {
    int ret = 0;
    for (int i = 0; i < 4; ++i) {
      ret <<= 8;
      ret += (int)b[i] & 0xFF;
    }
    return ret;
  }
}
