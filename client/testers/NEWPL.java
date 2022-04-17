import java.io.File;
import java.io.FileOutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

public class NEWPL {
    public static void main(String[] args) throws Exception {
        Socket socket = new Socket("localhost", 8080);
        // Recieve message from server

        byte[] buffer = new byte[1024];
        int read = socket.getInputStream().read(buffer);
        System.out.println(new String(buffer));

        // Send message to server
        ByteBuffer byteBuffer = ByteBuffer.allocate(23);
        byteBuffer.put("NEWPL 12345678 1234 ".getBytes());
        byteBuffer.put("***".getBytes());
        // Print the buffer in output.txt
        System.out.println(byteBuffer.array());
        File file = new File("newpl.txt");
        FileOutputStream fos = new FileOutputStream(file);
        fos.write(byteBuffer.array());
        socket.getOutputStream().write(byteBuffer.array());

        buffer = new byte[1024];
        read = socket.getInputStream().read(buffer);
        System.out.println(new String(buffer));

        socket.close();
        fos.close();
    }
}
