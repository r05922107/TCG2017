
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String args[]){
        String line;
        
        try {
        	boolean firstTime = true;
        	int n = 0;
            FileReader fileReader = new FileReader("tcga2016-question.txt");
            BufferedReader br = new BufferedReader(fileReader);
            //find n
            while((line = br.readLine()) != null){
            	if(line.charAt(0) == '$'){
            		if(firstTime){
            			firstTime = false;
            		}else break;
            	}else n++;
            }
            
            //pre-half = column, post-half = row
            ArrayList<List> columnRow = new ArrayList<>();
            
            //parse 
            fileReader = new FileReader("tcga2016-question.txt");
            br = new BufferedReader(fileReader);
            while((line = br.readLine()) != null){
            	if(line.charAt(0) == '$'){
            		for(int i=0; i<n; i++){
            			line = br.readLine();
//            			System.out.println(line);
            			String[] ind = line.split("	");
            			ArrayList<Integer> indexList = new ArrayList<>();
            			for(String temp : ind){
            				indexList.add(Integer.valueOf(temp));
            			}
            			columnRow.add(indexList);
            		}
            		break;
            	}
            }
            
//            //test parsing
//            for(int i=0; i<n; i++){
//            	for(int j=0; j<columnRow.get(i).size(); j++){
//            		System.out.print(columnRow.get(i).get(j) + " ");
//            	}
//            	System.out.println();
//            }
            
            br.close();
            fileReader.close();
            
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
