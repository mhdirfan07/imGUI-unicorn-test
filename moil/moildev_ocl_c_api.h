#ifndef MOILDEV_OCL_C_API_H
#define MOILDEV_OCL_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #ifdef BUILDING_MOILDEV_OCL
        #define MOILDEV_OCL_API __declspec(dllexport)
    #else
        #define MOILDEV_OCL_API __declspec(dllimport)
    #endif
#else
    #define MOILDEV_OCL_API __attribute__((visibility("default")))
#endif



/**
 * @brief Create a new Moildev OpenCL instance
 * @return Opaque pointer to the Moildev instance, or NULL on failure
 */
MOILDEV_OCL_API void* moildev_ocl_create(
    float cameraSensorWidth, float cameraSensorHeight,
    float icx, float icy, float ratio,
    float imageWidth, float imageHeight,
    float calibrationRatio,
    float para0, float para1, float para2,
    float para3, float para4, float para5,
    int openCLDeviceID
);

/**
 * @brief Destroy a Moildev OpenCL instance
 * @param instance Opaque pointer to the Moildev instance
 */
MOILDEV_OCL_API void moildev_ocl_destroy(void* instance);

/**
 * @brief Generate AnyPointM map
 */
MOILDEV_OCL_API int moildev_ocl_anypointm(
    void* instance,
    float* mapX, float* mapY,
    float alpha, float beta, float zoom
);

/**
 * @brief Generate AnyPointM2 map
 */
MOILDEV_OCL_API int moildev_ocl_anypointm2(
    void* instance,
    float* mapX, float* mapY,
    float alpha, float beta, float zoom
);

/**
 * @brief Generate PanoramaTube map
 */
MOILDEV_OCL_API int moildev_ocl_panoramatube(
    void* instance,
    float* mapX, float* mapY,
    float alpha_min, float alpha_max
);

/**
 * @brief Generate PanoramaCar map
 */
MOILDEV_OCL_API int moildev_ocl_panoramacar(
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
MOILDEV_OCL_API int moildev_ocl_panoramarev(
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
MOILDEV_OCL_API int moildev_ocl_panoramatooriginal(
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
MOILDEV_OCL_API float moildev_ocl_get_image_width(void* instance);

/**
 * @brief Get image height
 */
MOILDEV_OCL_API float moildev_ocl_get_image_height(void* instance);

#ifdef __cplusplus
}
#endif

#endif // MOILDEV_OCL_C_API_H
