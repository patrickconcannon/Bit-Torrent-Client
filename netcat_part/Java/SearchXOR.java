import java.util.ArrayList;
import java.util.List;
import java.util.Collections;

class SearchXOR {
    static int findMissingNumber(int[] arr) {
        List<Integer> values = new ArrayList<Integer>();
        for(int i : arr){
            values.add(Integer.valueOf(i));
        }
        Collections.sort(values);
        // To print the binary value of the int - Integer.toBinaryString(x)
        int xorValue=0;
        int minValue = values.get(0);
        for(int i=0;i<arr.length;i++, minValue++) {
            xorValue ^= minValue^arr[i];
        }
        return xorValue^minValue;
    }

    public static void main(String[] args){
        int[] arr = {11, 12,13,14,16};
        System.out.println(findMissingNumber(arr));
    }
}