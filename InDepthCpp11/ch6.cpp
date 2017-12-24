#include<iostream>
#include<chrono>
#include<thread>
#include<ratio>

#include<iomanip>
#include<ctime>
#include<string>
#include<cstdlib>

#include<codecvt>

using namespace std::chrono;

class Timer{
public:
    Timer():m_begin(high_resolution_clock::now()){}

    void reset()
    {
        m_begin = high_resolution_clock::now();
    }

    //默认输出毫秒
    template<typename Duration=milliseconds>
    int64_t elapsed()const
    {
        return duration_cast<Duration>(high_resolution_clock::now()-m_begin).count();
    }

    int64_t elapsed_micro()const
    {
        return duration_cast<microseconds>(high_resolution_clock::now()-m_begin).count();
    }

    int64_t elapsed_nano()const
    {
        return duration_cast<nanoseconds>(high_resolution_clock::now()-m_begin).count();
    }

    int64_t elapsed_seconds()const
    {
        return duration_cast<seconds>(high_resolution_clock::now()-m_begin).count();
    }

    int64_t elapsed_minutes()const
    {
        return duration_cast<minutes>(high_resolution_clock::now()-m_begin).count();
    }

    int64_t elapsed_hours()const
    {
        return duration_cast<hours>(high_resolution_clock::now()-m_begin).count();
    }

private:
    time_point<high_resolution_clock> m_begin;
};
void ch6_1()
{
    std::chrono::milliseconds ms1(3);
    std::chrono::microseconds mms1(6);

    std::chrono::duration<double,std::ratio<1,30>> hz30(3.5);
    std::cout<<"3 ms duration has "<<ms1.count()<<" ticks"<<std::endl;
    std::cout<<"6 us duration has "<<mms1.count()<<" ticks"<<std::endl;

    std::chrono::minutes t1(10);
    std::chrono::seconds t2(50);
    std::chrono::seconds t3 = t1-t2;
    std::cout<<t3.count()<<" second"<<std::endl;

    std::chrono::duration<double,std::ratio<9,7>> d1(3);
    std::chrono::duration<double,std::ratio<6,5>> d2(1);
    auto d3 = d2-d1;
    std::cout<<typeid(d3).name()<<std::endl;
    std::cout<<d3.count()<<std::endl;
    std::cout<<(std::chrono::duration_cast<std::chrono::minutes>(d3)).count()<<std::endl;

    //time_point 表示一个时间点
    typedef std::chrono::duration<int,std::ratio<60*60*24,1>> days_type;

    time_point<system_clock,days_type> today = time_point_cast<days_type>(system_clock::now());
    std::cout<<today.time_since_epoch().count()<<" days since epoch"<<std::endl;

    //time_point支持一些算术运算，比如两个time_point的差值周期数，和duration相加减，
    //Notes:不同clock的time_point是不能进行算术运算
    system_clock::time_point now_sys = system_clock::now();
    std:time_t lastTime = system_clock::to_time_t(now_sys-hours(24));
    std::time_t nextTime = system_clock::to_time_t(now_sys+hours(24));
    std::cout<<"One day ago,the time was "<<std::put_time(std::localtime(&lastTime),"%F %X")<<std::endl;
    std::cout<<"Next day,the time is"<<std::put_time(std::localtime(&nextTime),"%F %T")<<std::endl;

    /*获取系统时钟的clocks，表示当前的系统时钟，内部有time_point,duration,Rep,Period等信息，主要获取当前时间，以及实现time_t,time_point的相互转换。
     * system_clock 代表真实世界的挂钟时间，具体值依赖于系统
     * steady_clock 不能被“调整”的时钟，并不一定代表真实世界的挂钟时间，先后调用的now()得到的时间不会是递减的
     * high_resolution_clock 高精度时钟
    */
    system_clock::time_point tp1 = system_clock::now();
    std::cout<<"Hello,world."<<std::endl;
    system_clock::time_point tp2 = system_clock::now();
    std::cout<<duration_cast<microseconds>(tp2-tp1).count()<<" microseconds"<<std::endl;

    std::time_t ct2 = system_clock::to_time_t(tp2); //相反操作时 from_time_t
    std::cout<<std::put_time(std::localtime(&ct2),"%Y%m%d %H.%M.%S")<<std::endl;

    Timer timer;
    std::cout<<"Hi,test Timer!"<<std::endl;
    std::cout<<timer.elapsed()<<std::endl;
    std::cout<<timer.elapsed_hours()<<std::endl;
    std::cout<<timer.elapsed_micro()<<std::endl;
    std::cout<<timer.elapsed_minutes()<<std::endl;
    std::cout<<timer.elapsed_nano()<<std::endl;
    std::cout<<timer.elapsed_seconds()<<std::endl;
}

void ch6_2()
{
    /*数值类型和字符串的相互转换
     * atoi str->int
     * atol str->long
     * atoll str->long long
     * stof str->float
     * 如果需要转换的字符串前面部分不是数字，返回0；如果字符串前面包含数字和空格 ，会转换前面的数字部分，空格去掉
    */
    double f=1.53;
    std::string f_str = std::to_string(f);
    std::cout<<f_str<<std::endl;
    double f1=4.1253;
    std::wstring f_wstr = std::to_wstring(f1);
    std::wcout<<f_wstr<<std::endl;

    const char* s1="10";
    const char* s2="3.14";
    const char* s3 ="231 sd";
    const char* s4 = "sd323";

    auto num1 = std::atoi(s1);
    auto num2 = std::atoi(s2);
    auto num3 = std::atoi(s3);
    auto num4 = std::atoi(s4);
    std::cout<<"std::atoi(\""<<s1<<"\")="<<num1<<std::endl;
    std::cout<<"std::atoi(\""<<s2<<"\")="<<num3<<std::endl;
    std::cout<<"std::atoi(\""<<s3<<"\")="<<num3<<std::endl;
    std::cout<<"std::atoi(\""<<s4<<"\")="<<num4<<std::endl;
}

void ch6_3()
{
    /*宽字符串转换
     *
    */
    std::wstring str = L"中国人";
    std::wstring_convert<std::codecvt<wchar_t,char,std::mbstate_t>>
            converter(new std::codecvt<wchar_t,char,std::mbstate_t>("CHS"));
    std::string narrowStr = converter.to_bytes(str);
    std::cout<<narrowStr<<std::endl;

    std::wstring wstr = converter.from_bytes(narrowStr);
    std::wcout.imbue(std::locale("chs")); //初始化cout为中文输出
    std::wcout<<wstr<<std::endl;
}
void testCh6()
{
    std::cout<<"Start to test chapter 6"<<std::endl;
    ch6_1();
    ch6_2();
    ch6_3();
}
