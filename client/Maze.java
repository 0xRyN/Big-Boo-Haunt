import java.io.File;
import java.util.Arrays;
import java.util.Scanner;

public class Maze{

    //parse a maze file of 0 and 1 and put them in a 2d array
    public static int[][] parseMaze(String fileName){
        int[][] maze = new int[0][0];
        try{
            Scanner sc = new Scanner(new File(fileName));
            int row = 0;
            while(sc.hasNextLine()){
                row++;
                sc.nextLine();
            }
            sc.close();
            sc = new Scanner(new File(fileName));
            maze = new int[row][0];
            for(int i = 0; i < row; i++){
                String line = sc.nextLine();
                for(int j = 0; j < line.length(); j++){
                    if(line.charAt(j) == '1'){
                        maze[i] = Arrays.copyOf(maze[i], maze[i].length + 1);
                        maze[i][maze[i].length - 1] = 1;
                    }
                    else{
                        maze[i] = Arrays.copyOf(maze[i], maze[i].length + 1);
                        maze[i][maze[i].length - 1] = 0;
                    }
                }
            }
            sc.close();
        }
        catch(Exception e){
            System.out.println("Error: " + e);
        }
        return maze;
    }

    //function that put all n number of ghosts on zero in the maze
    public static int[][] putGhosts(int[][] maze, int n){
        int[][] mazeCopy = new int[maze.length][maze[0].length];
        for(int i = 0; i < maze.length; i++){
            for(int j = 0; j < maze[i].length; j++){
                mazeCopy[i][j] = maze[i][j];
            }
        }
        for(int i = 0; i < n; i++){
            int x = (int)(Math.random() * maze.length);
            int y = (int)(Math.random() * maze[0].length);
            if(mazeCopy[x][y] == 0){
                mazeCopy[x][y] = 2;
            }
            else{
                i--;
            }
        }
        return mazeCopy;
    }
    

    //function that put all n number of players on zero in the maze
    public static int[][] putPlayers(int[][] maze, int n){
        int[][] mazeCopy = new int[maze.length][maze[0].length];
        for(int i = 0; i < maze.length; i++){
            for(int j = 0; j < maze[i].length; j++){
                mazeCopy[i][j] = maze[i][j];
            }
        }
        for(int i = 0; i < n; i++){
            int x = (int)(Math.random() * maze.length);
            int y = (int)(Math.random() * maze[0].length);
            if(mazeCopy[x][y] == 0){
                mazeCopy[x][y] = 3;
            }
            else{
                i--;
            }
        }
        return mazeCopy;
    }
    


    public static int[][] putAll(int[][] maze, int numberPlayer, int numberGhost){
        return putPlayers(putGhosts(maze, numberGhost), numberPlayer);
    }

    public static void printMaze(int[][] maze){
        for(int i = 0; i < maze.length; i++){
            for(int j = 0; j < maze[i].length; j++){
                System.out.print(maze[i][j]);
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        int[][] maze = parseMaze("Maze/maze1");  
        printMaze(putAll(maze, 4, 20));
    }

}