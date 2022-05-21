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
        int nbGames = 0;
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
        for (int i = 0; i < nbGames; i++) {
            try {
                byte[] buffer = new byte[12];
                int read = socket.getInputStream().read(buffer);
                // format : OGAME uint8_t uint8_t***
                String message = new String(buffer, 0, 6);
                byte b = buffer[6];
                int game_id = b & 0xFF;
                String space = new String(buffer, 7, 1);
                b = buffer[8];
                int nbPlayers = b & 0xFF;
                String fin = new String(buffer, 9, read - 9);
                System.out.println(message + game_id + space + nbPlayers + fin);
            } catch (Exception e) {
                System.out.println("Erreur lors de la lecture d'une partie");
            }
        }
    }

    public static void newPl(Socket socket, String username, String port) {
        // send a msg to the server format : NEWPL username port***
        try {
            String a = "NEWPL " + username + " " + port + "***";
            ByteBuffer byteBuffer = ByteBuffer.allocate(a.length());
            byteBuffer.put(a.getBytes());
            // System.out.println(a);
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

    public static void regis(Socket socket, String username, String port, int game_id) {
        // send a msg to the server format : REGIS username port game_id***
        try {
            String a = "REGIS " + username + " " + port + " ";
            ByteBuffer byteBuffer = ByteBuffer.allocate(a.length() + 1 + 3);
            byteBuffer.put(a.getBytes());
            game_id = game_id & 0xFF;
            byteBuffer.put((byte) game_id);
            byteBuffer.put(("***").getBytes());
            socket.getOutputStream().write(byteBuffer.array());
            File file = new File("output.txt");
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(byteBuffer.array());
            byte[] buffer = new byte[5];
            int read = socket.getInputStream().read(buffer);
            String message = new String(buffer, 0, 5);
            System.out.println(message);
            if (message.equals("REGNO")) {
                buffer = new byte[3];
                read = socket.getInputStream().read(buffer);
                String fin = new String(buffer);
                System.out.println(message + fin);
                System.out.println("Vous n'avez pas pu vous inscrire dans cette partie");
            } else if (message.equals("REGOK")) {
                buffer = new byte[5];
                read = socket.getInputStream().read(buffer);
                String first = new String(buffer, 0, 1);
                byte b = buffer[1];
                int idGame = b & 0xFF;
                String space = new String(buffer, 0, 1);
                String fin = new String(buffer, 2, read - 2);
                System.out.println(message + first + idGame + fin);
                System.out.println("Vous êtes inscrit dans la partie : " + idGame);
            }
            /*
             * byte b = buffer[6];
             * int player_id = b & 0xFF;
             * String fin = new String(buffer, 7, read - 7);
             * System.out.println(message + player_id + fin);
             */

        } catch (Exception e) {
            e.printStackTrace();
            // System.out.println("Erreur lors de l'envoi du message REGIS");
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
        // Read next scanner
        while (true) {
            System.out.println("Que voulez vous faire :");
            System.out.println("1 :  Créer une partie");
            System.out.println("2 : Rejoindre une partie");
            Scanner scanner = new Scanner(System.in);
            String commande = scanner.nextLine();
            try {
                int rep = Integer.parseInt(commande);
                if (rep == 1) {
                    newPl(socket, nomUtilisateur, portUtilisateur);
                    break;
                } else if (rep == 2) {
                    System.out.println("Veuillez entrer l'id de la partie que vous voulez rejoindre");
                    String id = scanner.nextLine();
                    try {
                        int game_id = Integer.parseInt(id);
                        regis(socket, nomUtilisateur, portUtilisateur, game_id);
                        break;
                    } catch (Exception e) {
                        System.out.println("Veuillez entrer un id de partie valide");
                    }
                } else {
                    System.out.println("Veuillez entrer 1 ou 2");
                }
            } catch (Exception e) {
                System.out.println("Veuillez entrer un nombre correct");
            }
        }

        // newPl(socket, nomUtilisateur, portUtilisateur);
        // regis(socket, nomUtilisateur, portUtilisateur, 0);
    }
}
