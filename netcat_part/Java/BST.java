class BST {
    static class Node {
        int key;
        Node left, right;
        public Node(int value){
            key=value;
            left=right = null;
        }
    }
    Node root;
    public BST(){
        root=null;
    }

    public void insert(int newValue) {
        insertRec(root, newValue);
    }

    private Node insertRec(Node root, int newValue){
        if(root == null){
            root = new Node(newValue);
            return root;
        }
        if(newValue < root.key) {
            insertRec(root.left, newValue);
        } else if (newValue > root.key) {
            insertRec(root.right, newValue);
        }
        // else newValue == root.key, but we aren't including dulicates
        return root;
    }

    public void inOrder() {
        inOrderRec(root);
    }

    private void inOrderRec(Node root) {
        if(root != null) {
            /*
                In here lies the order of the different types of tree 
                searches. pre action goes first; post action last; in order
                action in the middle
            */
            inOrderRec(root.left);
            inOrderRec(root.right);
        }
    }
}