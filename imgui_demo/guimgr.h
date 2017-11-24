#ifndef GUIMGR_H
#define GUIMGR_H

#include <map>

#include "imgui/guimain.h"

class GuiMgr : public GuiMgrBase {
public:
    GuiMgr();
    ~GuiMgr() override;

    // true: quit app
    bool runLoop();

    // imgui protocol
    void prepare() override;
    bool runFrame() override;
    ImVec4* clearCol() override { return &clearCol_; }

private:
    bool quit_ = false;
    bool close_ = false;
    ImVec4 clearCol_;
    int frameId_ = 0;
};

#endif // GUIMGR_H
