import java.util.ArrayList;
import java.util.List;

public class NaiveDFS {
	
	public ArrayList<List> inputs;
	public int cNum;
	public final int BLACK = 1;
	public final int WHITE = 0;
	public ArrayList<List> tempAns;
	public ArrayList<List> answer;
	
	public NaiveDFS(ArrayList<List> inputs){
		this.inputs = inputs;
		cNum = inputs.size()/2;
		answer = new ArrayList<>();
	}
	
	public void run(){
		int nowInput = 0;
		ArrayList<List> ans = new ArrayList<>();
		//
		for(int row=cNum; row<2*cNum; row++){
			for(int i=0; i<inputs.get(row).size(); i++){
				nowInput = (int) inputs.get(row).get(i);
				ArrayList<Integer> ansRow = new ArrayList<>();
				
			}
			
		}
	}

	public ArrayList<Integer> findTempAns(int from, int to, int len, ArrayList<Integer> rowHeads, int rowNum){
		if(to < from) return null;
		if(len ==0) return null;
		if(to == from){
			if(len != 1){
				return null;
			}else{  //complete one row, do partial rowTest
				rowHeads.add(from);
				return rowHeads;
			}
		}

		//find rowNum th row
		for(int i = from+1; i<= to-len+2; i++){
			ArrayList<Integer> ans = new ArrayList<>();
			ans.addAll(rowHeads);
			ans.add(i-1);
			findTempAns(i, to, len-1, ans, rowNum);

		}



		return null;
	}

	public boolean checkRowValid(ArrayList<Integer> rowHeads, ArrayList<Integer> spec){



		return true;
	}
	
	public boolean checkCol(ArrayList<List> ans, boolean partial){
		int nowInput = 0;
		int contiBlack = 0;
		//check all col, i = col
		for(int i=0; i<cNum; i++){
			//k = row
			int k=0;
			//each col, check ans valid, j = input index
			for(int j=0; j<cNum; j++){
				nowInput = (int) inputs.get(i).get(j);
				//iterate all elements in the col ,k = row
				for(; k<cNum; k++){
					int color = (int) ans.get(k).get(i);
					if(color == BLACK){
						contiBlack++;
						if(contiBlack > nowInput) return false;
					}else{  //when meet white, the previous contiBlack should equal 
						if(contiBlack == nowInput){ //find next input
							break;
						}
						//if not partial, contiBlack should not less than input.
						if(!partial){
							if(contiBlack != 0){
								if(contiBlack < nowInput){
									return false;
								}
							}
						}
						contiBlack = 0;
					}
				}
			}
		}
		
		//check last contiBlack if not partial check
		if(!partial){
			if(contiBlack < nowInput){ //find next input
				return false;
			}
		}
		return true;
	}
}
