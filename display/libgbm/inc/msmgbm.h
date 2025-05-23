//**************************************************************************************************
// Copyright (c) 2017 Qualcomm Technologies, Inc.
// All Rights Reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//**************************************************************************************************

#ifndef _MSM_GBM_H_
#define _MSM_GBM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

//Enables debug log
#ifdef GBM_DEBUG
    #define  GBM_DBG_LOG fprintf
#else
    #define  GBM_DBG_LOG(...) {}
#endif

#define LOG_FATAL    (1)
#define LOG_ERR      (2)
#define LOG_WARN     (3)
#define LOG_INFO     (4)
#define LOG_DBG      (5)

extern int  g_debug_level;

#define LOG(level, ...) do {  \
                            if (level <= g_debug_level) { \
                                if(level==LOG_FATAL) \
                                  fprintf(stderr,"%s","GBM_FATAL::"); \
                                if(level==LOG_ERR) \
                                  fprintf(stderr,"%s", "GBM_ERR::"); \
                                if(level==LOG_WARN) \
                                  fprintf(stderr,"%s", "GBM_WARN::"); \
                                if(level==LOG_INFO) \
                                  fprintf(stderr,"%s", "GBM_INFO::"); \
                                if(level==LOG_DBG) \
                                  fprintf(stderr,"%s", "GBM_DBG::"); \
                                fprintf(stderr,"%s(%d)::",__func__,__LINE__); \
                                fprintf(stderr, __VA_ARGS__); \
                                fprintf(stderr, "\n"); \
                                fflush(stderr); \
                            } \
                        } while (0)




/*Currently three buffer mechanism is used for GPU rendering and Hardware Composition sync*/
#define NUM_BACK_BUFFERS 3
#define MAX_NUM_PLANES 3
#define QCMAGIC 0XDFDFECBA // need to compe up with something unique



/**
* State representation of the gbm buffer object
*
*/
#define GBM_BO_STATE_FREE                       (0x0) /* Unused/Free state of the Buffer Object */

#define GBM_BO_STATE_INUSE_BY_COMPOSITOR        (0x1) /* Buffer Object locked for composition */

#define GBM_BO_STATE_INUSE_BY_GPU               (0x2) /* Buffer Object locked for GPU rendering */

#define GBM_BO_STATE_NEW_FRONT_BUFFER           (0x3) /* Buffer Object currently available for
                                                   * grab by GPU or Compositor */


/**
* State representation of the gbm buffer object during
* gbm surface creation if enabled
*
*/

#ifndef ALLOCATE_SURFACE_BO_AT_CREATION

#define SURFACE_BOSLOT_STATE_FREE                    (0x0) /* Unused/Free state of the Buffer Object */

#define SURFACE_BOSLOT_STATE_INUSE_BY_COMPOSITOR     (0x1) /* BO locked for composition */

#define SURFACE_BOSLOT_STATE_HAS_NEW_FRONT_BUFFER    (0x2) /* BO currently available for
                                                        * grab by GPU or Compositor */
#endif


/**
 * msm gbm device object corresponding to the gbm device object instance
 *
 * The members in this structure should not be accessed directly.
 */

struct msmgbm_device {
   struct gbm_device base;
   uint32_t magic;
   int fd;  /* device fd */
   int iondev_fd;  /* ION device fd */
};

struct msmgbm_surface {
   struct gbm_surface base;
   uint32_t magic;
   struct msmgbm_device* device;
   struct msmgbm_bo *bo[NUM_BACK_BUFFERS];
#ifndef ALLOCATE_SURFACE_BO_AT_CREATION
   int bo_slot[NUM_BACK_BUFFERS];
#endif
};

/**
 * The allocated local msm buffer object for the corresponding gbm buffer object.
 *
 * The members in this structure should not be accessed directly.
 */
struct msmgbm_bo {
   struct gbm_bo base;
   uint32_t magic;
   struct msmgbm_device* device;
   uint32_t numplanes;
   size_t offset;
   uint32_t name;
   uint64_t gpuaddr;
   void* cpuaddr;
   void* mt_cpuaddr;
   size_t size;
   size_t mt_size;
   uint32_t current_state;
   uint32_t fbid;
   int ion_handle;
   int ion_mt_handle;
   int import_flg;
};


/**
 * Buffer Descriptor used for param passing between API's
 *
 */
struct gbm_bufdesc {
    uint32_t Width;
    uint32_t Height;
    uint32_t Format;
    uint32_t Usage;
};

struct gbm_buf_resource {
  struct gbm_buf_info *buffer_info;
  uint32_t magic;
};

/*
* Function prototypes of all supported API's by backend
*
*/

/** create the gbm bo
 *
 * \param handle to gbm device
 * \param width
 * \param height
 * \param format
 * \param usage flag
 * \return gbm bo handle
 *
 */
static struct gbm_bo *msmgbm_bo_create(struct gbm_device *gbm,uint32_t width,
                                   uint32_t height,uint32_t format, uint32_t usage);


/** Destroy the gbm bo
 *
 * \param bo The buffer object
 * \return None
 *
 */
static void msmgbm_bo_destroy(struct gbm_bo *bo);

/** To query gbm device pointer
 *
 * \param bo The buffer object
 * \return gbm device handle
 *
 */
static struct gbm_device* msmgbm_bo_get_device(struct gbm_bo *bo);

/** query gbm bo name
 *
 * \param gbm bo handle
 * \return id
 *
 */

int msmgbm_bo_get_name(struct gbm_bo* bo);


/** Perform write operation onto the gbm bo
 *
 * \param bo The buffer object
 * \param user buffer handle
 * \param byte count
 * \return =0 for success
 *         >0 for fail
 *
 */
static int msmgbm_bo_write(struct gbm_bo *bo, const void *buf, size_t count);

/** create the gbm bo
 *
 * \param import gbm bo handle depending on type
 * \param gbm device
 * \param type
 * \param void pointer
 * \param usage flag
 * \return gbm bo handle
 *
 */
struct gbm_bo * msmgbm_bo_import(struct gbm_device *gbm,uint32_t type,
                                                void *buffer, uint32_t usage);

/** Import gbm bo handle from fd
 *
 * \param handle to msmgbm_device
 * \param fd via void *
 * \param usage flags
 * \return gbm bo handle
 *
 */
struct gbm_bo * msmgbm_bo_import_fd(struct msmgbm_device *msm_dev,
                                                void *buffer, uint32_t usage);

/** Import gbm bo handle from name element of the wl buffer resource
 *
 * \param msmgbm_device handle
 * \param wl buffer handle
 * \param usage flag
 * \return gbm bo handle
 *
 */
struct gbm_bo * msmgbm_bo_import_wl_buffer(struct msmgbm_device *msm_dev,
                                                        void *buffer, uint32_t usage);

/** create the gbm bo
 *
 * \param msmgbm_device handle
 * \param egl buffer handle
 * \param usage flag
 * \return gbm bo handle
 *
 */
struct gbm_bo * msmgbm_bo_import_egl_image(struct msmgbm_device *msm_dev,
                                                        void *buffer, uint32_t usage);

/** create requested gbm surface with the dimensions
 *
 * \param gbm device handle
 * \param width
 * \param height
 * \param format
 * \param usage flag
 * \return gbm surface
 *
 */
static struct gbm_surface *
        msmgbm_surface_create(struct gbm_device *gbm,uint32_t width,
                                     uint32_t height,uint32_t format, uint32_t flags);

/** destroy gbm surface
 *
 * \param handle to gbm surface
 * \return None
 *
 */
static void msmgbm_surface_destroy(struct gbm_surface *surf);
/** Verifies if surface is allocated by MSM GBM backend
  *
  * \Returns GBM_ERROR values
  *
  */
static inline
int  msmgbm_validate_surface(struct gbm_surface *surf);
/** update the state of the current bo associated with the surface to used by compositor
 *
 * \param surface handle
 * \return gbm bo handle
 *
 */
static struct gbm_bo * msmgbm_surface_lock_front_buffer(struct gbm_surface *surf);
/** update the state of the current bo associated with the surface to free
 *
 * \param surface handle
 * \return gbm bo handle
 *
 */
static void msmgbm_surface_release_buffer(struct gbm_surface *surf,
                                                            struct gbm_bo *bo);
/* Returns bo which can be used as front buffer on success
  * \Returns GBM_ERROR values
  *
  */
struct gbm_bo* msmgbm_surface_get_free_bo(struct gbm_surface *surf);
/** update the state machine of a bo to be used as the surface front buffer
  * \Returns GBM_ERROR values
  *
  */
int msmgbm_surface_set_front_bo(struct gbm_surface *surf, struct gbm_bo *gbo);
/** Returns the CPU access address for the BO buffer
  * \Returns  CPU address on Success / Null on fail
  *
  */

void * msmgbm_bo_cpu_map(struct gbm_bo *gbo);
/* Unmap the BO buffer from CPU access
  * \Returns GBM_ERROR values
  *
  */
int msmgbm_bo_cpu_unmap(struct gbm_bo *gbo);
/*  Returns BO size on success
  * \Returns  size on Success/ GBM_ERROR Values on Failure
  *
  */
static inline
size_t msmgbm_bo_get_size(struct gbm_bo *bo);
/** create gbm device
 *
 * \param  display driver device fd
 * \return gbm device handle
 *
 */
static struct gbm_device * msmgbm_device_create(int fd);
/** destroy the gbm device handle
 *
 * \param gbm device handle
 * \return none
 *
 */
static void msmgbm_device_destroy(struct gbm_device *gbm);
/** To query gbm device fd
 *
 * \param bo The buffer object
 * \return fd
 *
 */
static int msmgbm_device_get_fd(struct gbm_bo *bo);
/** query device unique id
 *
 * \param handle to gbm device
 * \return ID
 *
 */
unsigned int msmgbm_device_get_magic(struct gbm_device *dev);
/** query drm device file name
 *
 * \return gbm bo handle
 *
 */
static inline
const char*  msmgbm_drm_device_get_name(void);
/** Authenticate the DRM magic ID
  *
  * \Returns 0 on success
  * \Returns -1 on failure
  *
  */
int  msmgbm_device_authenticate_magic(struct gbm_device *dev,
                                                            uint32_t magic);
/** Import a BO handle from a unique ID/name
  *
  * \Returns imported BO pointer on success
  * \Returns NULL on failure
  *
  */
struct gbm_bo* msmgbm_bo_import_from_name(struct gbm_device *dev,
                                                            unsigned int name);
/** Returns global name on success
  *
  * \Returns GBM_ERROR values on failure
  *
  */
int msmgbm_bo_get_name(struct gbm_bo* bo);

/** Verifies if device is allocated by MSM GBM backend
  *
  * \Returns GBM_ERROR values
  *
  */
static inline
int msmgbm_validate_device(struct gbm_device *dev);
/** Query metadata info depending on the operation/paramtype
  *
  * \Returns GBM_ERROR values
  *
  */
int msmgbm_get_metadata(struct gbm_bo *gbo, int paramType,void *param);
/** Set metadata info depending on the operation/paramtype
  *
  * \Returns GBM_ERROR values
  *
  */
int msmgbm_set_metadata(struct gbm_bo *gbo, int paramType,void *param);

/** Get RGB Data Address for a given gbm buffer object
  *
  * \Returns GBM_ERROR values
  *
  */
int msmgbm_get_rgb_data_address(struct gbm_bo *gbo, void **rgb_data);

#ifdef __cplusplus
}
#endif

#endif //_MSM_GBM_H_

