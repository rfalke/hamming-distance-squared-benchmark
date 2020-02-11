import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.file.Files;
import java.util.List;

public class FindSimilar {

    private int size;
    private int numData;
    private long data[];

    public static void main(String[] args) throws IOException {
        FindSimilar findSimilar = new FindSimilar();
        findSimilar.readData(args[0]);
        findSimilar.search(Integer.parseInt(args[1]));
    }

    private void readData(String filename) throws IOException {
        List<String> lines = Files.readAllLines(new File(filename).toPath());
        String[] parts = lines.get(0).split(" ");
        size = Integer.parseInt(parts[0]);
        numData = Integer.parseInt(parts[1]);
        int factor = size / 64;
        data = new long[numData * factor];
        for (int entryNo = 0; entryNo < numData; entryNo++) {
            String[] line = lines.get(entryNo + 1).split(" ");
            for (int i = 0; i < size / 64; i++) {
                data[entryNo * factor + i] = parsePart(line[0], i);
            }
        }
    }

    private long parsePart(String str, int partNo) {
        String substring = str.substring(partNo * 16, partNo * 16 + 16);
        BigInteger bigInteger = new BigInteger(substring, 16);
        return bigInteger.longValue();
    }

    private void search(int maxDist) {
        if (size == 64) {
            search64(maxDist);
        } else {
            search256(maxDist);
        }
    }

    private void search64(int maxDist) {
        for (int x = 0; x < numData; x++) {
            long to_compare = data[x];
            for (int y = x + 1; y < numData; y++) {
                int distance = Long.bitCount(to_compare ^ data[y]);

                if (distance <= maxDist) {
                    System.out.printf("%d %d %d\n", distance, x, y);
                }
            }
        }
    }

    private void search256(int maxDist) {
        for (int x = 0; x < numData; x++) {
            long to_compare0 = data[4 * x + 0];
            long to_compare1 = data[4 * x + 1];
            long to_compare2 = data[4 * x + 2];
            long to_compare3 = data[4 * x + 3];
            for (int y = x + 1; y < numData; y++) {
                int distance = (Long.bitCount(to_compare0 ^ data[4 * y + 0])
                        + Long.bitCount(to_compare1 ^ data[4 * y + 1])
                        + Long.bitCount(to_compare2 ^ data[4 * y + 2])
                        + Long.bitCount(to_compare3 ^ data[4 * y + 3]));

                if (distance <= maxDist) {
                    System.out.printf("%d %d %d\n", distance, x, y);
                }
            }
        }
    }
}