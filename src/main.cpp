#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>

// Core modules
#include "core/system_monitor.h"
#include "core/database_manager.h"

// UI modules
#include "ui/camera_view.h"
#include "ui/multiview_camera.h"
#include "ui/database_view.h"
#include "ui/widget_gallery_view.h"
#include "ui/sidebar_ui.h"

// ==========================================
// GLOBAL OBJECTS & CONSTANTS
// ==========================================
ImFont* fontNormal = nullptr;
ImFont* fontHeader = nullptr;

enum AppView { VIEW_CAMERA = 0, VIEW_DATABASE = 1, VIEW_SETTINGS = 2 };

// ==========================================
// APPLICATION STATE
// ==========================================
struct AppState {
    int currentView = VIEW_CAMERA;
    double cpu_val = 0.0;
    double mem_val = 0.0;
    float timer = 0.0f;
    std::vector<std::string> appLogs;
};

// ==========================================
// UI RENDERING
// ==========================================
void RenderUI(AppState& appState, MultiViewCamera& multiViewCamera, DatabaseView& dbView, WidgetGalleryView& widgetGallery, SidebarUI& sidebar) {
    ImGuiIO& io = ImGui::GetIO();
    
    // Update monitoring every second
    appState.timer += io.DeltaTime;
    if (appState.timer > 1.0f) {
        appState.cpu_val = GetCPUUsage();
        appState.mem_val = GetMemoryUsageMB();
        appState.timer = 0.0f;
    }
    
    // Main window - full screen
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    // ========== SIDEBAR (LEFT) WITH GRADIENT ==========
    ImVec2 sidebarScreenPos(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
    ImVec2 sidebarScreenSize(80, io.DisplaySize.y - ImGui::GetCursorScreenPos().y);
    
    // Draw gradient sidebar background FIRST
    ImDrawList* bgDrawList = ImGui::GetWindowDrawList();
    ImU32 col_top = ImGui::GetColorU32(ImVec4(110.0f/255.0f, 145.0f/255.0f, 165.0f/255.0f, 1.0f));    // RGB(110, 145, 165)
    ImU32 col_bottom = ImGui::GetColorU32(ImVec4(60.0f/255.0f, 100.0f/255.0f, 130.0f/255.0f, 1.0f));   // Darker shade
    bgDrawList->AddRectFilledMultiColor(sidebarScreenPos, ImVec2(sidebarScreenPos.x + sidebarScreenSize.x, sidebarScreenPos.y + sidebarScreenSize.y),
                                        col_top, col_top, col_bottom, col_bottom);
    
    ImGui::BeginChild("LeftSidebar", ImVec2(80, 0), false);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));  // Transparent child bg
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 0.7f));  // Semi-transparent dark buttons
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.9f));  // Lighter on hover
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.45f, 0.8f, 1.0f));  // Blue when active
    sidebar.Render(appState.currentView);
    ImGui::PopStyleColor(4);
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // ========== MAIN CONTENT (CENTER/RIGHT) ==========
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    
    // Header with stats
    ImGui::TextColored(ImVec4(1.f, 0.8f, 0.2f, 1.f), "FPS: %.1f | CPU: %.1f%% | RAM: %.1f MB", 
                       io.Framerate, appState.cpu_val, appState.mem_val);
    ImGui::Separator();
    ImGui::Spacing();
    
    // ========== VIEW: CAMERA (MAIN VIEW) ==========
    if (appState.currentView == 0) { // Live Camera with Multi-View
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.6f, 1.0f), "MULTI-VIEW CAMERA (PANORAMA + ANYPOINT)");
        ImGui::Spacing();
        
        // Update and render multi-view camera
        multiViewCamera.Update();
        multiViewCamera.Render();
        
        // Camera controls
        ImGui::BeginChild("MultiViewControls", ImVec2(0, 100), true);
        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "CAMERA SETTINGS");
        ImGui::Spacing();
        
        MultiViewState& mvState = multiViewCamera.GetState();
        ImGui::Combo("Camera Device", &mvState.selectedCamIndex, "Camera 0\0Camera 1\0Camera 2\0");
        ImGui::Checkbox("Canny Edge Detection##multiview", &mvState.applyCanny);
        ImGui::SliderFloat("Panorama Alpha", &mvState.pano_alpha, -90.f, 90.f);
        ImGui::SliderFloat("Panorama Beta", &mvState.pano_beta, -180.f, 180.f);
        
        ImGui::EndChild();;
    }
    // ========== VIEW: DATABASE ==========
    else if (appState.currentView == 1) {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "DATABASE LOGS");
        ImGui::Spacing();
        dbView.Render();
    }
    // ========== VIEW: SETTINGS / WIDGET GALLERY ==========
    else if (appState.currentView == 2) {
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "WIDGET GALLERY");
        ImGui::Separator();
        widgetGallery.Render();
    }
    // ========== DEFAULT VIEWS FOR OTHER SIDEBAR ITEMS ==========
    else {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "View %d", appState.currentView);
        ImGui::TextWrapped("This is a placeholder for additional views. Click different icons in the sidebar to switch.");
    }
    
    ImGui::PopStyleVar();
    ImGui::EndChild();
    
    ImGui::End();
    ImGui::PopStyleVar();
}

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    std::cout << "=== Starting Moildev ImGui Application ===" << std::endl;
    
    // Initialize database
    DatabaseManager dbManager;
    try {
        dbManager.Initialize();
        std::cout << "[INFO] Database initialized successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to initialize database: " << e.what() << std::endl;
        return -1;
    }
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "[ERROR] Failed to initialize GLFW" << std::endl;
        return -1;
    }
    std::cout << "[INFO] GLFW initialized." << std::endl;
    
    // Set window hints for fixed size (non-resizable)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Moildev ImGui", NULL, NULL);
    if (!window) {
        std::cerr << "[ERROR] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "[INFO] Window created successfully (1280x720 - Fixed Size)." << std::endl;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    std::cout << "[INFO] ImGui initialized." << std::endl;
    
    // Load fonts with better path handling
    const char* fontPaths[] = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/noto/NotoSans-Regular.ttf",
        nullptr
    };
    
    fontNormal = nullptr;
    for (int i = 0; fontPaths[i] != nullptr; i++) {
        std::ifstream fNormal(fontPaths[i]);
        if (fNormal.good()) {
            fontNormal = io.Fonts->AddFontFromFileTTF(fontPaths[i], 16.0f);
            std::cout << "[INFO] Normal Font loaded from: " << fontPaths[i] << std::endl;
            break;
        }
    }
    
    if (!fontNormal) {
        fontNormal = io.Fonts->AddFontDefault();
        std::cout << "[INFO] Using default font" << std::endl;
    }
    
    fontHeader = nullptr;
    const char* fontHeaderPaths[] = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/noto/NotoSans-Bold.ttf",
        nullptr
    };
    
    for (int i = 0; fontHeaderPaths[i] != nullptr; i++) {
        std::ifstream fHeader(fontHeaderPaths[i]);
        if (fHeader.good()) {
            fontHeader = io.Fonts->AddFontFromFileTTF(fontHeaderPaths[i], 22.0f);
            std::cout << "[INFO] Header Font loaded from: " << fontHeaderPaths[i] << std::endl;
            break;
        }
    }
    
    if (!fontHeader) {
        fontHeader = fontNormal;
        std::cout << "[INFO] Using normal font for header" << std::endl;
    }
    
    // Build font atlas and update GPU resources
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    io.Fonts->Build();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
    
    // Setup ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 4.0f;
    style.WindowBorderSize = 0.0f;  // Hide borders
    style.ChildBorderSize = 0.0f;   // Hide child borders
    style.PopupBorderSize = 0.0f;   // Hide popup borders
    style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.45f, 0.8f, 1.f);
    
    // Dark gray background (#222222) for content views
    float darkBg = 34.0f / 255.0f;  // #222222
    style.Colors[ImGuiCol_WindowBg] = ImVec4(darkBg, darkBg, darkBg, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(darkBg, darkBg, darkBg, 1.0f);
    
    // Initialize views
    MultiViewCamera multiViewCamera;
    multiViewCamera.Initialize();
    DatabaseView dbView(&dbManager);
    WidgetGalleryView widgetGallery;
    SidebarUI sidebar;
    
    std::cout << "[INFO] All views initialized." << std::endl;
    std::cout << "=== Application started successfully ===" << std::endl;
    
    // Application state
    AppState appState;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        RenderUI(appState, multiViewCamera, dbView, widgetGallery, sidebar);
        
        ImGui::Render();
        
        int dw, dh;
        glfwGetFramebufferSize(window, &dw, &dh);
        glViewport(0, 0, dw, dh);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    std::cout << "[INFO] Shutting down..." << std::endl;
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    std::cout << "[INFO] Application closed successfully." << std::endl;
    return 0;
}
