class StringReverse {
    public static String reverseInPlace(String str) {
        StringBuilder sb = new StringBuilder(str);
        int length = sb.length();
        for(int i=0;i < length/2;i++) {
            final char current = sb.charAt(i);
            final int otherEnd = length - i - 1;
            sb.setCharAt(i, sb.charAt(otherEnd));
            sb.setCharAt(otherEnd, current);
        }
        return sb.toString();
    }

    public static String reverseInPlaceChar(String str) {
       char[] c1 = str.toCharArray();
       char[] c2 = new char[str.length()];

       for(int i=0;i<str.length();i++) {
           c2[i] = c1[str.length()-i-1];
       }
       return new String(c2);
    }


    public static void main(String[] args) {
        System.out.println(reverseInPlaceChar("paddy"));
    }
}

