import java.util.*;

public class HammingCode {

    // Function to calculate parity during encoding
    static int calcParity(int[] bits, int pos) {
        int parity = 0;
        for (int i = 1; i < bits.length; i++) {
            if ((i & pos) != 0) {
                parity ^= bits[i];
            }
        }
        return parity;
    }

    // Encode data with Hamming Code
    static int[] encode(String data) {
        int m = data.length();
        int r = 0;

        // Find number of parity bits required
        while (Math.pow(2, r) < (m + r + 1)) {
            r++;
        }

        int n = m + r;
        int[] bits = new int[n + 1]; // 1-based indexing
        int j = 0;

        // Place data bits
        for (int i = 1; i <= n; i++) {
            if ((i & (i - 1)) == 0) continue; // skip parity positions
            bits[i] = data.charAt(j++) - '0';
        }

        // Calculate parity bits
        for (int i = 0; i < r; i++) {
            int pos = 1 << i;
            bits[pos] = calcParity(bits, pos);
        }

        return bits;
    }

    // Detect and correct single-bit error
    static int[] detectAndCorrect(int[] bits) {
        int n = bits.length - 1;
        int errorPos = 0;
        int i = 0;

        while ((1 << i) <= n) {
            int pos = 1 << i;
            int parity = 0;
            for (int j = 1; j <= n; j++) {
                if ((j & pos) != 0) {
                    parity ^= bits[j]; // include all covered bits
                }
            }   
            if (parity != 0) { // parity mismatch
                errorPos += pos;
            }
            i++;
        }

        if (errorPos != 0) {
            System.out.println("Error detected at position: " + errorPos);
            bits[errorPos] ^= 1; // Correct the error
            System.out.println("Error corrected!");
        } else {
            System.out.println("No error detected.");
        }

        return bits;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter data bits (e.g. 1011): ");
        String data = sc.nextLine();

        // Encode
        int[] encoded = encode(data);
        System.out.print("Encoded data with Hamming Code: ");
        for (int i = 1; i < encoded.length; i++) System.out.print(encoded[i]);
        System.out.println();

        // Simulate error
        int[] received = Arrays.copyOf(encoded, encoded.length);
        System.out.print("Enter error position to flip (0 for none): ");
        int pos = sc.nextInt();
        if (pos != 0 && pos < received.length) {
            received[pos] ^= 1;
        }

        System.out.print("Received data: ");
        for (int i = 1; i < received.length; i++) System.out.print(received[i]);
        System.out.println();

        // Detect and correct
        int[] corrected = detectAndCorrect(received);
        System.out.print("Corrected data: ");
        for (int i = 1; i < corrected.length; i++) System.out.print(corrected[i]);
        System.out.println();

        sc.close();
    }
}
