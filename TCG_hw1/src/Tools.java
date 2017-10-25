import java.util.ArrayList;
import java.util.List;

public class Tools {

    public static final int BLACK = 1;
    public static final int WHITE = 0;

    public static ArrayList<List> initRawHeadsAns(int cNum, ArrayList<List> inputs) {
        ArrayList<List> ans = new ArrayList<>();
        for(int row=0; row<cNum; row++){
            ArrayList<Integer> inputRow = (ArrayList<Integer>) inputs.get(row+cNum);
            ArrayList<Integer> rowHeads = new ArrayList<>();
            if(inputRow.get(0) != 0){
                rowHeads.add(1);
                for(int i=1; i<inputRow.size(); i++){
                    rowHeads.add(rowHeads.get(i-1) + inputRow.get(i-1) + 1);
                }
                ans.add(rowHeads);
            }else{  //consider input row = 0
                rowHeads.add(0);
                ans.add(rowHeads);
            }

        }
        return ans;
    }

    public static ArrayList<Integer> initWhiteRow(int cNum){
        ArrayList<Integer> row = new ArrayList<>();
        for(int j=0; j<cNum; j++){
            row.add(WHITE);
        }
        return row;
    }

    public static ArrayList<List> initWhiteAnswer(int cNum){
        ArrayList<List> answer = new ArrayList<>();
        for(int i=0; i<cNum; i++){
            ArrayList<Integer> row = new ArrayList<>();
            for(int j=0; j<cNum; j++){
                row.add(WHITE);
            }
            answer.add(row);
        }
        return answer;
    }

    public static void arrangeRowHeads(int moveDigit, ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
//		System.out.println("arrange row");
        if(canMove(moveDigit, rowHeads, inputRow)){
            rowHeads.set(moveDigit, rowHeads.get(moveDigit)+1);
            for(int i= moveDigit; i<rowHeads.size()-1; i++){
                rowHeads.set(i+1, rowHeads.get(i) + inputRow.get(i) + 1);
            }
        }
    }

    //step1: check if rowHeads(0) = 1
    //			true=>
    //check if last digit(after move) < len
    public static boolean canMove(int moveDigit,  ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
        if(moveDigit == rowHeads.size()-1){
            if(rowHeads.get(0) == 1){
//				System.out.println("cannot move");
                return false;
            }else{
                return true;
            }
        }
        if(rowHeads.get(moveDigit) < rowHeads.get(moveDigit+1) - inputRow.get(moveDigit) -1){
            return true;
        }
//		System.out.println("cannot move");
        return false;
    }

    //rowHeads to answer type
    public static ArrayList<Integer> colorRow(ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow, int cNum){
        ArrayList<Integer> rowAns = new ArrayList<>();
        ArrayList<Integer> rowColor = new ArrayList<>();
        //extend rowHeads
        for(int i=0; i<rowHeads.size(); i++){
            for(int j=0; j<inputRow.get(i); j++){
                rowAns.add(rowHeads.get(i)+j-1);  //-1 because index from 0
//				System.out.print(rowHeads.get(i)+j-1 + " ");
            }
        }
        //make white row
        for(int i=0; i<cNum; i++){
            rowColor.add(Tools.WHITE);
        }
        //if rowHeads = 0 , return all white
        if(rowHeads.get(0) == 0){
            return rowColor;
        }

        //draw
        for(int i=0; i<rowAns.size(); i++){
            rowColor.set(rowAns.get(i), Tools.BLACK);
        }
        return rowColor;
    }

}
