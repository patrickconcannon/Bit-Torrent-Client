public class MinHeap {
    private int[] Heap;
    private int size;
    private int minSize;

    private static final int FRONT = 1; 

    public MinHeap(int minSize) {
        this.minSize = minSize;
        this.size = 0;
        Heap = new int[this.minSize+1];
        Heap[0] = Integer.MIN_VALUE; // MAX_VALUE
    }

    private int parent(int pos) {
        return pos / 2;
    }
    private int leftChild(int pos) {
        return 2 * pos;
    }
    private int rightChild(int pos) {
        return (2 * pos) + 1;
    }
    private boolean isLeaf(int pos) {
        // Check are in final row
        if ((pos >= size / 2) && pos <= size) { 
            return true;
        }
        return false;
    }
    private void swap(int firstPos, int secondPos) {
        int temp = Heap[firstPos];
        Heap[firstPos] = Heap[secondPos];
        Heap[secondPos] = temp;
    }
    private void minHeapify(int pos) {
        // Keep going until you are at the leaf
        if (isLeaf(pos)){ 
            return;
        }
        if (Heap[pos] > Heap[leftChild(pos)] || Heap[pos] > Heap[rightChild(pos)]) { // >
            // Check which is child is greater
            if (Heap[leftChild(pos)] < Heap[rightChild(pos)]) {
                swap(pos, leftChild(pos));
                minHeapify(leftChild(pos));
            } else {
                swap(pos, rightChild(pos));
                minHeapify(rightChild(pos));
            }
        }
    }
    public void insert(int element) {
        Heap[++size] = element;

        int current = size;
        while(Heap[current] < Heap[parent(current)]) { // >
            swap(current, parent(current));
            current = parent(current);
        }
    }
    public void print() { 
        for (int i = 1; i <= (size / 2); i++) { 
            System.out.print(" PARENT : " + Heap[i] + "\tLEFT CHILD : " + 
                      Heap[2 * i] + " RIGHT CHILD : " + Heap[2 * i + 1]); 
            System.out.println(); 
        } 
    }

    public int extractMin() { // For a min heap, you remove it and then replace the last element with it
        int popped = Heap[FRONT]; 
        // swap up leaf node to top
        Heap[FRONT] = Heap[size]; 
        // remove the leaf node
        Heap[size] = 0; 
        size--;

        minHeapify(FRONT); 
        return popped; 
    } 
    
    public int getSize() {
        return size;
    }

    public static void main(String[] arg) { 
        System.out.println("The min Heap is "); 
        MinHeap minHeap = new MinHeap(15); 
        minHeap.insert(5); 
        minHeap.insert(3); 
        minHeap.insert(17); 
        minHeap.insert(10); 
        minHeap.insert(84);     
        minHeap.insert(19); 
        minHeap.insert(6); 
        minHeap.insert(22); 
        minHeap.insert(9); 
    
        minHeap.print(); 
        System.out.println("Size: " + minHeap.getSize());
        System.out.println("The min val is " + minHeap.extractMin());
        minHeap.print(); 
        System.out.println("Size: " + minHeap.getSize());
    }  
}