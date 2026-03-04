#ifndef MOILDEV_CPU_HPP
#define MOILDEV_CPU_HPP

#include "moildev_common.hpp"
#include <string>


using namespace std;

namespace moildev::cpu
{
    class Moildev
    {
    public:
        Moildev(float cameraSensorWidth, float cameraSensorHeight,
                float icx, float icy, float ratio,
                float imageWidth, float imageHeight,
                float calibrationRatio,
                float para0, float para1, float para2,
                float para3, float para4, float para5);
        ~Moildev();

        int AnyPointM(float *mapX, float *mapY, float alpha, float beta, float zoom);
        int AnyPointM2(float *mapX, float *mapY, float alpha, float beta, float zoom);
        int PanoramaTube(float *mapX, float *mapY, float alpha_min, float alpha_max);
        int PanoramaCar(float *mapX, float *mapY, float my_p_alpha_max_vendor, float p_iC_alpha_degree, float p_iC_beta_degree, bool flip_h = false, bool flip_v = false);
        int PanoramaRev(float *mapX, float *mapY, float alpha_max, float alpha_degree, float beta_degree);
        std::pair<int, int> PanoramaToOriginal(float panorama_x, float panorama_y, float alpha_max);

        float getImageWidth() const { return cameraParams.imageWidth; }
        float getImageHeight() const { return cameraParams.imageHeight; }

    private:
        std::vector<int> alphaRhoTable;
        std::vector<int> rhoAlphaTable;
        CameraParams cameraParams;
        void initAlphaRhoTable();
        int getRhoFromAlpha(float alpha);
        float getAlphaFromRho(int rho);
    };
}

#endif // MOILDEV_CPU_HPP