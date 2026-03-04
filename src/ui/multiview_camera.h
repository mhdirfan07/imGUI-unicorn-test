#pragma once
#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <memory>
#include "moil/moildev_ocl.hpp"

// ==========================================
// MULTI-VIEW CAMERA STATE & LOGIC
// ==========================================
struct MultiViewState {
    int currentCamIndex = 0;
    int selectedCamIndex = 0;
    cv::VideoCapture cap;
    cv::Mat frame, sensorFrame;
    
    // Panorama view
    cv::Mat panoramaFrame;
    GLuint texPanorama = 0;
    cv::Mat mapXPano, mapYPano;
    float pano_alpha_max = 110.f;
    float pano_alpha = 0.f;
    float pano_beta = 0.f;
    bool panoCached = false;
    float lastPanoAlpha = -999, lastPanoBeta = -999, lastPanoAlphaMax = -999;
    
    // Anypoint views (3x)
    cv::Mat anypointFrame[3];
    GLuint texAnypoint[3] = {0, 0, 0};
    cv::Mat mapXAny[3], mapYAny[3];
    float any_alpha[3] = {0.f, 45.f, -45.f};
    float any_beta[3] = {0.f, 90.f, -90.f};
    float any_zoom[3] = {4.f, 4.f, 4.f};
    bool anyPointCached[3] = {false, false, false};
    float lastAnyAlpha[3] = {-999, -999, -999};
    float lastAnyBeta[3] = {-999, -999, -999};
    float lastAnyZoom[3] = {-999, -999, -999};
    
    bool isFirstRun = true;
    bool isMoilInit = false;
    std::unique_ptr<moildev::ocl::Moildev> moil;
    
    int frameCounter = 0;
    int frameSkipCounter = 0;
    int frameSkip = 1;  // Process every frame for better responsiveness
    bool applyCanny = false;
};

class MultiViewCamera {
public:
    MultiViewCamera();
    ~MultiViewCamera();
    
    void Initialize();
    void Update();
    void Render();
    
    MultiViewState& GetState() { return state; }
    
private:
    MultiViewState state;
    
    void InitializeMoildev();
    void UpdateCamera();
    void ProcessFrame();
    void UpdatePanoramaView();
    void UpdateAnypointViews();
};
