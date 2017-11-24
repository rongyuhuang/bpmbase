#include "guimgr.h"

#include <chrono>
#include <thread>

#include <lightev/lev.h>
#include "utils/breakpad.h"
#include "utils/logging.h"

class CtpApp {
public:
    CtpApp()
        : ios_(lev::EvBaseLoop())
        , signalset_()
    {
    }

    void run()
    {
        signalset_.setUserData1(this);
        signalset_.newSignal(CtpApp::handleSignal, SIGINT, ios_.base());
        signalset_.start();
        LOG(INFO) << __FUNCTION__ << ",running,ctrl+c for gui";
        ios_.loop();
        if (guiThread_.get()) {
            guiThread_->join();
            guiThread_.reset(nullptr);
        }
    }

private:
    static void handleSignal(evutil_socket_t, short, void* arg)
    {
        lev::EvEvent* ev = (lev::EvEvent*)arg;
        CtpApp* app = (CtpApp*)ev->userData1();
        app->countSignalGot_++;
        LOG(INFO) << __FUNCTION__ << ",got ctrl+c:" << app->countSignalGot_;
        if (app->guiThread_.get()) {
            LOG(INFO) << __FUNCTION__ << ",gui running already";
        } else {
            app->guiThread_.reset(
                new std::thread(std::bind(&CtpApp::guiThreadMain, app)));
        }
    }

    void guiThreadMain()
    {
        GuiMgr guiMgr;
        bool needQuit = guiMgr.runLoop();
        guiThread_->detach();
        guiThread_.reset(nullptr);
        if (needQuit) {
            ios_.loopexit(nullptr);
        }
    }

private:
    lev::EvBaseLoop ios_;
    lev::EvEvent signalset_;
    int countSignalGot_ = 0;
    std::unique_ptr<std::thread> guiThread_;
};


int main(int argc, char* argv[])
{
    bpm_CrashDumpInitialize();
    FLAGS_log_dir = "c:\\temp\\log";
    FLAGS_alsologtostderr = true;
    google::InitGoogleLogging(__argv[0]);

    lev::EvInit evinit;
    {
        CtpApp ctpApp;
        ctpApp.run();
    }

    google::ShutdownGoogleLogging();
    return 0;
}
