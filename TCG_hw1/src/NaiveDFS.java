import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class NaiveDFS {
	
	public ArrayList<List> inputs;
	public int cNum;
	public final int BLACK = 1;
	public final int WHITE = 0;
	public int[] last;  //last row heads last position
	public int[] len;  //row input's size
	public ArrayList<List> answer;
	public ArrayList<List> answerDraw;
	public boolean findAns = false;
	
	public NaiveDFS(ArrayList<List> inputs){
		this.inputs = inputs;
		cNum = inputs.size()/2;
		answer = new ArrayList<>();
		last = new int[cNum];
		len = new int[cNum];
		for(int i=0; i<cNum; i++){
			len[i] = inputs.get(i+cNum).size();
			last[i] = cNum - (int) inputs.get(i+cNum).get(len[i]-1) + 1;
		}
	}

	public void run(){
		doSearch(0, initRawHeadsAns());

		if(findAns){
			answerDraw = new ArrayList<>();
			for(int i=0; i<cNum; i++){
				answerDraw.add(colorRow((ArrayList<Integer>) answer.get(i), (ArrayList<Integer>) inputs.get(i+cNum)));
			}
		}
	}
	
	public void doSearch(int rowNum, ArrayList<List> ans){
		if(rowNum >= cNum) return;
		if(findAns) return;
		ArrayList<List> localAns = new ArrayList<>();
		localAns.addAll(ans);
		//get this row
		ArrayList<Integer> thisRow = (ArrayList<Integer>) localAns.get(rowNum);
		//move to next row
		while(getNextRow(rowNum, thisRow, (ArrayList<Integer>) inputs.get(rowNum+cNum))){
			localAns.set(rowNum, thisRow);
			doSearch(rowNum+1, localAns);
			if(findAns) return;
		}
		//DFS: do this layer after lower layer has been processed
		if(checkCol(ans)){
			answer = ans;
			findAns = true;
		}
	}

	public boolean getNextRow(int rowNum, ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
		//can move all digit right 1 unit
		if(rowHeads.get(len[rowNum]-1) < last[rowNum]){
			for(int i=0; i<len[rowNum]; i++){
				rowHeads.set(i, rowHeads.get(i)+1);
			}
		}else {
			//move next digit
			for(int moveDigit = len[rowNum]-2; moveDigit >= 0; moveDigit--){
				//Cannot move, all possible row has been iterated.
				if(moveDigit == 0){
					return false;
				}
				if( canMove(moveDigit, rowHeads, inputRow)){
					//move right moveDigit 1, and arrange backward digit
					arrangeRowHeads(moveDigit, rowHeads, inputRow);
					break;
				}
			}
		}
		return true;
	}

	public ArrayList<List> initRawHeadsAns() {
		ArrayList<List> ans = new ArrayList<>();
		for(int row=0; row<cNum; row++){
			ArrayList<Integer> inputRow = (ArrayList<Integer>) inputs.get(row+cNum);
			ArrayList<Integer> rowHeads = new ArrayList<>();
			rowHeads.add(1);
			for(int i=1; i<inputRow.size(); i++){
				rowHeads.add(rowHeads.get(i-1) + inputRow.get(i-1) + 1);
			}
			ans.add(rowHeads);
		}
		return ans;
	}

	public ArrayList<List> initWhiteAnswer(){
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

	//
	public void arrangeRowHeads(int moveDigit, ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
		if(canMove(moveDigit, rowHeads, inputRow)){
			rowHeads.set(moveDigit, rowHeads.get(moveDigit)+1);
			for(int i= moveDigit; i<rowHeads.size()-1; i++){
				rowHeads.set(i+1, rowHeads.get(i) + inputRow.get(i) + 1);
			}
		}
	}

	//check if last digit(after move) < len
	public boolean canMove(int moveDigit,  ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
		if(rowHeads.get(moveDigit) < rowHeads.get(moveDigit+1) - inputRow.get(moveDigit) -1){
			return true;
		}
		return false;
	}

//	//check if last digit(after move) < len
//	public boolean canMove(int moveDigit, int rowNum, ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
//		int spaceNum = len[rowNum] - moveDigit - 1;
//		int sum = 0;
//		for(int i=moveDigit; i<len[rowNum]-1; i++){
//			sum += inputRow.get(i);
//		}
//		if(rowHeads.get(moveDigit)+1 + spaceNum + sum < last[rowNum]){
//			return true;
//		}
//		return false;
//	}

	//rowHeads to answer type
	public ArrayList<Integer> colorRow(ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
		ArrayList<Integer> rowAns = new ArrayList<>();
		ArrayList<Integer> rowColor = new ArrayList<>();
		//extend rowHeads
		for(int i=0; i<rowHeads.size(); i++){
			for(int j=0; j<inputRow.get(i); j++){
				rowAns.add(rowHeads.get(i)+j);
			}
		}
		//make white row
		for(int i=0; i<cNum; i++){
			rowColor.add(WHITE);
		}
		//draw
		for(int i=0; i<rowAns.size(); i++){
			rowColor.set(rowAns.get(i), BLACK);
		}
		return rowColor;
	}
	
	public boolean checkCol(ArrayList<List> ans){
		int nowInput = 0;
		int contiBlack = 0;
		//check all col, i = col
		for(int i=0; i<cNum; i++){
			//k = row
			int k=0;
			//each col, check ans valid, j = input index
			for(int j=0; j<inputs.get(i).size(); j++){
				nowInput = (int) inputs.get(i).get(j);
				//iterate all elements in the col ,k = row
				for(; k<cNum; k++){
					int color = (int) ans.get(k).get(i);
					if(color == BLACK){
						contiBlack++;
						if(contiBlack > nowInput) return false;
					}else{  //when meet white, the previous contiBlack should equal 
						if(contiBlack == nowInput){ //find next input
							k++;
							break;
						}
						//contiBlack should not less than input.
						if(contiBlack != 0){
							if(contiBlack < nowInput){
								return false;
							}
						}
						contiBlack = 0;
					}
				}
			}
		}
		
		//check last contiBlack
		if(contiBlack < nowInput){ //find next input
			return false;
		}
		return true;
	}
}
