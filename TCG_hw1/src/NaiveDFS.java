import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class NaiveDFS {
	
	public ArrayList<List> inputs;
	public int cNum;
	public int[] last;  //last row heads last position
	public int[] len;  //row input's size
	public ArrayList<List> answer;
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
		doSearch(0, Tools.initRawHeadsAns(cNum, inputs));
	}
	
	public boolean doSearch(int rowNum, ArrayList<List> ans){
		if(rowNum >= cNum) return true;
		if(findAns) return false;
		ArrayList<List> localAns = new ArrayList<>();
		//clone ans
		for(int i=0; i<cNum; i++){
			ArrayList<Integer> temp = (ArrayList<Integer>) ans.get(i);
			localAns.add((ArrayList<Integer>) temp.clone());
		}
		//get this row
		ArrayList<Integer> thisRow = (ArrayList<Integer>) localAns.get(rowNum);



		//try to transform row
		do{
			//Find deeper layer
//			System.out.println("Process rowNum: " + rowNum);
			localAns.set(rowNum, thisRow);
			boolean checkFlag = doSearch(rowNum+1, localAns);
			if(findAns) return false;

			//DFS: do this layer after lower layer has been processed
			if(checkFlag){
				ArrayList<List> answerDraw = new ArrayList<>();
				for(int i=0; i<cNum; i++){
					answerDraw.add(Tools.colorRow((ArrayList<Integer>) localAns.get(i), (ArrayList<Integer>) inputs.get(i+cNum), cNum));
				}
//				Main.print(answerDraw);
				if(checkCol(answerDraw)){
					answer = answerDraw;
					findAns = true;
				}
			}
		}while(getNextRow(rowNum, thisRow, (ArrayList<Integer>) inputs.get(rowNum+cNum)));

		return false;
	}

	public boolean getNextRow(int rowNum, ArrayList<Integer> rowHeads, ArrayList<Integer> inputRow){
		//consider rowHeads = 0
		if(rowHeads.get(0) == 0){
			return false;
		}
		//can move all digit right 1 unit
		if(rowHeads.get(len[rowNum]-1) < last[rowNum]){
			for(int i=0; i<len[rowNum]; i++){
				rowHeads.set(i, rowHeads.get(i)+1);
			}
		}else {
			if(len[rowNum]-2 < 0) return false;
			//move next digit
			for(int moveDigit = len[rowNum]-1; moveDigit >= 0; moveDigit--){
				//Cannot move, all possible row has been iterated.
				if(moveDigit == 0){
					return false;
				}
				if( Tools.canMove(moveDigit, rowHeads, inputRow)){
					//move right moveDigit 1,
					int range = rowHeads.get(0) - 1;
					for(int i=0; i<len[rowNum]; i++){
						rowHeads.set(i, rowHeads.get(i) - range);
					}
					rowHeads.set(moveDigit, rowHeads.get(moveDigit) + 1);
					// and arrange backward digit
					Tools.arrangeRowHeads(moveDigit, rowHeads, inputRow);
					break;
				}
			}
		}
		return true;
	}

	//

	
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
				if(nowInput == 0){
					break;
				}
				//iterate all elements in the col ,k = row
				for(; k<cNum; k++){
					int color = (int) ans.get(k).get(i);
					if(color == Tools.BLACK){
						contiBlack++;
						if(contiBlack > nowInput) return false;
					}else{  //when meet white, the previous contiBlack should equal
						if(contiBlack == nowInput){ //find next input
							k++;
							contiBlack = 0;
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
				//check last contiBlack
				if(contiBlack == nowInput){ //find next input
					contiBlack = 0;
				}
				if(contiBlack != 0){
					if(contiBlack < nowInput){
						return false;
					}
				}
				contiBlack = 0;
			}
			//after satisfy input, rest unit of the col should be white.
			for(; k<cNum; k++){
				int color = (int) ans.get(k).get(i);
				if(color == Tools.BLACK){
					return false;
				}
			}

		}


		return true;
	}
}
