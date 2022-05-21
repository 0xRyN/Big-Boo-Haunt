public class sendudp {
    // SEND A MESSAGE TO A UDP SOCKET
    // The class should have a constructor that takes a port number
    // The class should have a method that sends a message to a UDP socket
    // The class should have a method that prints the sent message

    private int port;
    private DatagramSocket socket;

    public sendudp(int port) {
        this.port = port;
        try {
            socket = new DatagramSocket(port);
        } catch (SocketException e) {
            System.out.println("error");
            e.printStackTrace();
        }
    }

    public void send(String message) {
        byte[] buffer = message.getBytes();
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length, InetAddress.getByName("localhost"), port);
        try {
            socket.send(packet);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
