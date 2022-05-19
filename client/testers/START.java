import java.io.File;
import java.io.FileOutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Scanner;

public class START {
   public static void main(String[] args) throws Exception {
      Socket socket = new Socket("localhost", 8080);
      byte[] buffer = new byte[1024];
      int read = socket.getInputStream().read(buffer);
      System.out.println(new String(buffer));
      Scanner scanner = new Scanner(System.in);

        while(true){
            String a = scanner.nextLine();
            //System.out.println(a);
            ByteBuffer byteBuffer = ByteBuffer.allocate(a.length());
            if(a.split(" ")[0].equals("NEWPL")){
                byteBuffer = ByteBuffer.allocate(a.length());
                byteBuffer.put(a.getBytes());
            }
            else if(a.equals("UNREG***")){
                byteBuffer = ByteBuffer.allocate(a.length());
                byteBuffer.put(a.getBytes());
            }
            else if(a.equals("START***")){
                byteBuffer = ByteBuffer.allocate(a.length());
                byteBuffer.put(a.getBytes());
            }
            else if(a.split(" ")[0].equals("REGIS")){
                byteBuffer = ByteBuffer.allocate(a.length()-1);
                String start = a.split(" ")[0]+" "+a.split(" ")[1]+" "+a.split(" ")[2];
                byteBuffer.put(start.getBytes());
                int val = Integer.parseInt(Character.toString(a.split(" ")[3].charAt(0)));
                byteBuffer.put((byte) val);
                byteBuffer.put("***".getBytes());
            }
            else if(a.split(" ")[0].equals("LIST?")){
                byteBuffer = ByteBuffer.allocate(a.length());
                String start = a.split(" ")[0]+" ";
                byteBuffer.put(start.getBytes());
                byteBuffer.put((byte) Integer.parseInt(Character.toString(a.split(" ")[1].charAt(0))));
                byteBuffer.put("***".getBytes());
            }
            //System.out.println(byteBuffer.array());
            if(byteBuffer.remaining() == 0){
                socket.getOutputStream().write(byteBuffer.array());
    
                buffer = new byte[1024];
                read = socket.getInputStream().read(buffer);
                System.out.println(new String(buffer));
            }
            else{
                System.out.print(a.split(" ")[0]);
                System.out.println("Invalid command");
            }
        }
        //socket.close();

        // Recieve message from server
   }
}