#pragma once
#include "imgui.h"

// ==========================================
// WIDGET GALLERY VIEW
// ==========================================
class WidgetGalleryView {
public:
    WidgetGalleryView();
    
    void Render();
    
private:
    // Widget state variables
    bool showDemoWindow = false;
    bool check = true;
    int radio = 0;
    int counter = 0;
    char strInput[128] = "Hello World";
    float fValue = 0.5f;
    int iValue = 100;
    float col4[4] = {1.0f, 0.5f, 0.0f, 1.0f};
    float progress = 0.0f;
    int item_current = 0;
};
