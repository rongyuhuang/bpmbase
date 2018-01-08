#ifndef MEMPIECE_H
#define MEMPIECE_H

#include <vector>

class DoubleVector {
public:
    int len;
    const double* data;
    DoubleVector()
        : len(0)
        , data(0)
    {
    }
    DoubleVector(const double* _data, int _len)
        : len(_len)
        , data(_data)
    {
    }
    double operator[](int i) const { return data[i]; }
    DoubleVector rightPiece(int count, int shift = 0)
    {
        return DoubleVector(&data[len - count - shift], count);
    }
    double lastVal(int rightPos = 0) { return data[len - 1 - rightPos]; }
    static DoubleVector from(const std::vector<double>& b)
    {
        return DoubleVector(b.data(), b.size());
    }
};

#endif // MEMPIECE_H
