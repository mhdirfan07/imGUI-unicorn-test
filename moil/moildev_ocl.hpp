#ifndef MOILDEV_OCL_HPP
#define MOILDEV_OCL_HPP

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "moildev_common.hpp"
#include <CL/opencl.hpp>

using namespace std;

namespace moildev::ocl
{

    class Moildev
    {
    public:
        Moildev(float cameraSensorWidth, float cameraSensorHeight,
                float icx, float icy, float ratio,
                float imageWidth, float imageHeight,
                float calibrationRatio,
                float para0, float para1, float para2,
                float para3, float para4, float para5,
                int openCLDeviceID = 0);
        ~Moildev();

        static std::vector<std::string> getOpenCLDevices();

        int AnyPointM(float *mapX, float *mapY, float alpha, float beta, float zoom);
        int AnyPointM2(float *mapX, float *mapY, float alpha, float beta, float zoom);
        int PanoramaTube(float *mapX, float *mapY, float alpha_min, float alpha_max);
        int PanoramaCar(float *mapX, float *mapY, float my_p_alpha_max_vendor, float p_iC_alpha_degree, float p_iC_beta_degree, bool flip_h = false, bool flip_v = false);
        int PanoramaRev(float *mapX, float *mapY, float alpha_max, float alpha_degree, float beta_degree);
        std::pair<int, int> PanoramaToOriginal(float panorama_x, float panorama_y, float alpha_max);

        float getImageWidth() const { return cameraParams.imageWidth; }
        float getImageHeight() const { return cameraParams.imageHeight; }

    private:
        int N;
        int openCLDeviceID;
        bool hostUnifiedMemory;
        std::vector<int> alphaRhoTable;
        std::vector<int> rhoAlphaTable;
        CameraParams cameraParams;

        cl::Context context;
        cl::Device device;
        cl::CommandQueue queue;
        cl::Program program;

        cl::Kernel kernel_anypointm;
        cl::Kernel kernel_anypointm2;
        cl::Kernel kernel_panoramatube;
        cl::Kernel kernel_panoramacar;
        cl::Kernel kernel_panoramarev;

        cl::Buffer dev_mapX;
        cl::Buffer dev_mapY;
        cl::Buffer dev_config;

        std::optional<cl::Buffer> dev_anypointm_params;
        std::optional<cl::Buffer> dev_anypointm2_params;
        std::optional<cl::Buffer> dev_panoramatube_params;
        std::optional<cl::Buffer> dev_panoramatube_precalc_alpha;
        std::optional<cl::Buffer> dev_panoramatube_precalc_beta;
        std::optional<cl::Buffer> dev_panoramacar_params;
        std::optional<cl::Buffer> dev_panoramacar_precalc_alpha;
        std::optional<cl::Buffer> dev_panoramacar_precalc_beta;
        std::optional<cl::Buffer> dev_panoramarev_params;
        std::optional<cl::Buffer> dev_panoramarev_rhoalphatab;
        std::optional<cl::Buffer> dev_panoramarev_panoMapX;
        std::optional<cl::Buffer> dev_panoramarev_panoMapY;

        void initOpenCL();
        void initBuffers();
        void initAlphaRhoTable();
        int getRhoFromAlpha(float alpha);
        float getAlphaFromRho(int rho);
    };
}

#endif // MOILDEV_OCL_HPP