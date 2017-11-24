#include "utils/breakpad.h"
#include "utils/fileutil.h"
#include "utils/logging.h"
#include "utils/strutil.h"

#include <chrono>
#include <thread>
/*
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
void test_boost_datetime() {
  LOG(INFO) << __FUNCTION__;
  boost::gregorian::date d1, d2, d3, d4;
  d1 = boost::gregorian::from_string("2017-2-25");
  d2 = boost::gregorian::from_undelimited_string("20170225");
  d3 = boost::gregorian::from_string("2017/2/25");
  d4 = boost::gregorian::from_string("2017,2,25");

  CHECK(d1.year() == 2017);
  CHECK(d1.month() == 2);
  CHECK(d1.day() == 25);
  CHECK(d1.day_of_week() == 6);

  CHECK(d2.year() == 2017);
  CHECK(d2.month() == 2);
  CHECK(d2.day() == 25);
  CHECK(d2.day_of_week() == 6);

  CHECK(d3.year() == 2017);
  CHECK(d3.month() == 2);
  CHECK(d3.day() == 25);
  CHECK(d3.day_of_week() == 6);

  CHECK(d4.year() == 2017);
  CHECK(d4.month() == 2);
  CHECK(d4.day() == 25);
  CHECK(d4.day_of_week() == 6);

  boost::posix_time::ptime pt1;
  pt1 = boost::posix_time::time_from_string("2017-02-25 12:13:14");
  boost::gregorian::date pd = pt1.date();
  boost::posix_time::time_duration pt = pt1.time_of_day();
  CHECK(pd.year() == 2017);
  CHECK(pd.month() == 2);
  CHECK(pd.day() == 25);
  CHECK(pd.day_of_week() == 6);

  CHECK(pt.hours() == 12);
  CHECK(pt.minutes() == 13);
  CHECK(pt.seconds() == 14);
  CHECK(pt.total_seconds() == 12 * 60 * 60 + 13 * 60 + 14);
}
*/

#include <boost/filesystem.hpp>
void test_boost_filesystem() {
  LOG(INFO) << __FUNCTION__;
  boost::filesystem::path path("c:/temp/mdapi/TradingDay.con");
  CHECK(path.empty() == false);
  CHECK(path.extension() == ".con");
  CHECK(path.filename() == "TradingDay.con");

  boost::filesystem::path new_path("c:/temp/boost/filesystem");
  CHECK(boost::filesystem::exists(new_path) == false);
  boost::filesystem::create_directories(new_path);
  CHECK(boost::filesystem::exists(new_path) == true);
  CHECK(boost::filesystem::is_directory(new_path) == true);
  boost::filesystem::remove_all("c:/temp/boost");
}

/*
#include <boost/format.hpp>
void test_boost_format() {
  LOG(INFO) << __FUNCTION__;
  std::string s = boost::str(boost::format("%s.%d") % "boost" % 5);
  CHECK(s == "boost.5");
}
*/

#include <boost/algorithm/string.hpp>
void test_boost_algorithm_string() {
  LOG(INFO) << __FUNCTION__;
  std::vector<std::string> ss;
  std::string context("boost.string.split");
  boost::split(ss, context, boost::is_any_of("."));
  CHECK(ss.size() == 3);
  CHECK(ss[0] == "boost");
  CHECK(ss[1] == "string");
  CHECK(ss[2] == "split");

  CHECK(boost::to_upper_copy(std::string("boost")) == "BOOST");
  CHECK(boost::trim_copy(std::string("   boost   ")) == "boost");
  CHECK(boost::trim_left_copy(std::string("   boost ")) != "boost");
}

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
void test_boost_asio() {
  LOG(INFO) << __FUNCTION__;
  boost::asio::io_service ios;
  boost::asio::deadline_timer t(ios, boost::posix_time::milliseconds(100));
  t.async_wait(
      [](const boost::system::error_code&) { LOG(INFO) << "timeout!"; });
  ios.run();
}

/*
//
// doc:
// http://www.cnblogs.com/biyeqingfeng/p/4960593.html
// http://blog.sina.com.cn/s/blog_62b4e3ff0100yyzr.html
// http://blog.csdn.net/for_tech/article/details/51122674
//
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
void test_boost_statechart() {
  LOG(INFO) << __FUNCTION__;
  namespace sc = boost::statechart;

  struct Event1 : sc::event<Event1> {};
  struct Event2 : sc::event<Event2> {};

  struct SimpleState1;

  struct StateMachine1 : sc::state_machine<StateMachine1, SimpleState1> {
    StateMachine1(void) { LOG(INFO) << "construct StateMachine1"; }
    ~StateMachine1(void) { LOG(INFO) << "distruct StateMachine1"; }
  };

  struct SimpleState1_1;

  struct SimpleState1
      : sc::simple_state<SimpleState1, StateMachine1, SimpleState1_1> {
    // typedef sc::transition<Event1, SimpleState1> reactions;
    SimpleState1(void) { LOG(INFO) << "enter SimpleState1"; }
    ~SimpleState1(void) { LOG(INFO) << "leave SimpleState1"; }
    typedef sc::custom_reaction<Event1> reactions;
    sc::result react(const Event1& evt) { return transit<SimpleState1>(); }
  };

  struct SimpleState1_2;

  struct SimpleState1_1 : sc::simple_state<SimpleState1_1, SimpleState1> {
    // typedef sc::transition<Event2, SimpleState1_2> reactions;
    SimpleState1_1(void) { LOG(INFO) << "enter SimpleState1_1"; }
    ~SimpleState1_1(void) { LOG(INFO) << "leave SimpleState1_1"; }
    typedef sc::custom_reaction<Event2> reactions;
    sc::result react(const Event2& evt) { return transit<SimpleState1_2>(); }
  };

  struct SimpleState1_2 : sc::simple_state<SimpleState1_2, SimpleState1> {
    // typedef sc::transition<Event2, SimpleState1_1> reactions;
    SimpleState1_2(void) { LOG(INFO) << "enter SimpleState1_2"; }
    ~SimpleState1_2(void) { LOG(INFO) << "leave SimpleState1_2"; }
    typedef sc::custom_reaction<Event2> reactions;
    sc::result react(const Event2& evt) { return transit<SimpleState1_1>(); }
  };

  if (1) {
    StateMachine1 myMachine;
    myMachine.initiate();
    LOG(INFO) << "----- before process Event2";
    myMachine.process_event(Event2());
    LOG(INFO) << "----- after process Event2";
    LOG(INFO) << "----- before process Event1";
    myMachine.process_event(Event1());
    LOG(INFO) << "----- after process Event1";
  }
}
*/

void test_chrono_timepoint()
{
    LOG(INFO) << __FUNCTION__;
    std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
    LOG(INFO) << duration.count();
}

/*
//
// doc:
// http://blog.csdn.net/zengraoli/article/details/9697841
//
#include <boost/signals2.hpp>
void test_boost_signal2() {
  LOG(INFO) << __FUNCTION__;
  std::function<void(int)> cb1 = [](int a) { LOG(INFO) << "cb1:" << a; };
  std::function<void(int)> cb2 = [](int a) { LOG(INFO) << "cb2:" << a; };

  boost::signals2::signal<void(int)> signal;
  auto c1 = signal.connect(0, cb1);
  auto c2 = signal.connect(0, cb2);
  signal(2);
  c1.disconnect();
  signal(1);
  c2.disconnect();
  signal(0);
}
*/
#include "utils/timeutil.h"
void test_utils_timeutil()
{
    LOG(INFO) << __FUNCTION__;
    time_t t = 0;

    t = bpm_str2ctime("2019-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2018-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2017-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2016-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2015-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2014-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2013-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2012-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2011-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2010-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2009-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    t = bpm_str2ctime("2008-03-02 20:31:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 60 == 0);
    /*
    // 本地时间有个8小时偏移，哎
    t = str2time("2019-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2018-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2017-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2016-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2015-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2014-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2013-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2012-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2011-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2010-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2009-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);
    t = str2time("2008-03-02 20:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 3600 == 0);

    t = str2time("2019-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2018-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2017-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2016-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2015-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2014-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2013-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2012-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2011-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2010-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2009-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
    t = str2time("2008-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t % 86400 == 0);
*/
    t = bpm_timeofctime("2019-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2018-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2017-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2016-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2015-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2014-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2013-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2012-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2011-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2010-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2009-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
    t = bpm_timeofctime("2008-03-02 08:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(t == 8 * 60 * 60);
}

#include <map>
void test_timesection(const char* dt, bool result)
{
    struct TimeSectionPair {
        int tpBegin;
        int tpEnd;
    };
    std::map<std::string, std::vector<TimeSectionPair> > timeSectionParsed;

    std::string x = "rb1710";
    std::string tsFilter = "09:00:00-10:15:00;10:30:00-11:30:00;13:30:00-15:00:00;21:00:00-23:00:00";
    std::vector<std::string> arrayFilter = StrUtil::split(tsFilter, ";");
    std::vector<TimeSectionPair> tsVector;
    for (auto& section : arrayFilter) {
        std::vector<std::string> sectionPair = StrUtil::split(section, "-");
        if (sectionPair.size() >= 1) {
            std::string timeBegin = StrUtil::printf("%s-%s", "20170519", sectionPair[0].c_str());
            std::string timeEnd = StrUtil::printf("%s-%s", "20170519", sectionPair[1].c_str());
            TimeSectionPair p;
            p.tpBegin = bpm_timeofctime(timeBegin.c_str(), "%Y%m%d-%H:%M:%S");
            p.tpEnd = bpm_timeofctime(timeEnd.c_str(), "%Y%m%d-%H:%M:%S");
            tsVector.push_back(p);
        }
    }
    timeSectionParsed[x] = std::move(tsVector);

    bool tsOk = false;
    int tpTick = bpm_timeofctime(dt, "%Y%m%d-%H:%M:%S");
    auto itTs = timeSectionParsed.find(x);
    if (itTs != timeSectionParsed.end()) {
        auto& vectorPair = itTs->second;
        for (auto& pair : vectorPair) {
            if (tpTick >= pair.tpBegin && tpTick < pair.tpEnd) {
                tsOk = true;
                break;
            }
        }
    }
    CHECK(tsOk == result);
}

#include <string>
#include <zmq.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n) Sleep(n)
#endif

void test_libzmq_client()
{
    LOG(INFO) << __FUNCTION__;

    LOG(INFO) << "Connecting to hello world server...\n";
    void* context = zmq_ctx_new();
    void* requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer[10];
        LOG(INFO) << "C:Sending Hello : " << request_nbr;
        zmq_send(requester, "Hello", 5, 0);
        zmq_recv(requester, buffer, 10, 0);
        LOG(INFO) << "C:Received World : " << request_nbr;
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return;
}

void test_libzmq_server()
{
    LOG(INFO) << __FUNCTION__;

    //  Socket to talk to clients
    void* context = zmq_ctx_new();
    void* responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    CHECK(rc == 0);

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer[10];
        zmq_recv(responder, buffer, 10, 0);
        LOG(INFO) << "S:Received Hello : " << request_nbr;
        sleep(1); //  Do some 'work'
        zmq_send(responder, "World", 5, 0);
    }
    return;
}

// doc:
// http://zguide.zeromq.org/page:all
void test_libzmq()
{
    LOG(INFO) << __FUNCTION__;
    std::thread s(test_libzmq_server);
    sleep(1000);
    std::thread c(test_libzmq_client);
    c.join();
    s.join();
}

// windows下的std::signal就是个傻X?是qtcreator的run父进程是个傻X
// 它第二次ctrl+c把test给杀了
#include <csignal>
void signal_handler(int signal)
{
    LOG(INFO) << __FUNCTION__ << ",can't kill by ctrl+c";
    std::signal(SIGINT, signal_handler);
}

void test_std_signal()
{
    LOG(INFO) << __FUNCTION__;
    // Install a signal handler
    std::signal(SIGINT, signal_handler);
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

#ifdef _WIN32
#include <windows.h>
static BOOL WINAPI ctrl_c_handler(DWORD code)
{
    if (code == CTRL_C_EVENT) {
        LOG(INFO) << __FUNCTION__ << ",can't kill by ctrl+c";
        return TRUE; /* We just ignore all control events. */
    }
    return FALSE;
}

void test_win32_signal()
{
    LOG(INFO) << __FUNCTION__;
    SetConsoleCtrlHandler(ctrl_c_handler, TRUE);
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
#endif

void test_breakpad()
{
    LOG(INFO) << __FUNCTION__;
    CHECK(0);
}

void test_finance_time()
{
    LOG(INFO) << __FUNCTION__;
    std::time_t b1, b2;

    b1 = bpm_str2ctime("2017-03-10 21:00:00", "%Y-%m-%d %H:%M:%S");
    b2 = bpm_str2ctime("2017-03-13 09:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(bpm_sametradingday(b1, b2));

    b1 = bpm_str2ctime("2017-03-10 09:00:00", "%Y-%m-%d %H:%M:%S");
    b2 = bpm_str2ctime("2017-03-13 09:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(!bpm_sametradingday(b1, b2));

    b1 = bpm_str2ctime("2017-03-10 21:00:00", "%Y-%m-%d %H:%M:%S");
    b2 = bpm_str2ctime("2017-03-13 21:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(!bpm_sametradingday(b1, b2));

    b1 = bpm_str2ctime("2017-03-13 09:00:00", "%Y-%m-%d %H:%M:%S");
    b2 = bpm_str2ctime("2017-03-13 21:00:00", "%Y-%m-%d %H:%M:%S");
    CHECK(!bpm_sametradingday(b1, b2));
}

static void slot_1()
{
    LOG(INFO) << __FUNCTION__;
}

static void slot_2()
{
    LOG(INFO) << __FUNCTION__;
}

#include "utils/sigslot.h"
void test_utils_sigslot()
{
    LOG(INFO) << __FUNCTION__;
    vdk::signal<void(void)> sig;
    sig.connect(&slot_1);
    sig.connect(&slot_2);
    sig.emit();
    CHECK(!sig.empty());
    sig.disconnect(&slot_1);
    CHECK(!sig.empty());
    sig.emit();
    sig.disconnect(&slot_2);
    CHECK(sig.empty());
    sig.connect(&slot_1);
    sig.connect(&slot_2);
    sig.emit();
    sig.disconnect_all();
    CHECK(sig.empty());
}
/*
#include <uv.h>
uint64_t libuv_timer_repeat = 0;
static void libuv_timer_callback(uv_timer_t* handle) {
  LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
  libuv_timer_repeat += 1;
  LOG(INFO) << "libuv_timer_repeat count:" << libuv_timer_repeat;
  if (libuv_timer_repeat == 10) {
    uv_loop_t* loop = uv_default_loop();
    uv_timer_stop(handle);
    uv_stop(loop);
  }
}

uint64_t libuv_signal_repeat = 0;
static void libuv_signal_handler(uv_signal_t* handle, int sig) {
  LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
  libuv_signal_repeat += 1;
  LOG(INFO) << "libuv_signal_repeat count:" << libuv_signal_repeat;
  if (libuv_signal_repeat == 3) {
    uv_loop_t* loop = uv_default_loop();
    uv_signal_stop(handle);
    uv_stop(loop);
  }
}

uv_loop_t* libuv_loop;
uv_async_t libuv_async;
static void libuv_async_handler(uv_async_t* handle) {
  LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
  // int got = (int)libuv_async.data;
  // LOG(INFO) << __FUNCTION__ << ",got:" << got;
}

static void libuv_threadMain() {
  LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
  for (int i = 0; i < 10; i++) {
    // libuv_async.data = (void*)i;
    uv_async_send(&libuv_async);
  }
}

void test_libuv_timer() {
  LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
  uv_loop_t* loop = uv_default_loop();

  // http://blog.csdn.net/fwb330198372/article/details/52842303
  uv_timer_t timer_req;
  uv_timer_init(loop, &timer_req);
  uv_timer_start(&timer_req, libuv_timer_callback, 1000, 1000);

  // http://blog.csdn.net/wangcg123/article/details/52386488
  uv_signal_t signal_req;
  uv_signal_init(loop, &signal_req);
  uv_signal_start(&signal_req, libuv_signal_handler, SIGINT);

  // http://www.cnblogs.com/guoxiaoqian/p/3945242.html
  libuv_loop = loop;
  uv_async_init(loop, &libuv_async, libuv_async_handler);
  std::thread t(libuv_threadMain);
  t.join();

  uv_run(loop, UV_RUN_DEFAULT);
}
*/


#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>

int countSignalGot_ = 0;
void asio_handleSignal(boost::system::error_code,
                       int,
                       boost::asio::signal_set* ss,
                       boost::asio::io_service* ios) {
  countSignalGot_++;
  LOG(INFO) << __FUNCTION__ << ",got ctrl+c:" << countSignalGot_;
  if (countSignalGot_ == 10) {
    ss->cancel();
    ios->stop();
  } else {
    ss->async_wait(std::bind(&asio_handleSignal, std::placeholders::_1,
                             std::placeholders::_2, ss, ios));
  }
}

void test_asio_signal() {
  LOG(INFO) << __FUNCTION__;
  boost::asio::io_service ios;
  boost::asio::signal_set ss(ios);
  ss.add(SIGINT);
  boost::asio::io_service::work wk(ios);
  ss.async_wait(std::bind(&asio_handleSignal, std::placeholders::_1,
                          std::placeholders::_2, &ss, &ios));
  ios.run();
}


#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/thread.h>
#include <event2/util.h>
int libevent_timer_count = 0;
static void libevent_timeout_cb(evutil_socket_t fd, short x, void* arg)
{
    libevent_timer_count++;
    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    LOG(INFO) << __FUNCTION__ << ",count = " << libevent_timer_count;
    event* timeout = (event*)arg;
    if (libevent_timer_count == 10) {
        evtimer_del(timeout);
    }
}

int libevent_signal_count = 0;
static void libevent_signal_cb(evutil_socket_t fd, short x, void* arg)
{
    libevent_signal_count++;
    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    LOG(INFO) << __FUNCTION__ << ",count = " << libevent_signal_count;
    event* signal = (event*)arg;
    if (libevent_signal_count == 10) {
        evsignal_del(signal);
    }
}

// http://blog.csdn.net/hursing/article/details/45440081
void test_libevent_timer()
{
#if defined(WIN32)
    WSADATA WSAData;
    WSAStartup(0x0202, &WSAData);
    evthread_use_windows_threads();
#else
    evthread_use_pthreads();
#endif

    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    event_base* base;
    base = event_base_new();

    event timeout;
    timeval tv;
    event_assign(&timeout, base, -1, EV_PERSIST, libevent_timeout_cb,
        (void*)&timeout);
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    evtimer_add(&timeout, &tv);

    event signal;
    evsignal_assign(&signal, base, SIGINT, libevent_signal_cb, (void*)&signal);
    event_add(&signal, NULL);

    event_base_dispatch(base);
    event_base_free(base);

#if defined(WIN32)
    WSACleanup();
#endif
}
/*
#include "lightev/lev.h"
#include "lightev/levhttp.h"
using namespace lev;
static void onHttpHello(struct evhttp_request* req, void* arg)
{
    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    EvHttpRequest evreq(req);
    evreq.output().printf("<..>Hello World!<..>");
    evreq.sendReply(200, "OK");

    timeval tv;
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    EvBaseLoop* loop = (EvBaseLoop*)arg;
    // loop->decLoopRef();
    // loop->loopbreak();
    loop->loopexit(&tv);
}

int test_libevent_lev()
{
    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    EvInit evinit;
    EvBaseLoop base;
    EvHttpServer http(base);

    http.addRoute("/hello", onHttpHello, &base);
    http.bind("127.0.0.1", 8080);

    // base.incLoopRef();
    base.loop();

    return 0;
}

static void onEventHappen(evutil_socket_t fd, short x, void* arg)
{
    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    EvEvent* ev = (EvEvent*)arg;
    int v = (int)ev->userData1();
    LOG(INFO) << __FUNCTION__ << ",val:" << v;
    ev->exitLoop();
    delete ev;
}

static void notifyEvent(event_base* loop)
{
    EvEvent* ev = new EvEvent;
    ev->newUser(onEventHappen, loop);
    ev->setUserData1((void*)100);
    ev->activateUser(1);
}

int test_libevent_lev_event()
{
    LOG(INFO) << __FUNCTION__ << ",tid:" << std::this_thread::get_id();
    EvInit evinit;
    EvBaseLoop loop;
    std::thread t(std::bind(notifyEvent, loop.base()));
    t.join();
    loop.loop();

    return 0;
}
*/
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>
void test_nana_helloworld()
{
    using namespace nana;
    form fm; // Our main window
    fm.caption("My first NANA demo"); // (with this title)
    place fm_place{ fm }; // have automatic layout
    bool really_quick{ false }; // and a default behavior
    label hello{ fm, "Hello World" }; // We put a label on our window
    button btn{ fm, "Quit" }; // and a button
    btn.tooltip("I will ask first"); // that show a tip upon mouse hover
    group act{ fm, "Actions" }; // Add a group of "options"
    act.add_option("Quick quickly") // and two options that control quick behavior
        .events()
        .click([&]() {   really_quick=true;
                                   btn.tooltip("Quick quickly"); });
    act.add_option("Ask first")
        .events()
        .click([&]() {   really_quick=false;
                                   btn.tooltip("I will ask first"); });
    btn.events().click([&]() // now the button know how to respond
        {
            if (!really_quick) // not really quick !
            {
                msgbox m(fm, "Our demo", msgbox::yes_no);
                m.icon(m.icon_question);
                m << "Are you sure you want to quick?";
                auto response = m();
                if (response != m.pick_yes)
                    return; // return to the demo
            }
            API::exit(); // or really quick
        });
    act.radio_mode(true); // Set "radio mode" (only one option selected)
    // let divide fm into fields to holds the other controls.
    // for example, let split fm into two fields separated by a movable vertical barre.
    fm_place.div("vertical <label margin=10>|70% <actions>");
    fm_place["label"] << hello << btn; // and place the controls there
    fm_place["actions"] << act;
    fm_place.collocate(); // and collocate all in place
    fm.show();
    exec();
}

#include "fmt/format.h"
void test_fmt(){
    fmt::print("Hello, {}!", "world");
    bool b = true;
    uint8_t u8 = 8;
    uint16_t u16 = 16;
    uint32_t u32 = 32;
    uint64_t u64 = 64;
    float_t f32 = 0.32f;
    double_t f64 = 0.64;
    std::string s = fmt::format("{}.{}.{}.{}.{}.{}.{}",b,u8,u16,u32,u64,f32,f64);
    fmt::print(s);
}

int main(int argc, char* argv[])
{
    bpm_CrashDumpInitialize();
    FLAGS_log_dir = "c:\\temp\\log";
    bpm_createDir(FLAGS_log_dir.c_str());
    FLAGS_alsologtostderr = true;
    google::InitGoogleLogging(argv[0]);


    // test_boost_datetime();
    // test_boost_filesystem();
    // test_boost_format();
    test_boost_algorithm_string();
     test_boost_asio();
    // test_boost_statechart();
    test_chrono_timepoint();
    // test_boost_signal2();
    test_utils_timeutil();
    test_libzmq();

// test_std_signal();
#ifdef _WIN32
// test_win32_signal();
#endif

    test_finance_time();
    test_utils_sigslot();
    // test_libuv_timer();
   test_asio_signal();

    //test_libevent_timer();
    //test_libevent_lev();
    //test_libevent_lev_event();

    test_timesection("20170313-08:59:00", false);
    test_timesection("20170313-09:00:00", true);
    test_timesection("20170313-10:15:00", false);
    test_timesection("20170313-10:30:00", true);
    test_timesection("20170313-11:30:00", false);
    test_timesection("20170313-13:30:00", true);
    test_timesection("20170313-15:00:00", false);
    test_timesection("20170313-20:59:00", false);
    test_timesection("20170313-21:00:00", true);
    test_timesection("20170313-23:00:00", false);

    test_nana_helloworld();

    test_fmt();

    // test_breakpad();

    google::ShutdownGoogleLogging();
}
