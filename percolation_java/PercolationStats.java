import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats
{
    private final int n;
    private final int t;
    private double x;
    private double s;
    private double d1;
    private double d2;

    public PercolationStats(int n, int trials)
    {
        this.n = n;
        t = trials;

        conductTest();
    }

    private void conductTest()
    {
        double fractionSum = 0;
        double[] allFractions = new double[t];

        for (int i = 0; i < t; ++i)
        {
            Percolation p = new Percolation(n);

            while (!p.percolates())
            {
                int row = StdRandom.uniformInt(1, n + 1);
                int col = StdRandom.uniformInt(1, n + 1);

                if (!p.isOpen(row, col))
                {
                    p.open(row, col);
                }
            }

            int openSites = p.numberOfOpenSites();
            double doubleN = (double) (n * n);
            double f = (double) openSites / doubleN;
            fractionSum += f;
            allFractions[i] = f;
        }

        x = StdStats.mean(allFractions);
        s = StdStats.stddev(allFractions);

        double ts = Math.sqrt(t);
        double tmp = ((1.96 * s) / ts);
        d1 = x - tmp;
        d2 = x + tmp;
    }

    public double mean()
    {
        return x;
    }

    public double stddev()
    {
        return s;
    }

    public double confidenceLo()
    {
        return d1;
    }

    public double confidenceHi()
    {
        return d2;
    }

    public static void main(String[] args)
    {
        if (args.length != 2)
            throw new IllegalArgumentException();

        int n = Integer.parseInt(args[0]);
        int t = Integer.parseInt(args[1]);

        if (n <= 0 || t <= 0)
            throw new IllegalArgumentException();

        PercolationStats ps = new PercolationStats(n, t);

        System.out.println("mean                    = " + ps.mean());
        System.out.println("stddev                  = " + ps.stddev());
        System.out.println("95% confidence interval = [" + ps.confidenceLo() + ", " + ps.confidenceHi() + "]");
    }
}
