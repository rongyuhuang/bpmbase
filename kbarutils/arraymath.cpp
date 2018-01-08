#include "arraymath.h"
#include "lowess.h"
#include "utils/logging.h"

#include <algorithm>
#include <numeric>

//////

VectorMath::VectorMath(DoubleVector a)
{
    init(a);
}

// static
VectorMath* VectorMath::create(DoubleVector a)
{
    return new VectorMath(a);
}

VectorMath::VectorMath(const VectorMath& rhs)
{
    DoubleVector a = rhs;
    init(a);
}

VectorMath& VectorMath::operator=(const VectorMath& rhs)
{
    fini();
    DoubleVector a = rhs;
    init(a);

    return *this;
}

VectorMath::~VectorMath()
{
    fini();
}

void VectorMath::destroy()
{
    delete this;
}

//////

void VectorMath::init(DoubleVector a)
{
    data_.resize(a.len, 0);
    memcpy(data_.data(), a.data, sizeof(double) * a.len);
}

void VectorMath::fini()
{
    data_.clear();
}

//////

VectorMath::operator DoubleVector() const
{
    return result();
}

DoubleVector VectorMath::result() const
{
    return DoubleVector(data_.data(), data_.size());
}

//////

//
// TODO(hege):如果数组里面有NaN，在numpy里面，
// max min sum avg var stddev med percentile都为NaN
// 这增加了无谓的复杂性，应该在历史数据提供和实时数据提供那里
// 做数据清洗，保证数据的正常
// 信号系统和执行系统，假设数据都是对的，最多在加载数据和接收
// 实时数据那里，做个check，不通过就skip，在arraymath里面
// 就不做这种无谓的事情了
//
double VectorMath::max() const
{
    return maxValue();
}

double VectorMath::maxValue() const
{
    auto result = std::max_element(data_.begin(), data_.end());
    return *result;
}

int VectorMath::maxIndex() const
{
    auto result = std::max_element(data_.begin(), data_.end());
    return std::distance(data_.begin(), result);
}

double VectorMath::min() const
{
    return minValue();
}

double VectorMath::minValue() const
{
    auto result = std::min_element(data_.begin(), data_.end());
    return *result;
}

int VectorMath::minIndex() const
{
    auto result = std::min_element(data_.begin(), data_.end());
    return std::distance(data_.begin(), result);
}

// 和 sum
// numpy.sum
double VectorMath::sum() const
{
    double result = 0.0;
    for (int i = 0; i < data_.size(); i++) {
        // if(data_[i]==Bpm::NoValue){
        //    return Bpm::NoValue;
        //}
        result += data_[i];
    }
    return result;
}

// 均值/期望值/E/expections/mean : avg
// avg = 算术平均
// numpy.mean
double VectorMath::avg() const
{
    CHECK(data_.size() > 0);
    // double s = sum();
    // if(s==Bpm::NoValue){
    //    return s;
    //}
    return sum() / data_.size();
}

// 方差/var/DX:variances
// 实际值期望值的差的平方的和的均值，展开后就是实际值的平方的均值 - 期望值的平方
// D(X)=E(X²)-E²(X)
// W可以取0或1，取0求样本方差的无偏估计值（除以N-1)；对应取1求得的是方差（除以N）
// numpy.var
double VectorMath::var(int w) const
{
    CHECK(data_.size() + w > 1);
    double mean = avg();
    double dx = 0.0;
    for (int i = 0; i < data_.size(); i++) {
        dx += (data_[i] - mean) * (data_[i] - mean);
    }

    return dx / (data_.size() - 1 + w);
}

// 标准差=方差的算术平方根
// StdDev,样本标准差^2=（样本方差/（数据个数-1））
// StdDevp,总体标准差^2=（总体方差/（数据个数））
// 样本标准差
// numpy.std
double VectorMath::stdDev() const
{
    CHECK(data_.size() > 1);
    double dx = var();
    double stddev = std::sqrt(dx);
    return stddev;
}

// 总体标准差
double VectorMath::stdDevp() const
{
    CHECK(data_.size() >= 1);
    double dx = var(1);
    double stddev = std::sqrt(dx);
    return stddev;
}

// 协方差：covariances
// Cov(X,Y)=E(XY)-E(X)E(Y)
// numpy.cov
double VectorMath::cov(DoubleVector b) const
{
    CHECK(b.len == data_.size());
    double ex = avg();
    double ey = VectorMath(b).avg();
    double exy = VectorMath(b).mul(result()).avg();
    return exy - ex * ey;
}

// 例子：
// Xi 1.1 1.9 3
// Yi 5.0 10.4 14.6
// E(X) = (1.1+1.9+3)/3=2
// E(Y) = (5.0+10.4+14.6)/3=10
// E(XY)=(1.1×5.0+1.9×10.4+3×14.6)/3=23.02
// Cov(X,Y)=E(XY)-E(X)E(Y)=23.02-2×10=3.02
// D(X)=E(X²)-E²(X)=(1.1²+1.9²+3²)/3 - 4=4.60-4=0.6 σx=0.77
// D(Y)=E(Y²)-E²(Y)=(5²+10.4²+14.6²)/3-100=15.44 σy=3.93
// r(X,Y)=Cov(X,Y)/(σxσy)=3.02/(0.77×3.93) = 0.9979

// 相关系数：correlation
// r(X,Y)=Cov(X,Y)/(σxσy)
// numpy.correlate
double VectorMath::corr(DoubleVector b) const
{
    CHECK(b.len == data_.size());
    double dx = var(1);
    double dy = VectorMath(b).var(1);
    return cov(b) / (std::sqrt(dx) * std::sqrt(dy));
}

// 中值/中位数 median
// 排好序后中间的那个值，如果为偶数就是中间两个数的平均值
// numpy.median
double VectorMath::med() const
{
    // if(std::find(data_.begin(),data_.end(),Bpm::NoValue)!=data_.end()){
    //    return Bpm::NoValue;
    //}

    CHECK(data_.size() > 0);
    std::vector<double> a = data_;
    std::sort(a.begin(), a.end());

    int first, second;
    if (a.size() % 2 != 0) {
        first = a.size() / 2;
        second = a.size() / 2;
    }
    else {
        first = a.size() / 2 - 1;
        second = a.size() / 2;
    }

    return (a[first] + a[second]) / 2;
}

// 百分位数 percentile
// numpy.percenttile
double VectorMath::percentile(double p) const
{
    CHECK(p >= 0 && p <= 100);

    std::vector<double> a = data_;
    std::sort(a.begin(), a.end());

    // 把数据分段，看p落在哪两个点之间
    // n个点，分成n-1段
    double sectionIndex = (a.size() - 1) * p / 100.0;
    int first = floor(sectionIndex);
    int second = ceil(sectionIndex);
    double firstVal = a[first];
    double secondVal = a[second];
    double val = firstVal + (secondVal - firstVal) * (sectionIndex - first);
    return val;
}

// 以最小二乘法做线性回归估计这直线方程式
// y=ax+b
double VectorMath::slope(DoubleVector y, double* intercept) const
{
    std::vector<double> arrayY;
    if (y.len == 0) {
        arrayY.resize(data_.size());
        for (int i = 0; i < data_.size(); i++) {
            arrayY[i] = i;
        }
        y = DoubleVector(arrayY.data(), data_.size());
    }
    CHECK(y.len == data_.size());

    DoubleVector Y = y;
    DoubleVector X = result();

    double t1 = VectorMath(X).mul(Y).sum() * X.len;
    double t2 = VectorMath(X).sum() * VectorMath(Y).sum();
    double t3 = VectorMath(X).mul(X).sum() * X.len;
    double t4 = std::pow(VectorMath(X).sum(), 2);
    double a = (t1 - t2) / (t3 - t4);

    double t5 = VectorMath(X).mul(X).sum() * VectorMath(Y).sum();
    double t6 = VectorMath(X).sum() * VectorMath(X).mul(Y).sum();
    double b = (t5 - t6) / (t3 - t4);

    if (intercept != nullptr) {
        *intercept = b;
    }

    return a;
}

VectorMath& VectorMath::add(DoubleVector b)
{
    for (int i = 0; i < data_.size() && i < b.len; i++) {
        data_[i] += b[i];
    }
    return *this;
}

VectorMath& VectorMath::add(double b)
{
    for (int i = 0; i < data_.size(); i++) {
        data_[i] += b;
    }
    return *this;
}

VectorMath& VectorMath::sub(DoubleVector b)
{
    for (int i = 0; i < data_.size() && i < b.len; i++) {
        data_[i] -= b[i];
    }
    return *this;
}

VectorMath& VectorMath::sub(double b)
{
    for (int i = 0; i < data_.size(); i++) {
        data_[i] -= b;
    }
    return *this;
}

VectorMath& VectorMath::mul(DoubleVector b)
{
    for (int i = 0; i < data_.size() && i < b.len; i++) {
        data_[i] *= b[i];
    }
    return *this;
}

VectorMath& VectorMath::mul(double b)
{
    for (int i = 0; i < data_.size(); i++) {
        data_[i] *= b;
    }
    return *this;
}

VectorMath& VectorMath::div(DoubleVector b)
{
    for (int i = 0; i < data_.size() && i < b.len; i++) {
        data_[i] /= b[i];
    }
    return *this;
}

VectorMath& VectorMath::div(double b)
{
    for (int i = 0; i < data_.size(); i++) {
        data_[i] /= b;
    }
    return *this;
}

// 零除零时候，用zeroByZeroValue代替值
VectorMath& VectorMath::financeDiv(DoubleVector b, double zeroByZeroValue)
{
    for (int i = 0; i < data_.size() && i < b.len; i++) {
        if (data_[i] == 0.0 && b[i] == 0.0) {
            data_[i] = zeroByZeroValue;
        }
        else {
            data_[i] /= b[i];
        }
    }
    return *this;
}

// 向右移动offset位，如果原值为NoValue，就不变；左边的用fillValue代替
// a[i]=a[i-offset];
VectorMath& VectorMath::shift(int offset, double fillValue)
{
    CHECK(offset >= 0 && offset <= data_.size());
    for (int i = data_.size() - 1; i >= offset; i--) {
        if (data_[i] != Bpm::NoValue) {
            data_[i] = data_[i - offset];
        }
    }
    for (int i = 0; i < offset; i++) {
        data_[i] = fillValue;
    }
    return *this;
}

// a[i] = a[i]-a[i-offset]
VectorMath& VectorMath::delta(int offset)
{
    if (offset <= 0 || offset > data_.size())
        return *this;
    CHECK(offset > 0 && offset <= data_.size());
    for (int i = data_.size() - 1; i >= offset; i--) {
        // if(data_[i-offset]==Bpm::NoValue){
        //    data_[i] = Bpm::NoValue;
        //}else{
        data_[i] -= data_[i - offset];
        //}
    }
    for (int i = 0; i < offset; i++) {
        data_[i] = Bpm::NoValue;
    }
    return *this;
}

// a[i] = a[i]/a[i-offset]
VectorMath& VectorMath::rate(int offset)
{
    if (offset <= 0 || offset > data_.size())
        return *this;
    CHECK(offset > 0 && offset <= data_.size());
    for (int i = data_.size() - 1; i >= offset; i--) {
        // if(data_[i-offset]==Bpm::NoValue){
        //    data_[i] = Bpm::NoValue;
        //}else{
        data_[i] /= data_[i - offset];
        //}
    }
    for (int i = 0; i < offset; i++) {
        data_[i] = Bpm::NoValue;
    }
    return *this;
}

// a[i] = abs(a[i])
VectorMath& VectorMath::abs()
{
    for (int i = 0; i < data_.size(); i++) {
        data_[i] = std::abs(data_[i]);
    }
    return *this;
}

// accumulated total of its previous element (including itself).
VectorMath& VectorMath::acc()
{
    for (int i = 1; i < data_.size(); i++) {
        data_[i] += data_[i - 1];
    }
    return *this;
}

// 保留一个子数组，-1 表示从startIndex一直都结尾都保留
VectorMath& VectorMath::trim(int startIndex, int len)
{
    CHECK(startIndex >= 0 && startIndex <= data_.size() - 1);
    CHECK(len >= -1);

    //右边的
    if (len >= 0) {
        data_.erase(data_.begin() + startIndex + len, data_.end());
    }
    //左边的
    data_.erase(data_.begin(), data_.begin() + startIndex);

    return *this;
}

// 插入数组，-1表示插入到结尾
VectorMath& VectorMath::insert(DoubleVector a, int insertPoint)
{
    if (insertPoint == -1) {
        insertPoint = data_.size();
    }
    std::vector<double> tmp;
    tmp.resize(a.len);
    memcpy(tmp.data(), a.data, a.len * sizeof(double));
    data_.insert(data_.begin() + insertPoint, tmp.begin(), tmp.end());

    return *this;
}

// 插入一段重复的值，-1表示插入到结尾
VectorMath& VectorMath::insert(double c, int len, int insertPoint)
{
    if (insertPoint == -1) {
        insertPoint = data_.size();
    }
    data_.insert(data_.begin() + insertPoint, len, c);

    return *this;
}

// 替换a为b
VectorMath& VectorMath::replace(double a, double b)
{
    std::replace(data_.begin(), data_.end(), a, b);
    return *this;
}

// 移动平均,interval为窗口
// 前interval-1个值替换为NoValue
VectorMath& VectorMath::movAvg(int interval)
{
    std::vector<double> x = data_;

    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).avg();
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }

    return *this;
}

// 指数平均,计算公式:
// avg(n) = value(n) * smoothingFactor + avg(n - 1) * (1 - smoothingFactor)
// smoothingFactor:should be between 0 - 1
// For (n = 0), its exponential average is the original value.
VectorMath& VectorMath::expAvg(double smoothingFactor)
{
    CHECK(smoothingFactor >= 0 && smoothingFactor <= 1);
    for (int i = 1; i < data_.size(); i++) {
        data_[i] = data_[i] * smoothingFactor + data_[i - 1] * (1 - smoothingFactor);
    }
    return *this;
}

// 移动中值，前internval-1个值替换为NoValue
VectorMath& VectorMath::movMed(int interval)
{
    std::vector<double> x = data_;

    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).med();
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }

    return *this;
}

// 移动百分位数
VectorMath& VectorMath::movPercentile(int interval, double p)
{
    CHECK(p >= 0 && p <= 100);
    std::vector<double> x = data_;
    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).percentile(p);
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }

    return *this;
}

VectorMath& VectorMath::movMax(int interval)
{
    std::vector<double> x = data_;
    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).maxValue();
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }

    return *this;
}

VectorMath& VectorMath::movMin(int interval)
{
    std::vector<double> x = data_;
    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).minValue();
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }
    return *this;
}

VectorMath& VectorMath::movStdDev(int interval)
{
    std::vector<double> x = data_;
    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).stdDevp();
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }

    return *this;
}

// 移动相关系数，moving correlation
// 特指：卡尔·皮尔逊设计的统计指标——相关系数(Pearson correlation coefficient)
VectorMath& VectorMath::movCorr(int interval, DoubleVector b)
{
    std::vector<double> arrayB;
    if (b.len == 0) {
        arrayB.resize(interval);
        for (int i = 0; i < interval; i++) {
            arrayB[i] = i;
        }
        b = DoubleVector(arrayB.data(), interval);
    }
    CHECK(b.len == interval);

    std::vector<double> x = data_;
    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).corr(b);
    }

    // ChartDirectorde 实现这里没有填充
    // for (int i = 0; i < first; i++) {
    //  data_[i] = Bpm::NoValue;
    //}

    return *this;
}

VectorMath& VectorMath::movSlope(int interval)
{
    std::vector<double> x = data_;
    int first = interval - 1;

    for (int i = first; i < x.size(); i++) {
        DoubleVector a(&x[i - interval + 1], interval);
        data_[i] = VectorMath(a).slope();
    }

    for (int i = 0; i < first; i++) {
        data_[i] = Bpm::NoValue;
    }

    return *this;
}

// 曲线平滑拟合，0-1-2-3-...作为x轴
// LOWESS算法
VectorMath& VectorMath::lowess(double smoothness, int iteration)
{
    CppLowess::TemplatedLowess<std::vector<double>, double> dlowess;
    int len = data_.size();
    std::vector<double> x(len), r(len), tmp1(len), tmp2(len);
    for (int i = 0; i < x.size(); i++) {
        x[i] = i;
    }
    dlowess.lowess(x, data_, smoothness, iteration, 0.0, r, tmp1, tmp2);
    data_.swap(r);

    return *this;
}

// 曲线平滑拟合，参数b作为x轴
// LOWESS算法
VectorMath& VectorMath::lowess(DoubleVector b,
    double smoothness,
    int iteration)
{
    CHECK(b.len == data_.size());
    CppLowess::TemplatedLowess<std::vector<double>, double> dlowess;
    std::vector<double> x(b.data, b.data + b.len);
    std::vector<double> r(b.len), tmp1(b.len), tmp2(b.len);
    dlowess.lowess(x, data_, smoothness, iteration, 0.0, r, tmp1, tmp2);
    data_.swap(r);
    return *this;
}

// select函数：decisionArray数组的值如果>0，那么对应下标的原数组的值保留否则被替换为fillValue
// 如果decisionArray为空，decisionArray等于原数组
// >0
VectorMath& VectorMath::selectGTZ(DoubleVector decisionArray,
    double fillValue)
{
    if (decisionArray.len == 0) {
        decisionArray = result();
    }
    for (int i = 0; i < decisionArray.len; i++) {
        if (decisionArray[i] > 0) {
        }
        else {
            data_[i] = fillValue;
        }
    }
    return *this;
}

// >=0
VectorMath& VectorMath::selectGEZ(DoubleVector decisionArray,
    double fillValue)
{
    if (decisionArray.len == 0) {
        decisionArray = result();
    }
    for (int i = 0; i < decisionArray.len; i++) {
        if (decisionArray[i] >= 0) {
        }
        else {
            data_[i] = fillValue;
        }
    }
    return *this;
}

// <0
VectorMath& VectorMath::selectLTZ(DoubleVector decisionArray,
    double fillValue)
{
    if (decisionArray.len == 0) {
        decisionArray = result();
    }
    for (int i = 0; i < decisionArray.len; i++) {
        if (decisionArray[i] < 0) {
        }
        else {
            data_[i] = fillValue;
        }
    }
    return *this;
}

// <=0
VectorMath& VectorMath::selectLEZ(DoubleVector decisionArray,
    double fillValue)
{
    if (decisionArray.len == 0) {
        decisionArray = result();
    }
    for (int i = 0; i < decisionArray.len; i++) {
        if (decisionArray[i] <= 0) {
        }
        else {
            data_[i] = fillValue;
        }
    }
    return *this;
}

// ==0
VectorMath& VectorMath::selectEQZ(DoubleVector decisionArray,
    double fillValue)
{
    if (decisionArray.len == 0) {
        decisionArray = result();
    }
    for (int i = 0; i < decisionArray.len; i++) {
        if (decisionArray[i] == 0) {
        }
        else {
            data_[i] = fillValue;
        }
    }
    return *this;
}

// !=0
VectorMath& VectorMath::selectNEZ(DoubleVector decisionArray,
    double fillValue)
{
    if (decisionArray.len == 0) {
        decisionArray = result();
    }
    for (int i = 0; i < decisionArray.len; i++) {
        if (decisionArray[i] != 0) {
        }
        else {
            data_[i] = fillValue;
        }
    }
    return *this;
}
