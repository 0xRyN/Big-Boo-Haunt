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
            System.out.println(message);
            byte b = buffer[6];
            nbGames = b & 0xFF;
            String fin = new String(buffer, 7, read - 7);
            System.out.println(message + nbGames + fin);
        } catch (Exception e) {
            System.out.println("Erreur lors de la lecture du nombre de parties0");
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
                System.out.println("Erreur lors de la lecture d'une partie1");
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
                String fin = new String(buffer, 2, read - 2);
                System.out.println(message + first + idGame + fin);
                System.out.println("Vous êtes inscrit dans la partie : " + idGame);
                lobbyWait(socket);
            }

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
                String fin = new String(buffer, 2, read - 2);
                System.out.println(message + first + idGame + fin);
                System.out.println("Vous êtes inscrit dans la partie : " + idGame);
                lobbyWait(socket);
            }

        } catch (Exception e) {
            e.printStackTrace();
            // System.out.println("Erreur lors de l'envoi du message REGIS");
        }

    }

    public static void lobbyWait(Socket socket) {
        while (true) {
            System.out.println("Que voulez vous faire :");
            System.out.println(
                    "0 : Se mettre 'Pret' (Attention vous ne pourrez plus quitter la partie avant son départ)");
            System.out.println("1 :  Afficher la taille d'un labyrinthe");
            System.out.println("2 : Afficher la liste des parties disponnibles");
            System.out.println("3 : Afficher la liste des joueurs dans une partie");
            System.out.println("4 : Quitter la partie");
            Scanner scanner = new Scanner(System.in);
            String commande = scanner.nextLine();
            try {
                int rep = Integer.parseInt(commande);
                if (rep == 0) {
                    inGame(socket);
                }
                if (rep == 1) {
                    System.out.println("Du labyrinthe de quelle partie voulez vous connaitre la taille ?");
                    String id = scanner.nextLine();
                    try {
                        int game_id = Integer.parseInt(id);
                        sizeq(socket, game_id);
                    } catch (Exception e) {
                        System.out.println("Veuillez entrer un id de partie valide");
                    }
                } else if (rep == 2) {
                    try {
                        gamesList(socket);
                    } catch (Exception e) {
                        System.out.println("Erreur lors de l'affichage des parties");
                    }
                } else if (rep == 3) {
                    System.out.println("Veuillez entrer 1 ou 2");
                } else if (rep == 4) {
                    System.out.println("Vous avez quitte la partie");
                    break;
                }
            } catch (Exception e) {
                System.out.println("Veuillez entrer un nombre correct");
            }
        }
    }

    public static void sizeq(Socket socket, int game_id) {
        // send a msg to the server format : SIZEQ game_id***
        try {
            String a = "SIZE? ";
            ByteBuffer byteBuffer = ByteBuffer.allocate(a.length() + 4);
            byteBuffer.put(a.getBytes());
            game_id = game_id & 0xFF;
            byteBuffer.put((byte) game_id);
            byteBuffer.put(("***").getBytes());
            socket.getOutputStream().write(byteBuffer.array());
            byte[] buffer = new byte[5];
            int read = socket.getInputStream().read(buffer);
            String message = new String(buffer, 0, 5);
            System.out.println(message);
            if (message.equals("SIZE!")) {
                buffer = new byte[11];
                read = socket.getInputStream().read(buffer);
                String space = new String(buffer, 0, 1);
                byte b = buffer[1];
                int idGame = b & 0xFF;
                String space2 = new String(buffer, 2, 1);
                byte b2 = buffer[3];
                byte b3 = buffer[4];
                int h = (b2 & 0xff) + (b3 & 0xff) * 0x100;
                String space3 = new String(buffer, 5, 1);
                byte b4 = buffer[6];
                byte b5 = buffer[7];
                int w = (b4 & 0xff) + (b5 & 0xff) * 0x100;
                String fin = new String(buffer, 8, read - 8);
                System.out.println(message + space + idGame + space2 + h + space3 + w + fin);
                System.out.println("La taille du labyrinthe est : " + fin);
            } else {

            }

        } catch (Exception e) {
            System.out.println("Erreur lors de l'envoi du message SIZEQ");
        }
    }

    public static void inGame(Socket socket) {
        String ready = "START***";
        try {
            ByteBuffer byteBuffer = ByteBuffer.allocate(ready.length());
            byteBuffer.put(ready.getBytes());
            socket.getOutputStream().write(byteBuffer.array());
            byte[] buffer = new byte[39];
            int read = socket.getInputStream().read(buffer);
            String start = new String(buffer, 0, 6);
            byte a = buffer[6];
            int m = a & 0xFF;
            String space = new String(buffer, 7, 1);
            byte b = buffer[8];
            byte c = buffer[9];
            int h = (b & 0xff) + (c & 0xff) * 0x100;
            String space2 = new String(buffer, 10, 1);
            byte b2 = buffer[11];
            byte c2 = buffer[12];
            int w = (b2 & 0xff) + (c2 & 0xff) * 0x100;
            String space3 = new String(buffer, 13, 1);
            byte b3 = buffer[14];
            int f = b3 & 0xFF;
            String space4 = new String(buffer, 15, 1);
            String ip = new String(buffer, 16, 15);
            String ip_parsed = ip.substring(0, 9);
            String space5 = new String(buffer, 31, 1);
            String port = new String(buffer, 32, 4);
            int port_parsed = Integer.parseInt(port);
            String fin = new String(buffer, 36, read - 36);
            System.out.println(
                    start + m + space + h + space2 + w + space3 + f + space4 + ip_parsed + space5 + port_parsed + fin);
            byte[] buffer2 = new byte[25];
            read = socket.getInputStream().read(buffer2);
            String start2 = new String(buffer2);
            System.out.println(new String(buffer2));
            InetAddress group = InetAddress.getByName(ip_parsed);
            MulticastSocket s = new MulticastSocket(port_parsed);
            s.joinGroup(group);
            Broadcast broadcast = new Broadcast(s);
            broadcast.start();
        } catch (Exception e) {
            e.printStackTrace();
        }

        while (true) {
        }
    }

    public static void gamesList(Socket socket) {
        String a = "GAME?***";
        ByteBuffer byteBuffer = ByteBuffer.allocate(a.length());
        byteBuffer.put(a.getBytes());
        try {
            socket.getOutputStream().write(byteBuffer.array());
            readGames(socket);
        } catch (Exception e) {
            System.out.println("Erreur lors de l'affichage des parties");
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
                } else if (rep == 2) {
                    System.out.println("Veuillez entrer l'id de la partie que vous voulez rejoindre");
                    String id = scanner.nextLine();
                    try {
                        int game_id = Integer.parseInt(id);
                        regis(socket, nomUtilisateur, portUtilisateur, game_id);
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
