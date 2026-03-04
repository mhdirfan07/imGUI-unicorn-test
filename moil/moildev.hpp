#ifndef MOILDEV_HPP
#define MOILDEV_HPP

#include <memory>
#include <string>
#include <vector>

struct IMoildevPlugin;

namespace moildev
{
    /**
     * @brief Backend type enumeration
     */
    enum class Backend
    {
        AUTO,   // Automatically select the best available backend
        CPU,    // Force CPU backend
        OCL     // Force OpenCL backend
    };

    /**
     * @brief Abstract interface for backend implementations
     *
     * This interface is used to provide a unified API for both
     * dynamically loaded (CPU and OpenCL) backends.
     */
    class IMoildevBackend
    {
    public:
        virtual ~IMoildevBackend() = default;

        virtual int AnyPointM(float *mapX, float *mapY, float alpha, float beta, float zoom) = 0;
        virtual int AnyPointM2(float *mapX, float *mapY, float alpha, float beta, float zoom) = 0;
        virtual int PanoramaTube(float *mapX, float *mapY, float alpha_min, float alpha_max) = 0;
        virtual int PanoramaCar(float *mapX, float *mapY, float my_p_alpha_max_vendor,
                               float p_iC_alpha_degree, float p_iC_beta_degree,
                               bool flip_h, bool flip_v) = 0;
        virtual int PanoramaRev(float *mapX, float *mapY, float alpha_max,
                               float alpha_degree, float beta_degree) = 0;
        virtual std::pair<int, int> PanoramaToOriginal(float panorama_x, float panorama_y, float alpha_max) = 0;

        virtual float getImageWidth() const = 0;
        virtual float getImageHeight() const = 0;
    };

    /**
     * @brief Unified Moildev wrapper class with dynamic backend selection
     *
     * This class provides a unified interface that automatically selects
     * the best available backend (CPU or OpenCL) at runtime using dynamic loading.
     * The OpenCL backend is loaded via dlopen/LoadLibrary, so the program will
     * run even if OpenCL libraries are not installed on the system.
     */
    class Moildev
    {
    public:
        /**
         * @brief Construct Moildev with automatic backend selection
         */
        Moildev(float cameraSensorWidth, float cameraSensorHeight,
                float icx, float icy, float ratio,
                float imageWidth, float imageHeight,
                float calibrationRatio,
                float para0, float para1, float para2,
                float para3, float para4, float para5,
                Backend backend = Backend::AUTO,
                int openCLDeviceID = 0);

        /**
         * @brief Destructor
         */
        ~Moildev();

        // Disable copy constructor and assignment operator
        Moildev(const Moildev&) = delete;
        Moildev& operator=(const Moildev&) = delete;

        // Enable move constructor and assignment operator
        Moildev(Moildev&&) noexcept;
        Moildev& operator=(Moildev&&) noexcept;

        /**
         * @brief Get the currently active backend
         */
        Backend getBackend() const { return activeBackend; }

        /**
         * @brief Get backend name as string
         */
        std::string getBackendName() const;

        /**
         * @brief Check if OpenCL backend library is available for dynamic loading
         */
        static bool isOpenCLAvailable();

        /**
         * @brief Check if CPU backend library is available for dynamic loading
         */
        static bool isCPUAvailable();

        /**
         * @brief Get list of available OpenCL devices
         * @return Vector of device names (empty if OpenCL is not available)
         */
        static std::vector<std::string> getOpenCLDevices();

        // Map generation methods
        int AnyPointM(float *mapX, float *mapY, float alpha, float beta, float zoom);
        int AnyPointM2(float *mapX, float *mapY, float alpha, float beta, float zoom);
        int PanoramaTube(float *mapX, float *mapY, float alpha_min, float alpha_max);
        int PanoramaCar(float *mapX, float *mapY, float my_p_alpha_max_vendor,
                       float p_iC_alpha_degree, float p_iC_beta_degree,
                       bool flip_h = false, bool flip_v = false);
        int PanoramaRev(float *mapX, float *mapY, float alpha_max,
                       float alpha_degree, float beta_degree);
        std::pair<int, int> PanoramaToOriginal(float panorama_x, float panorama_y, float alpha_max);

        // Getter methods
        float getImageWidth() const;
        float getImageHeight() const;

    private:
        std::unique_ptr<IMoildevBackend> backend;
        Backend activeBackend;

        void* oclLibHandle; // Handle for dynamically loaded OpenCL backend library
        void* cpuLibHandle; // Handle for dynamically loaded CPU backend library
        IMoildevPlugin* oclPlugin;
        IMoildevPlugin* cpuPlugin;

        // Stored parameters for fallback re-initialization
        struct InitParams {
            float cameraSensorWidth;
            float cameraSensorHeight;
            float icx;
            float icy;
            float ratio;
            float imageWidth;
            float imageHeight;
            float calibrationRatio;
            float para0;
            float para1;
            float para2;
            float para3;
            float para4;
            float para5;
        } m_params;
        int m_openCLDeviceID;

        void switchToCPU();

        void initializeBackend(float cameraSensorWidth, float cameraSensorHeight,
                              float icx, float icy, float ratio,
                              float imageWidth, float imageHeight,
                              float calibrationRatio,
                              float para0, float para1, float para2,
                              float para3, float para4, float para5,
                              Backend backendChoice,
                              int openCLDeviceID);

        std::unique_ptr<IMoildevBackend> createCPUBackend(
            float cameraSensorWidth, float cameraSensorHeight,
            float icx, float icy, float ratio,
            float imageWidth, float imageHeight,
            float calibrationRatio,
            float para0, float para1, float para2,
            float para3, float para4, float para5);

        std::unique_ptr<IMoildevBackend> createOpenCLBackend(
            float cameraSensorWidth, float cameraSensorHeight,
            float icx, float icy, float ratio,
            float imageWidth, float imageHeight,
            float calibrationRatio,
            float para0, float para1, float para2,
            float para3, float para4, float para5,
            int openCLDeviceID);
    };

} // namespace moildev

#endif // MOILDEV_HPP
