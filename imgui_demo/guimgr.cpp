#include "guimgr.h"

#include <thread>

#include "utils/logging.h"

GuiMgr::GuiMgr()
{
}

GuiMgr::~GuiMgr()
{
}

bool GuiMgr::runLoop()
{
    LOG(INFO) << __FUNCTION__ << ",enter";
    imgui::guiMain(this, L"imgui_demo", 100, 100, 800 + 20, 600 + 20);
    LOG(INFO) << __FUNCTION__ << ",leave";
    return quit_;
}

void GuiMgr::prepare()
{
    quit_ = close_ = false;
    clearCol_ = ImColor(114, 144, 154);
}

bool GuiMgr::runFrame()
{
    if (frameId_ % 60 == 0) {
    }
    frameId_++;

    // admin
    if (1) {
        ImGui::SetNextWindowSize(ImVec2(200, 120), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("admin");
        ImGui::Text("FPS:%.1f", ImGui::GetIO().Framerate);
        if (ImGui::Button("quit app")) {
            quit_ = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("close window")) {
            close_ = true;
        }

        ImGui::End();
    }

    // data
    if (1) {
        ImGui::SetNextWindowSize(ImVec2(540, 120), ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(20 + 200 + 20, 20),
            ImGuiSetCond_FirstUseEver);
        ImGui::Begin("data");
        {
            ImGui::Columns(6, "data_columns");
            ImGui::Separator();
            ImGui::Text("data_1");
            ImGui::NextColumn();
            ImGui::Text("data_2");
            ImGui::NextColumn();
            ImGui::Text("data_3");
            ImGui::NextColumn();
            ImGui::Text("data_4");
            ImGui::NextColumn();
            ImGui::Text("data_5");
            ImGui::NextColumn();
            ImGui::Text("data_6");
            ImGui::NextColumn();
            ImGui::Separator();
        }
        ImGui::End();
    }

    return (quit_ | close_);
}

//////
