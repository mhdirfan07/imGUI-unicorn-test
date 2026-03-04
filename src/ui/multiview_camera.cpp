#include "multiview_camera.h"
#include "../utils/graphics_utils.h"
#include "imgui.h"
#include <iostream>
#include <cstdio>

MultiViewCamera::MultiViewCamera() {}

MultiViewCamera::~MultiViewCamera() {
    if (state.cap.isOpened()) state.cap.release();
}

void MultiViewCamera::Initialize() {
    InitializeMoildev();
}

void MultiViewCamera::InitializeMoildev() {
    if (!state.isMoilInit) {
        try {
            // Original high resolution settings
            state.moil = std::make_unique<moildev::ocl::Moildev>(2592, 1944, 1249, 976, 1, 2592, 1944, 1, 0, 0, -32.973, 67.825, -41.581, 504.7);
            
            // Panorama maps
            state.mapXPano.create(1944, 2592, CV_32F);
            state.mapYPano.create(1944, 2592, CV_32F);
            
            // Anypoint maps (3x)
            for (int i = 0; i < 3; i++) {
                state.mapXAny[i].create(1944, 2592, CV_32F);
                state.mapYAny[i].create(1944, 2592, CV_32F);
            }
            
            state.isMoilInit = true;
            std::cout << "[INFO] MOILDEV Multi-View initialized successfully." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to initialize MOILDEV: " << e.what() << std::endl;
            state.isMoilInit = false;
        }
    }
}

void MultiViewCamera::Update() {
    UpdateCamera();
    ProcessFrame();
}

void MultiViewCamera::UpdateCamera() {
    if (state.selectedCamIndex != state.currentCamIndex || state.isFirstRun) {
        std::cout << "[INFO] Attempting to open camera device: " << state.selectedCamIndex << std::endl;
        
        if (state.cap.isOpened()) {
            state.cap.release();
            std::cout << "[INFO] Previous camera released." << std::endl;
        }
        
        // Try to open selected camera
        state.cap.open(state.selectedCamIndex);
        
        if (state.cap.isOpened()) {
            state.currentCamIndex = state.selectedCamIndex;
            
            // Optimize camera settings for better performance
            state.cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
            state.cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
            state.cap.set(cv::CAP_PROP_AUTOFOCUS, 0);  // Disable autofocus for consistency
            state.cap.set(cv::CAP_PROP_BUFFERSIZE, 1); // Minimize buffer to reduce latency
            
            std::cout << "[SUCCESS] Camera " << state.selectedCamIndex << " opened successfully." << std::endl;
        } else {
            std::cerr << "[ERROR] Failed to open camera " << state.selectedCamIndex << std::endl;
            std::cout << "[INFO] Attempting fallback to camera 0..." << std::endl;
            
            // Try fallback to camera 0
            state.cap.open(0);
            if (state.cap.isOpened()) {
                state.selectedCamIndex = 0;
                state.currentCamIndex = 0;
                state.cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
                state.cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
                state.cap.set(cv::CAP_PROP_AUTOFOCUS, 0);
                state.cap.set(cv::CAP_PROP_BUFFERSIZE, 1);
                std::cout << "[SUCCESS] Fallback to camera 0 successful." << std::endl;
            } else {
                std::cerr << "[ERROR] All camera open attempts failed." << std::endl;
                state.selectedCamIndex = state.currentCamIndex;
            }
        }
        state.isFirstRun = false;
    }
}

void MultiViewCamera::ProcessFrame() {
    if (!state.isMoilInit || !state.moil) {
        return;
    }
    
    if (state.cap.isOpened()) {
        // Grab frames without decoding to maintain frame rate
        if (!state.cap.grab()) {
            return;
        }
        
        // Process only every Nth frame to reduce computational load
        if (state.frameSkipCounter >= state.frameSkip) {
            if (!state.cap.retrieve(state.frame)) {
                return;
            }
            
            if (!state.frame.empty()) {
                // Resize to sensor resolution
                if (state.frame.cols != 2592 || state.frame.rows != 1944) {
                    cv::resize(state.frame, state.sensorFrame, cv::Size(2592, 1944), 0, 0, cv::INTER_AREA);
                } else {
                    state.sensorFrame = state.frame.clone();
                }
                
                // Update panorama and anypoint views
                UpdatePanoramaView();
                UpdateAnypointViews();
            }
            state.frameSkipCounter = 0;
        } else {
            state.frameSkipCounter++;
        }
        
        state.frameCounter++;
    }
}

void MultiViewCamera::UpdatePanoramaView() {
    // Only recalculate maps if parameters changed
    if (!state.panoCached || 
        state.lastPanoAlpha != state.pano_alpha || 
        state.lastPanoBeta != state.pano_beta || 
        state.lastPanoAlphaMax != state.pano_alpha_max) {
        
        float* px = (float*)state.mapXPano.data;
        float* py = (float*)state.mapYPano.data;
        state.moil->PanoramaCar(px, py, state.pano_alpha_max, state.pano_alpha, state.pano_beta, false, false);
        
        state.lastPanoAlpha = state.pano_alpha;
        state.lastPanoBeta = state.pano_beta;
        state.lastPanoAlphaMax = state.pano_alpha_max;
        state.panoCached = true;
    }
    
    // Always apply remap to latest frame
    cv::remap(state.sensorFrame, state.panoramaFrame, state.mapXPano, state.mapYPano, cv::INTER_LINEAR);
    cv::flip(state.panoramaFrame, state.panoramaFrame, -1);
    cv::resize(state.panoramaFrame, state.panoramaFrame, cv::Size(1280, 360), 0, 0, cv::INTER_AREA);
    
    // Apply Canny if enabled
    if (state.applyCanny) {
        cv::Mat gray, edges;
        cv::cvtColor(state.panoramaFrame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, 50, 150);
        cv::cvtColor(edges, state.panoramaFrame, cv::COLOR_GRAY2BGR);
    }
    
    if (state.texPanorama) glDeleteTextures(1, &state.texPanorama);
    state.texPanorama = GraphicsUtils::MatToTexture(state.panoramaFrame);
}

void MultiViewCamera::UpdateAnypointViews() {
    for (int i = 0; i < 3; i++) {
        // Check if parameters changed for this view
        if (!state.anyPointCached[i] || 
            state.lastAnyAlpha[i] != state.any_alpha[i] || 
            state.lastAnyBeta[i] != state.any_beta[i] || 
            state.lastAnyZoom[i] != state.any_zoom[i]) {
            
            float* px = (float*)state.mapXAny[i].data;
            float* py = (float*)state.mapYAny[i].data;
            state.moil->AnyPointM(px, py, state.any_alpha[i], state.any_beta[i], state.any_zoom[i]);
            
            state.lastAnyAlpha[i] = state.any_alpha[i];
            state.lastAnyBeta[i] = state.any_beta[i];
            state.lastAnyZoom[i] = state.any_zoom[i];
            state.anyPointCached[i] = true;
        }
        
        // Always apply remap to latest frame
        cv::remap(state.sensorFrame, state.anypointFrame[i], state.mapXAny[i], state.mapYAny[i], cv::INTER_LINEAR);
        cv::flip(state.anypointFrame[i], state.anypointFrame[i], -1);
        cv::resize(state.anypointFrame[i], state.anypointFrame[i], cv::Size(420, 360), 0, 0, cv::INTER_AREA);
        
        // Apply Canny if enabled
        if (state.applyCanny) {
            cv::Mat gray, edges;
            cv::cvtColor(state.anypointFrame[i], gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, edges, 50, 150);
            cv::cvtColor(edges, state.anypointFrame[i], cv::COLOR_GRAY2BGR);
        }
        
        if (state.texAnypoint[i]) glDeleteTextures(1, &state.texAnypoint[i]);
        state.texAnypoint[i] = GraphicsUtils::MatToTexture(state.anypointFrame[i]);
    }
}

void MultiViewCamera::Render() {
    ImGuiIO& io = ImGui::GetIO();
    
    // ========== PANORAMA VIEW (TOP) ==========
    ImGui::BeginChild("PanoramaView", ImVec2(0, 260), true);
    
    if (state.texPanorama) {
        float w = ImGui::GetContentRegionAvail().x;
        ImGui::Image((ImTextureID)(intptr_t)state.texPanorama, ImVec2(w, 260));
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "No panorama data available");
    }
    
    ImGui::EndChild();
    
    // ========== ANYPOINT VIEWS (BOTTOM 3x) ==========
    ImGui::BeginChild("AnypointView", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.8f, 1.0f), "ANYPOINT VIEWS");
    ImGui::Spacing();
    
    // 3 columns for anypoint views
    ImGui::Columns(3, "AnypointColumns", true);
    
    for (int i = 0; i < 3; i++) {
        if (state.texAnypoint[i]) {
            float colW = ImGui::GetColumnWidth() - 10;
            ImGui::Image((ImTextureID)(intptr_t)state.texAnypoint[i], ImVec2(colW - 10, colW * 0.85f));
        }
        
        char label[32];
        snprintf(label, sizeof(label), "Alpha##%d", i);
        ImGui::SliderFloat(label, &state.any_alpha[i], -90.f, 90.f);
        
        snprintf(label, sizeof(label), "Beta##%d", i);
        ImGui::SliderFloat(label, &state.any_beta[i], -180.f, 180.f);
        
        snprintf(label, sizeof(label), "Zoom##%d", i);
        ImGui::SliderFloat(label, &state.any_zoom[i], 1.f, 14.f);
        
        if (i < 2) ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::EndChild();
}
