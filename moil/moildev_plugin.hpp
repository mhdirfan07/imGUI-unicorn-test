#ifndef MOILDEV_PLUGIN_HPP
#define MOILDEV_PLUGIN_HPP

#include <vector>
#include <string>

#ifdef _WIN32
    #ifdef BUILDING_MOILDEV_PLUGIN
        #define MOILDEV_PLUGIN_API __declspec(dllexport)
    #else
        #define MOILDEV_PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define MOILDEV_PLUGIN_API __attribute__((visibility("default")))
#endif

/**
 * @brief Plugin interface for Moildev backends
 *
 * Each backend (CPU, OpenCL) implements this interface and exports
 * moildev_create_plugin() and moildev_destroy_plugin() functions.
 */
struct IMoildevPlugin {
    // Instance lifecycle
    virtual void* create(
        float cameraSensorWidth, float cameraSensorHeight,
        float icx, float icy, float ratio,
        float imageWidth, float imageHeight,
        float calibrationRatio,
        float para0, float para1, float para2,
        float para3, float para4, float para5,
        int openCLDeviceID = 0) = 0;
    virtual void destroy(void* instance) = 0;

    // Map generation functions
    virtual int AnyPointM(void* instance, float* mapX, float* mapY,
                          float alpha, float beta, float zoom) = 0;
    virtual int AnyPointM2(void* instance, float* mapX, float* mapY,
                           float alpha, float beta, float zoom) = 0;
    virtual int PanoramaTube(void* instance, float* mapX, float* mapY,
                             float alpha_min, float alpha_max) = 0;
    virtual int PanoramaCar(void* instance, float* mapX, float* mapY,
                            float p_alpha_max, float p_iC_alpha, float p_iC_beta,
                            bool flip_h, bool flip_v) = 0;
    virtual int PanoramaRev(void* instance, float* mapX, float* mapY,
                            float alpha_max, float alpha_degree, float beta_degree) = 0;
    virtual std::pair<int, int> PanoramaToOriginal(void* instance, float panorama_x, float panorama_y,
                                                   float alpha_max) = 0;

    // Getters
    virtual float getImageWidth(void* instance) const = 0;
    virtual float getImageHeight(void* instance) const = 0;

    // OpenCL specific (optional - default implementations for CPU)
    virtual std::vector<std::string> getOpenCLDevices() { return {}; }

    virtual ~IMoildevPlugin() = default;
};

// Plugin entry points - only these 2 functions need extern "C"
extern "C" MOILDEV_PLUGIN_API IMoildevPlugin* moildev_create_plugin();
extern "C" MOILDEV_PLUGIN_API void moildev_destroy_plugin(IMoildevPlugin*);

#endif // MOILDEV_PLUGIN_HPP
