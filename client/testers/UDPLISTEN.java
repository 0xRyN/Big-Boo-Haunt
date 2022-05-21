import java.io.IOException;
import java.net.*;

public class UDPLISTEN extends Thread {
    // Make a class that listen in a UDP socket
    // The class should have a constructor that takes a port number
    // The class should have a run method that listens in a UDP socket
    // The class should have a method that prints the received message

    private int port;
    private DatagramSocket socket;

    public UDPLISTEN(int port) throws SocketException {
        this.port = port;
        socket = new DatagramSocket(port);

    }

    public void run() {
        while (true) {
            byte[] buffer = new byte[10];
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            try {
                socket.receive(packet);
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.out.println(new String(packet.getData()));
        }
    }
}
