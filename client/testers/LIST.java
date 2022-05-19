import java.io.File;
import java.io.FileOutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Scanner;


public class LIST {
    public static void main(String[] args) throws Exception {


        Socket socket = new Socket("localhost", 8080);

        byte[] buffer = new byte[1024];
        int read = socket.getInputStream().read(buffer);
        System.out.println(new String(buffer));

        // Send message to server
        ByteBuffer byteBuffer = ByteBuffer.allocate(10);
        byteBuffer.put("SIZE? ".getBytes());
        byteBuffer.put((byte) 1);
        byteBuffer.put("***".getBytes());
        // Print the buffer in output.txt
        //System.out.println(byteBuffer.array());
        File file = new File("output.txt");
        FileOutputStream fos = new FileOutputStream(file);
        fos.write(byteBuffer.array());
        socket.getOutputStream().write(byteBuffer.array());

        buffer = new byte[1024];
        read = socket.getInputStream().read(buffer);
        System.out.println(new String(buffer));

        // Create scanner
        Scanner scanner = new Scanner(System.in);
        scanner.next();
        socket.close();
        fos.close();
        scanner.close();
    }
}
