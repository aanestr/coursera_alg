import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation
{
    private boolean [][] opened;
    private WeightedQuickUnionUF qu;
    private int nn;
    private int openedAmount = 0;

    public Percolation(int n)
    {
        if (n <= 0)
            throw new IllegalArgumentException();

        qu = new WeightedQuickUnionUF(n*n + 2);
        opened = new boolean[n][n];
        nn = n;
    }

    private int calculateIndex(int row, int col)
    {
        return row * nn + col + 1;
    }

    private void checkIndex(int r, int c)
    {
        if (r < 0 || c < 0)
            throw new IllegalArgumentException();

        if (r >= nn || c >= nn)
            throw new IllegalArgumentException();
    }

    public void open(int row, int col)
    {
        row -= 1;
        col -= 1;
        checkIndex(row, col);

        if (opened[row][col])
            return;

        opened[row][col] = true;
        openedAmount += 1;

        int v1 = calculateIndex(row, col);

        if (col != 0 && opened[row][col - 1])
        {
            int v2 = calculateIndex(row, col - 1);
            qu.union(v1, v2);
        }

        if ((col != nn -1) && opened[row][col + 1])
        {
            int v2 = calculateIndex(row, col + 1);
            qu.union(v1, v2);
        }

        if (row != 0 && opened[row - 1][col])
        {
            int v2 = calculateIndex(row - 1, col);
            qu.union(v1, v2);
        }

        if ((row != nn - 1) && opened[row + 1][col])
        {
            int v2 = calculateIndex(row + 1, col);
            qu.union(v1, v2);
        }

        if (row == 0)
            qu.union(v1, 0);
        if (row == nn - 1)
            qu.union(v1, nn * nn + 1);
    }

    public boolean isOpen(int row, int col)
    {
        row -= 1;
        col -= 1;
        checkIndex(row, col);

        return opened[row][col];
    }

    public boolean isFull(int row, int col)
    {
        if (!isOpen(row, col))
            return false;

        row -= 1;
        col -= 1;

        int index = calculateIndex(row, col);

        int max0 = qu.find(0);
        int maxByIndex = qu.find(index);

        return maxByIndex == max0;
    }

    public int numberOfOpenSites()
    {
        return openedAmount;
    }

    public boolean percolates()
    {
        return qu.find(0) == qu.find(nn * nn + 1);
    }
}