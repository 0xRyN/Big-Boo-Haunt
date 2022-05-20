import java.net.*;
public class Broadcast implements Runnable {
    private static DatagramPacket packet;
    private static MulticastSocket socket;
    private static byte[] buffer;

    public Broadcast(MulticastSocket socket){
        Broadcast.packet = new DatagramPacket(new byte[1024], 1024);
        Broadcast.socket = socket;
        Broadcast.buffer = new byte[1024];
    }

    @Override
    public void run() {
        while(true){
            try {
                socket.receive(packet);
                buffer = packet.getData();
                System.out.println(new String(packet.getData()));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
}
