import java.io.*;
import java.net.*;

public class dany {
    public static void main(String[] args) {
        try {
            DatagramSocket dso = new DatagramSocket();
            byte[] data;
            for (int i = 0; i <= 10; i++) {
                String s = "MESSAGE " + i;
                data = s.getBytes();
                InetSocketAddress ia = new InetSocketAddress("224.1.1.1", 5402);
                DatagramPacket paquet = new DatagramPacket(data, data.length, ia);
                dso.send(paquet);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}