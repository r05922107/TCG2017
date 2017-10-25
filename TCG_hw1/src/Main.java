
import sun.rmi.server.InactiveGroupException;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String args[]){
        String line;
        
//        try {
//        	boolean firstTime = true;
//        	int n = 0;
//            FileReader fileReader = new FileReader("tcga2016-question.txt");
//            BufferedReader br = new BufferedReader(fileReader);
//            //find n
//            while((line = br.readLine()) != null){
//            	if(line.charAt(0) == '$'){
//            		if(firstTime){
//            			firstTime = false;
//            		}else break;
//            	}else n++;
//            }
//
//            //pre-half = column, post-half = row
//
//            //parse
//            fileReader = new FileReader("tcga2016-question.txt");
//            br = new BufferedReader(fileReader);
//            while((line = br.readLine()) != null){
//                if(line.length() < 2){
//                    continue;
//                }
//            	if(line.charAt(0) == '$'){
//                    ArrayList<List> columnRow = new ArrayList<>();
//            		for(int i=0; i<n; i++){
//            			line = br.readLine();
////            			System.out.println(line);
//                        ArrayList<Integer> indexList = new ArrayList<>();
//                        if(!line.equals("")){
//                            String[] ind = line.split("	");
//                            for(String temp : ind){
//                                indexList.add(Integer.valueOf(temp));
//                            }
//                        }else{
//                            indexList.add(0);
//                        }
//            			columnRow.add(indexList);
//            		}
//            		NaiveDFS dfs = new NaiveDFS(columnRow);
//                    dfs.run();
//
//                    if(dfs.findAns){
//                        System.out.println("find answer");
//                        print(dfs.answer);
//                    }else{
//                        System.out.println("running error");
//                    }
////            		break;
//            	}
//            }
//
////            //test parsing
////            for(int i=0; i<n; i++){
////            	for(int j=0; j<columnRow.get(i).size(); j++){
////            		System.out.print(columnRow.get(i).get(j) + " ");
////            	}
////            	System.out.println();
////            }
//
//            br.close();
//            fileReader.close();
//
//        } catch (FileNotFoundException e) {
//            e.printStackTrace();
//        } catch (IOException e) {
//            e.printStackTrace();
//        }




        //test Extension
        ArrayList<Integer> a = new ArrayList<>();

    }

    public static void print(ArrayList<List> ans){
        for(int i=0; i<ans.size(); i++){
            for(int j=0; j<ans.get(i).size(); j++){
                System.out.print((int) ans.get(i).get(j) + " ");
            }
            System.out.println();
        }
    }

}
