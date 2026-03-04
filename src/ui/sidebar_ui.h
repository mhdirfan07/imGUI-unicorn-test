#pragma once
#include "imgui.h"

// ==========================================
// SIDEBAR UI MODULE
// ==========================================
enum SidebarIcon {
    ICON_HOME = 0,
    ICON_CAMERA = 1,
    ICON_VIDEO = 2,
    ICON_RECORD = 3,
    ICON_SLIDERS = 4,
    ICON_SETTINGS = 5,
    ICON_DATABASE = 6,
    ICON_GALLERY = 7,
    ICON_VEHICLE = 8,
    ICON_STEERING = 9,
    ICON_CLOCK = 10,
};

class SidebarUI {
public:
    SidebarUI();
    
    void Render(int& selectedView);
    int GetSelectedView() const { return selectedView; }
    
private:
    int selectedView = 0;
    
    void RenderIconButton(const char* icon, const char* label, int viewId, int& selected);
};
