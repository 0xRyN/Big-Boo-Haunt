import java.io.IOException;
import java.net.Socket;

public class Playgame{
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("localhost", 8080);
        // Recieve message from server

        byte[] buffer = new byte[1024];
        int read = socket.getInputStream().read(buffer);
        System.out.println(new String(buffer));
        while(true){
            
        }
    }
}