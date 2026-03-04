#pragma once
#include <opencv2/opencv.hpp>
#include <GL/gl.h>

// ==========================================
// 3. GRAPHICS UTILS
// ==========================================
class GraphicsUtils {
public:
    static GLuint MatToTexture(const cv::Mat& mat);
};
