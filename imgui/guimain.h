#ifndef IMGUI_GUIMAIN_H
#define IMGUI_GUIMAIN_H

#include "imgui.h"

class GuiMgrBase {
 public:
  GuiMgrBase() {}
  virtual ~GuiMgrBase() {}

  virtual void prepare() = 0;
  // true:返回true退出循环
  virtual bool runFrame() = 0;
  virtual ImVec4* clearCol() = 0;
};

namespace imgui {
IMGUI_API int guiMain(GuiMgrBase* guiMgr,
                      const wchar_t* title,
                      int x,
                      int y,
                      int width,
                      int height);
}
#endif  // IMGUI_GUIMAIN_H
