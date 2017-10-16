import java.io.*;

public class Main {
    public static void main(String args[]){
        parse("tcga2016-question.txt");


    }

    private int[][] parse(String inputName){
        String line;
        try {
            FileReader fileReader = new FileReader(inputName);
            BufferedReader br = new BufferedReader(fileReader);
            while((line = br.readLine()) != null){

            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
