import java.util.ArrayList;
import java.util.List;

public class Extension {
    public static ArrayList<Integer> method1_1(ArrayList<Integer> heads, ArrayList<Integer> input, int cNum){
//        ArrayList<Integer> leftest = new ArrayList<>();
//        ArrayList<Integer> rightest = new ArrayList<>();
//
//        leftest.addAll(heads);
//        rightest.addAll(heads);
        int range = (cNum - 1) - (heads.get(cNum-1) + input.get(cNum-1) - 1);
//        for(int i=0; i<rightest.size(); i++){
//            rightest.set(i, rightest.get(i) + range);
//        }

        ArrayList<Integer> heuristic = Tools.initWhiteRow(cNum);

        //hLen = length of black run heuristic, h = head of heuristic
        for(int i=0; i<heads.size(); i++){
            int hLen = input.get(i) - range;
            int h = heads.get(i) + range;
            for(int j=0; j<hLen; j++){
                heuristic.set(h+j-1, Tools.BLACK); //-1 because index from 0
            }
        }
        return heuristic;
    }



}
