import java.io.File;
import java.io.FileOutputStream;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Scanner;

public class START {

    public static void main(String[] args) throws Exception {
        /*
         * UDPLISTEN udplisten = new UDPLISTEN(Integer.parseInt(args[0]));
         * udplisten.start();
         */
        boolean is_ingame = false;
        Socket socket = new Socket("localhost", 8080);
        byte[] buffer = new byte[10];
        int read = socket.getInputStream().read(buffer);
        System.out.print(new String(buffer));
        for (int i = 0; i < Integer.parseInt(Character.toString((new String(buffer)).split(" ")[1].charAt(0))); i++) {
            buffer = new byte[12];
            read = socket.getInputStream().read(buffer);
            System.out.println(new String(buffer));
        }
        System.out.println();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            if (!is_ingame) {
                String a = scanner.nextLine();
                // System.out.println(a);
                ByteBuffer byteBuffer = ByteBuffer.allocate(a.length());
                if (a.equals("UNREG***") || a.equals("START***") || a.split(" ")[0].equals("NEWPL")
                        || a.split(" ")[0].equals("GAME?***")) {
                    byteBuffer = ByteBuffer.allocate(a.length());
                    byteBuffer.put(a.getBytes());
                } else if (a.split(" ")[0].equals("REGIS")) {
                    byteBuffer = ByteBuffer.allocate(a.length() - 1);
                    String start = a.split(" ")[0] + " " + a.split(" ")[1] + " " + a.split(" ")[2];
                    byteBuffer.put(start.getBytes());
                    int val = Integer.parseInt(Character.toString(a.split(" ")[3].charAt(0)));
                    byteBuffer.put((byte) val);
                    byteBuffer.put("***".getBytes());
                } else if (a.split(" ")[0].equals("LIST?") || a.split(" ")[0].equals("SIZE?")) {
                    byteBuffer = ByteBuffer.allocate(a.length());
                    String start = a.split(" ")[0] + " ";
                    byteBuffer.put(start.getBytes());
                    byteBuffer.put((byte) Integer.parseInt(Character.toString(a.split(" ")[1].charAt(0))));
                    byteBuffer.put("***".getBytes());
                }
                // System.out.println(byteBuffer.array());
                if (byteBuffer.remaining() == 0) {
                    if (a.split(" ")[0].equals("LIST?")) {
                        socket.getOutputStream().write(byteBuffer.array());
                        buffer = new byte[12];
                        read = socket.getInputStream().read(buffer);
                        String tmpRep = new String(buffer);
                        System.out.print(tmpRep);
                        if (tmpRep.charAt(0) == 'L') {
                            int amount = Integer
                                    .parseInt(Character.toString((new String(buffer)).split(" ")[2].charAt(0)));
                            for (int i = 0; i < amount; i++) {
                                buffer = new byte[17];
                                read = socket.getInputStream().read(buffer);
                                System.out.print(new String(buffer));
                            }
                        }
                        System.out.println();
                    } else if (a.equals("START***")) {
                        socket.getOutputStream().write(byteBuffer.array());
                        buffer = new byte[1024];
                        read = socket.getInputStream().read(buffer);
                        String buffContent = new String(buffer);
                        System.out.println(buffContent);
                        // get ip and port in new String(buffer) from the message "WELCO m h w f ip
                        // port***"
                        String[] buffargs = buffContent.split(" ");
                        String ip = buffargs[5].substring(0, 9);
                        int port = Integer.parseInt(buffargs[6].substring(0, 4));
                        // System.out.println("Ecouter l'ip et le port : " + ip + ":" + port);
                        // Create new bradcast item
                        InetAddress group = InetAddress.getByName(ip);
                        MulticastSocket s = new MulticastSocket(port);
                        s.joinGroup(group);
                        Broadcast broadcast = new Broadcast(s);
                        broadcast.start();
                        // System.out.println("Started broadcast");
                        is_ingame = true;
                        buffer = new byte[1024];
                        read = socket.getInputStream().read(buffer);
                        buffContent = new String(buffer);
                        System.out.println(buffContent);
                    } else {
                        socket.getOutputStream().write(byteBuffer.array());
                        buffer = new byte[1024];
                        read = socket.getInputStream().read(buffer);
                        System.out.println(new String(buffer));
                    }

                } else {
                    System.out.print(a.split(" ")[0]);
                    System.out.println("Invalid command");
                }
            } else {
                String a = scanner.nextLine();
                // System.out.println(a);
                ByteBuffer byteBuffer = ByteBuffer.allocate(a.length());
                if (a.split(" ")[0].equals("MALL?")) {
                    byteBuffer = ByteBuffer.allocate(a.length());
                    byteBuffer.put(a.getBytes());
                    socket.getOutputStream().write(byteBuffer.array());
                    buffer = new byte[1024];
                    read = socket.getInputStream().read(buffer);
                    System.out.println(new String(buffer));
                } else if (a.split(" ")[0].equals("UPMOV") || a.split(" ")[0].equals("DOMOV")
                        || a.split(" ")[0].equals("LEMOV") || a.split(" ")[0].equals("RIMOV")) {
                    byteBuffer = ByteBuffer.allocate(a.length());
                    String start = a.split(" ")[0] + " ";
                    byteBuffer.put(start.getBytes());
                    byteBuffer.put((byte) Integer.parseInt(Character.toString(a.split(" ")[1].charAt(0))));
                    byteBuffer.put("***".getBytes());
                } else {
                    byteBuffer.put(a.getBytes());

                    socket.getOutputStream().write(byteBuffer.array());
                    buffer = new byte[1024];
                    read = socket.getInputStream().read(buffer);
                    System.out.println(new String(buffer));
                }

            }

        }

        // socket.close();

        // Recieve message from server
    }
}