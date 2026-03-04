#pragma once
#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <memory>
#include "moil/moildev_ocl.hpp"

// ==========================================
// CAMERA VIEW STATE & LOGIC
// ==========================================
struct CameraState {
    int currentCamIndex = 0;
    int selectedCamIndex = 0;
    cv::VideoCapture cap;
    cv::Mat frame, moilFrame, processedFrame;
    GLuint texOriginal = 0, texResult = 0;
    bool isFirstRun = true;
    bool isMoilInit = false;
    std::unique_ptr<moildev::ocl::Moildev> moil;
    cv::Mat mapX, mapY;
    int moilMode = 0;
    bool moilParamsChanged = true;
    
    // MOILDEV PARAMETERS
    float any_alpha = 0.f, any_beta = 0.f, any_zoom = 4.f;
    float pano_alpha_max = 110.f, pano_alpha = 0.f, pano_beta = 0.f;
    
    // PROCESSING
    bool applyCanny = false;
    int frameCounter = 0;
};

class CameraView {
public:
    CameraView();
    ~CameraView();
    
    void Initialize();
    void Update();
    void Render();
    
    CameraState& GetState() { return state; }
    
private:
    CameraState state;
    
    void InitializeMoildev();
    void UpdateCamera();
    void ProcessFrame();
};
