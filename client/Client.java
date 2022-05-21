import java.io.File;
import java.io.FileOutputStream;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Scanner;

class Client {

    public static String setUsername() {
        System.out.println("Veuillez entrer votre nom d'utilisateur");
        Scanner scanner = new Scanner(System.in);
        String nomUtilisateur = scanner.nextLine();
        if (nomUtilisateur.length() != 8) {
            System.out.println(
                    "Votre nom d'utilisateur n'a pas la bonne longueur veuillez entrer un nom de 8 caractères exactement");
            return setUsername();
        } else {
            return nomUtilisateur;
        }
    }

    public static void readGames(Socket socket) {
        int  nbGames = 0;
        try {
            byte[] buffer = new byte[10];
            int read = socket.getInputStream().read(buffer);
            // Format : GAMES uint8_t***
            String message = new String(buffer, 0, 6);
            byte b = buffer[6];
            nbGames = b & 0xFF;
            String fin = new String(buffer, 7, read - 7);
            System.out.println(message + nbGames + fin);
        } catch (Exception e) {
            System.out.println("Erreur lors de la lecture du nombre de parties");
        }
        for(int i=0;i<nbGames;i++){
            try{
                byte[] buffer = new byte[12];
                int read = socket.getInputStream().read(buffer);
                //format : OGAME uint8_t uint8_t***
                String message = new String(buffer, 0, 6);
                byte b = buffer[6];
                int game_id = b & 0xFF;
                String space = new String(buffer, 7, 1);
                b = buffer[8];
                int nbPlayers = b & 0xFF;
                String fin = new String(buffer, 9, read - 9);
                System.out.println(message + game_id + space + nbPlayers + fin);
            }catch(Exception e){
                System.out.println("Erreur lors de la lecture d'une partie");
            }
        }
    }

    public static void newPl(Socket socket, String username, String port) {
        //send a msg to the server format : NEWPL username port***
        try {
            String a = "NEWPL "+username+" "+port+"***";
            ByteBuffer byteBuffer = ByteBuffer.allocate(a.length());
            byteBuffer.put(a.getBytes());
            //System.out.println(a);
            socket.getOutputStream().write(byteBuffer.array());
            byte[] buffer = new byte[10];
            int read = socket.getInputStream().read(buffer);
            String message = new String(buffer, 0, 6);
            byte b = buffer[6];
            int game_id = b & 0xFF;
            String fin = new String(buffer, 7, read - 7);
            System.out.println(message + game_id + fin);

        } catch (Exception e) {
            System.out.println("Erreur lors de l'envoi du message NEWPL");
        }

    }

    public static void main(String[] args) throws Exception {
        String portUtilisateur;
        Socket socket;
        try {
            portUtilisateur = args[0];
            int portUser = Integer.parseInt(portUtilisateur);
            UDPLISTEN udplisten = new UDPLISTEN(portUser);
            udplisten.start();
        } catch (Exception e) {
            System.out.println("Veuillez entrer un port en premier argument (ex: 'java Client 8080')");
            return;
        }
        System.out.println("Bienvenue sur Big Boo Haunt");
        String nomUtilisateur = setUsername();
        System.out.println("Votre nom d'utilisateur est maintenant : " + nomUtilisateur);

        // COnnect to the server
        try {
            socket = new Socket("localhost", 8080);
            readGames(socket);
        } catch (Exception e) {
            e.printStackTrace();
            // System.out.println("Impossible de se connecter au serveur");
            return;
        }
        newPl(socket, nomUtilisateur, portUtilisateur);
    }
}
