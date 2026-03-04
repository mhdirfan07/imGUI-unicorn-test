#include "camera_view.h"
#include "../utils/graphics_utils.h"
#include "imgui.h"
#include <iostream>

CameraView::CameraView() {}

CameraView::~CameraView() {
    if (state.cap.isOpened()) state.cap.release();
}

void CameraView::Initialize() {
    InitializeMoildev();
}

void CameraView::InitializeMoildev() {
    if (!state.isMoilInit) {
        try {
            state.moil = std::make_unique<moildev::ocl::Moildev>(2592, 1944, 1249, 976, 1, 2592, 1944, 1, 0, 0, -32.973, 67.825, -41.581, 504.7);
            state.mapX.create(1944, 2592, CV_32F);
            state.mapY.create(1944, 2592, CV_32F);
            state.isMoilInit = true;
            std::cout << "[INFO] MOILDEV initialized successfully." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to initialize MOILDEV: " << e.what() << std::endl;
            state.isMoilInit = false;
        }
    }
}

void CameraView::Update() {
    UpdateCamera();
    ProcessFrame();
}

void CameraView::UpdateCamera() {
    if (state.selectedCamIndex != state.currentCamIndex || state.isFirstRun) {
        if (state.cap.isOpened()) state.cap.release();
        state.cap.open(state.selectedCamIndex);
        if (state.cap.isOpened()) {
            state.currentCamIndex = state.selectedCamIndex;
        } else {
            state.selectedCamIndex = state.currentCamIndex;
            state.cap.open(state.currentCamIndex);
        }
        state.isFirstRun = false;
    }
}

void CameraView::ProcessFrame() {
    if (!state.isMoilInit || !state.moil) {
        return; // Skip processing jika MOILDEV belum ready
    }
    
    if (state.cap.isOpened()) {
        state.cap >> state.frame;
        if (!state.frame.empty()) {
            cv::Mat sensorFrame;
            cv::resize(state.frame, sensorFrame, cv::Size(2592, 1944));
            
            // Apply MOILDEV transformation
            if (state.moilMode != 0) {
                if (state.moilParamsChanged) {
                    float* px = (float*)state.mapX.data;
                    float* py = (float*)state.mapY.data;
                    if (state.moilMode == 1) {
                        state.moil->AnyPointM(px, py, state.any_alpha, state.any_beta, state.any_zoom);
                    } else if (state.moilMode == 2) {
                        state.moil->PanoramaCar(px, py, state.pano_alpha_max, state.pano_alpha, state.pano_beta, false, false);
                    }
                    state.moilParamsChanged = false;
                }
                cv::remap(sensorFrame, state.moilFrame, state.mapX, state.mapY, cv::INTER_LINEAR);
                cv::flip(state.moilFrame, state.moilFrame, -1);
                if (state.moilMode == 2) {
                    cv::resize(state.moilFrame, state.moilFrame, cv::Size(state.moilFrame.cols * 2, state.moilFrame.rows));
                }
                cv::resize(state.moilFrame, state.moilFrame, cv::Size(1280, 720));
            } else {
                cv::resize(sensorFrame, state.moilFrame, cv::Size(1280, 720));
            }
            
            // Apply Canny filter if enabled
            if (state.applyCanny) {
                cv::Mat gray, edges;
                cv::cvtColor(state.moilFrame, gray, cv::COLOR_BGR2GRAY);
                cv::Canny(gray, edges, 50, 150);
                cv::cvtColor(edges, state.processedFrame, cv::COLOR_GRAY2BGR);
            } else {
                state.processedFrame = state.moilFrame;
            }
            
            // Update textures
            if (state.texOriginal) glDeleteTextures(1, &state.texOriginal);
            state.texOriginal = GraphicsUtils::MatToTexture(sensorFrame);
            
            if (state.texResult) glDeleteTextures(1, &state.texResult);
            state.texResult = GraphicsUtils::MatToTexture(state.processedFrame);
            
            state.frameCounter++;
        }
    }
}

void CameraView::Render() {
    ImGui::Checkbox("Apply Canny Edge Filter", &state.applyCanny);
    float vidH = ImGui::GetContentRegionAvail().y * 0.7f;
    ImGui::BeginChild("VideoFrame", ImVec2(0, vidH), true);
    ImGui::Columns(2, "VideoColumns", true);
    
    ImGui::Text("Original");
    if (state.texOriginal) {
        float colW = ImGui::GetColumnWidth();
        ImGui::Image((ImTextureID)(intptr_t)state.texOriginal, ImVec2(colW - 10, (colW - 10) * (1944.f / 2592.f)));
    }
    
    ImGui::NextColumn();
    ImGui::Text("Result");
    if (state.texResult) {
        float colW = ImGui::GetColumnWidth();
        ImGui::Image((ImTextureID)(intptr_t)state.texResult, ImVec2(colW - 10, (colW - 10) * (720.f / 1280.f)));
        
        // Mouse Interaction
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            ImVec2 d = io.MouseDelta;
            if (state.moilMode == 1) {
                state.any_alpha -= d.y * 0.2f;
                state.any_beta -= d.x * 0.2f;
                state.moilParamsChanged = true;
            } else if (state.moilMode == 2) {
                state.pano_alpha -= d.y * 0.2f;
                state.pano_beta -= d.x * 0.2f;
                state.moilParamsChanged = true;
            }
        }
    }
    
    ImGui::Columns(1);
    ImGui::EndChild();
}
