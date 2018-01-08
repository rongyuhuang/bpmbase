#ifndef ARRAYMATH_H
#define ARRAYMATH_H

#include "mempiece.h"

namespace Bpm {
static const double NoValue = 1.7e308;
}

class VectorMath {
public:
    VectorMath() = delete;
    VectorMath(DoubleVector a);
    static VectorMath* create(DoubleVector a);
    VectorMath(const VectorMath& rhs);
    VectorMath& operator=(const VectorMath& rhs);
    ~VectorMath();
    void destroy();

    operator DoubleVector() const;
    DoubleVector result() const;

    double max() const;
    double maxValue() const;
    int maxIndex() const;
    double min() const;
    double minValue() const;
    int minIndex() const;
    double sum() const;
    double avg() const;
    double var(int w = 0) const;
    double cov(DoubleVector b) const;
    double corr(DoubleVector b) const;
    double stdDev() const;
    double stdDevp() const;
    double med() const;
    double percentile(double p) const;
    double slope(DoubleVector y = DoubleVector(),
        double* intercept = nullptr) const;

    VectorMath& add(DoubleVector b);
    VectorMath& add(double b);
    VectorMath& sub(DoubleVector b);
    VectorMath& sub(double b);
    VectorMath& mul(DoubleVector b);
    VectorMath& mul(double b);
    VectorMath& div(DoubleVector b);
    VectorMath& div(double b);
    VectorMath& financeDiv(DoubleVector b, double zeroByZeroValue);
    VectorMath& shift(int offset = 1, double fillValue = Bpm::NoValue);
    VectorMath& delta(int offset = 1);
    VectorMath& rate(int offset = 1);
    VectorMath& abs();
    VectorMath& acc();

    VectorMath& trim(int startIndex = 0, int len = -1);
    VectorMath& insert(DoubleVector a, int insertPoint = -1);
    VectorMath& insert(double c, int len, int insertPoint = -1);
    VectorMath& replace(double a, double b);

    VectorMath& movAvg(int interval);
    VectorMath& expAvg(double smoothingFactor);
    VectorMath& movMed(int interval);
    VectorMath& movPercentile(int interval, double p);
    VectorMath& movMax(int interval);
    VectorMath& movMin(int interval);
    VectorMath& movStdDev(int interval);
    VectorMath& movCorr(int interval, DoubleVector b = DoubleVector());
    VectorMath& movSlope(int interval);
    VectorMath& lowess(double smoothness = 0.25, int iteration = 0);
    VectorMath& lowess(DoubleVector b,
        double smoothness = 0.25,
        int iteration = 0);

    VectorMath& selectGTZ(DoubleVector decisionArray = DoubleVector(),
        double fillValue = 0);
    VectorMath& selectGEZ(DoubleVector decisionArray = DoubleVector(),
        double fillValue = 0);
    VectorMath& selectLTZ(DoubleVector decisionArray = DoubleVector(),
        double fillValue = 0);
    VectorMath& selectLEZ(DoubleVector decisionArray = DoubleVector(),
        double fillValue = 0);
    VectorMath& selectEQZ(DoubleVector decisionArray = DoubleVector(),
        double fillValue = 0);
    VectorMath& selectNEZ(DoubleVector decisionArray = DoubleVector(),
        double fillValue = 0);

private:
    void init(DoubleVector a);
    void fini();

private:
    std::vector<double> data_;
};

#endif // ARRAYMATH_H
