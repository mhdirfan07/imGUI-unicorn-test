#ifndef MOILDEV_CPU_C_API_H
#define MOILDEV_CPU_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #ifdef BUILDING_MOILDEV_CPU
        #define MOILDEV_CPU_API __declspec(dllexport)
    #else
        #define MOILDEV_CPU_API __declspec(dllimport)
    #endif
#else
    #define MOILDEV_CPU_API __attribute__((visibility("default")))
#endif



/**
 * @brief Create a new Moildev CPU instance
 * @return Opaque pointer to the Moildev instance, or NULL on failure
 */
MOILDEV_CPU_API void* moildev_cpu_create(
    float cameraSensorWidth, float cameraSensorHeight,
    float icx, float icy, float ratio,
    float imageWidth, float imageHeight,
    float calibrationRatio,
    float para0, float para1, float para2,
    float para3, float para4, float para5
);

/**
 * @brief Destroy a Moildev CPU instance
 * @param instance Opaque pointer to the Moildev instance
 */
MOILDEV_CPU_API void moildev_cpu_destroy(void* instance);

/**
 * @brief Generate AnyPointM map
 */
MOILDEV_CPU_API int moildev_cpu_anypointm(
    void* instance,
    float* mapX, float* mapY,
    float alpha, float beta, float zoom
);

/**
 * @brief Generate AnyPointM2 map
 */
MOILDEV_CPU_API int moildev_cpu_anypointm2(
    void* instance,
    float* mapX, float* mapY,
    float alpha, float beta, float zoom
);

/**
 * @brief Generate PanoramaTube map
 */
MOILDEV_CPU_API int moildev_cpu_panoramatube(
    void* instance,
    float* mapX, float* mapY,
    float alpha_min, float alpha_max
);

/**
 * @brief Generate PanoramaCar map
 */
MOILDEV_CPU_API int moildev_cpu_panoramacar(
    void* instance,
    float* mapX, float* mapY,
    float my_p_alpha_max_vendor,
    float p_iC_alpha_degree,
    float p_iC_beta_degree,
    bool flip_h, bool flip_v
);

/**
 * @brief Generate PanoramaRev map
 */
MOILDEV_CPU_API int moildev_cpu_panoramarev(
    void* instance,
    float* mapX, float* mapY,
    float alpha_max,
    float alpha_degree,
    float beta_degree
);

/**
 * @brief Convert panorama coordinates to original fisheye coordinates
 * @return 0 on success, -1 on error
 */
MOILDEV_CPU_API int moildev_cpu_panoramatooriginal(
    void* instance,
    float panorama_x, 
    float panorama_y, 
    float alpha_max,
    int* out_origX,
    int* out_origY
);  

/**
 * @brief Get image width
 */
MOILDEV_CPU_API float moildev_cpu_get_image_width(void* instance);

/**
 * @brief Get image height
 */
MOILDEV_CPU_API float moildev_cpu_get_image_height(void* instance);

#ifdef __cplusplus
}
#endif

#endif // MOILDEV_CPU_C_API_H
