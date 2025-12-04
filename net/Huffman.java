import java.util.*;

// Node class for Huffman Tree
class HuffmanNode {
    int data;       // frequency
    char c;         // character
    HuffmanNode left;
    HuffmanNode right;
}

// Comparator to order nodes by frequency
class MyComparator implements Comparator<HuffmanNode> {
    public int compare(HuffmanNode x, HuffmanNode y) {
        return x.data - y.data;
    }
}

public class Huffman {

    // Recursive function to generate and print Huffman codes
    public static void printCode(HuffmanNode root, String s, Map<Character, String> huffmanCode) {
        // Base case: leaf node
        if (root.left == null && root.right == null) {
            huffmanCode.put(root.c, s);
            System.out.println(root.c + " : " + s);
            return;
        }

        // Traverse left and right subtrees
        printCode(root.left, s + "0", huffmanCode);
        printCode(root.right, s + "1", huffmanCode);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter a string to encode using Huffman Coding:");
        String text = sc.nextLine();
        sc.close();

        // Step 1: Count frequency of each character
        Map<Character, Integer> freq = new HashMap<>();
        for (char c : text.toCharArray()) {
            freq.put(c, freq.getOrDefault(c, 0) + 1);
        }

        // Step 2: Create a priority queue (min-heap)
        PriorityQueue<HuffmanNode> q = new PriorityQueue<>(new MyComparator());

        // Step 3: Create a leaf node for each character
        for (Map.Entry<Character, Integer> entry : freq.entrySet()) {
            HuffmanNode hn = new HuffmanNode();
            hn.c = entry.getKey();
            hn.data = entry.getValue();
            hn.left = null;
            hn.right = null;
            q.add(hn);
        }

        // Step 4: Build the Huffman tree
        HuffmanNode root = null;
        while (q.size() > 1) {
            HuffmanNode x = q.poll();
            HuffmanNode y = q.poll();

            HuffmanNode f = new HuffmanNode();
            f.data = x.data + y.data;
            f.c = '-';
            f.left = x;
            f.right = y;
            root = f;
            q.add(f);
        }

        // Step 5: Print Huffman codes
        System.out.println("\nHuffman Codes for characters:");
        Map<Character, String> huffmanCode = new HashMap<>();
        printCode(root, "", huffmanCode);

        // Step 6: Encode the input string
        StringBuilder encoded = new StringBuilder();
        for (char c : text.toCharArray()) {
            encoded.append(huffmanCode.get(c));
        }

        System.out.println("\nEncoded String: " + encoded);

        // Step 7: Decode the encoded string
        System.out.println("\nDecoding the encoded string...");
        StringBuilder decoded = new StringBuilder();
        HuffmanNode current = root;
        for (int i = 0; i < encoded.length(); i++) {
            current = (encoded.charAt(i) == '0') ? current.left : current.right;
            if (current.left == null && current.right == null) {
                decoded.append(current.c);
                current = root;
            }
        }

        System.out.println("Decoded String: " + decoded);
    }
}
