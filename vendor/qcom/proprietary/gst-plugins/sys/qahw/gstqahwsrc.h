/*
*Copyright (c) 2017 Qualcomm Technologies, Inc.
*All Rights Reserved.
*Confidential and Proprietary - Qualcomm Technologies, Inc.
 */


#ifndef __GST_QAHWSRC_H__
#define __GST_QAHWSRC_H__

#include <gst/audio/audio.h>
#include <mm-audio/qahw_api/inc/qahw_api.h>
#include <mm-audio/qahw_api/inc/qahw_defs.h>

G_BEGIN_DECLS

#define GST_TYPE_QAHW_SRC            (gst_qahwsrc_get_type())
#define GST_QAHW_SRC(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_QAHW_SRC,GstQahwSrc))
#define GST_QAHW_SRC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_QAHW_SRC,GstQahwSrcClass))
#define GST_IS_QAHW_SRC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_QAHW_SRC))
#define GST_IS_QAHW_SRC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_QAHW_SRC))
#define GST_QAHW_SRC_CAST(obj)       ((GstQahwSrc *)(obj))

typedef enum
{
  GST_QAHWSRC_INPUT_DEVICE_COMMUNICATION = AUDIO_DEVICE_IN_COMMUNICATION,
  GST_QAHWSRC_INPUT_DEVICE_AMBIENT = AUDIO_DEVICE_IN_AMBIENT,
  GST_QAHWSRC_INPUT_DEVICE_BUILTIN_MIC = AUDIO_DEVICE_IN_BUILTIN_MIC,
  GST_QAHWSRC_INPUT_DEVICE_BLUETOOTH_SCO_HEADSET = AUDIO_DEVICE_IN_BLUETOOTH_SCO_HEADSET,
  GST_QAHWSRC_INPUT_DEVICE_WIRED_HEADSET = AUDIO_DEVICE_IN_WIRED_HEADSET,
  GST_QAHWSRC_INPUT_DEVICE_AUX_DIGITAL = AUDIO_DEVICE_IN_AUX_DIGITAL,
  GST_QAHWSRC_INPUT_DEVICE_HDMI = AUDIO_DEVICE_IN_HDMI,
  GST_QAHWSRC_INPUT_DEVICE_VOICE_CALL = AUDIO_DEVICE_IN_VOICE_CALL,
  GST_QAHWSRC_INPUT_DEVICE_TELEPHONY_RX = AUDIO_DEVICE_IN_TELEPHONY_RX,
  GST_QAHWSRC_INPUT_DEVICE_BACK_MIC = AUDIO_DEVICE_IN_BACK_MIC,
  GST_QAHWSRC_INPUT_DEVICE_REMOTE_SUBMIX = AUDIO_DEVICE_IN_REMOTE_SUBMIX,
  GST_QAHWSRC_INPUT_DEVICE_ANALOG_DOCK_HEADSET = AUDIO_DEVICE_IN_ANLG_DOCK_HEADSET,
  GST_QAHWSRC_INPUT_DEVICE_DIGITAL_DOCK_HEADSET = AUDIO_DEVICE_IN_DGTL_DOCK_HEADSET,
  GST_QAHWSRC_INPUT_DEVICE_USB_ACCESSORY = AUDIO_DEVICE_IN_USB_ACCESSORY,
  GST_QAHWSRC_INPUT_DEVICE_USB_DEVICE = AUDIO_DEVICE_IN_USB_DEVICE,
  GST_QAHWSRC_INPUT_DEVICE_FM_TUNER = AUDIO_DEVICE_IN_FM_TUNER,
  GST_QAHWSRC_INPUT_DEVICE_TV_TUNER = AUDIO_DEVICE_IN_TV_TUNER,
  GST_QAHWSRC_INPUT_DEVICE_LINE = AUDIO_DEVICE_IN_LINE,
  GST_QAHWSRC_INPUT_DEVICE_SPDIF = AUDIO_DEVICE_IN_SPDIF,
  GST_QAHWSRC_INPUT_DEVICE_BLUETOOTH_A2DP = AUDIO_DEVICE_IN_BLUETOOTH_A2DP,
  GST_QAHWSRC_INPUT_DEVICE_LOOPBACK = AUDIO_DEVICE_IN_LOOPBACK,
  GST_QAHWSRC_INPUT_DEVICE_IP = AUDIO_DEVICE_IN_IP,
  GST_QAHWSRC_INPUT_DEVICE_PROXY = AUDIO_DEVICE_IN_PROXY,
  GST_QAHWSRC_INPUT_DEVICE_DEFAULT = AUDIO_DEVICE_IN_DEFAULT,
} GstQAHWSrcInputDevice;

#define GST_TYPE_QAHWSRC_INPUT_DEVICE (gst_qahwsrc_input_device_get_type ())

typedef enum
{
  GST_QAHWSRC_AUDIO_SOURCE_DEFAULT = AUDIO_SOURCE_DEFAULT,
  GST_QAHWSRC_AUDIO_SOURCE_MIC = AUDIO_SOURCE_MIC,
  GST_QAHWSRC_AUDIO_SOURCE_VOICE_UPLINK = AUDIO_SOURCE_VOICE_UPLINK,
  GST_QAHWSRC_AUDIO_SOURCE_VOICE_DOWNLINK = AUDIO_SOURCE_VOICE_DOWNLINK,
  GST_QAHWSRC_AUDIO_SOURCE_VOICE_CALL = AUDIO_SOURCE_VOICE_CALL,
  GST_QAHWSRC_AUDIO_SOURCE_CAMCORDER = AUDIO_SOURCE_CAMCORDER,
  GST_QAHWSRC_AUDIO_SOURCE_VOICE_RECOGNITION = AUDIO_SOURCE_VOICE_RECOGNITION,
  GST_QAHWSRC_AUDIO_SOURCE_VOICE_COMMUNICATION = AUDIO_SOURCE_VOICE_COMMUNICATION,
  GST_QAHWSRC_AUDIO_SOURCE_REMOTE_SUBMIX = AUDIO_SOURCE_REMOTE_SUBMIX,
  GST_QAHWSRC_AUDIO_SOURCE_UNPROCESSED = AUDIO_SOURCE_UNPROCESSED,
  GST_QAHWSRC_AUDIO_SOURCE_FM_TUNER = AUDIO_SOURCE_FM_TUNER,
  GST_QAHWSRC_AUDIO_SOURCE_HOTWORD = AUDIO_SOURCE_HOTWORD,
} GstQAHWSrcAudioSource;

#define GST_TYPE_QAHWSRC_AUDIO_SOURCE (gst_qahwsrc_audio_source_get_type ())

typedef enum {
  GST_AUDIO_INPUT_FLAG_NONE          = AUDIO_INPUT_FLAG_NONE,
  GST_AUDIO_INPUT_FLAG_FAST          = AUDIO_INPUT_FLAG_FAST,
  GST_AUDIO_INPUT_FLAG_HW_HOTWORD    = AUDIO_INPUT_FLAG_HW_HOTWORD,
  GST_AUDIO_INPUT_FLAG_RAW           = AUDIO_INPUT_FLAG_RAW,
  GST_AUDIO_INPUT_FLAG_SYNC          = AUDIO_INPUT_FLAG_SYNC,
} GstAudioInputFlags;

#define GST_TYPE_AUDIO_INPUT_FLAGS (gst_audio_input_flags_get_type())
GType gst_audio_input_flags_get_type (void);


typedef struct _GstQahwSrc GstQahwSrc;
typedef struct _GstQahwSrcClass GstQahwSrcClass;

/**
 * GstQahwSrc:
 *
 * Opaque data structure
 */
struct _GstQahwSrc {
  GstAudioSrc           src;

  gchar                 *module_id;
  gchar                 *stream_profile;

  qahw_module_handle_t  *module;
  qahw_stream_handle_t  *stream;
  GstQAHWSrcInputDevice input_device;
  GstQAHWSrcAudioSource audio_source;
  gchar                 *device_address;
  audio_input_flags_t   audio_input_flags;
  audio_config_t        config;
  gint                  audio_handle;
  gboolean              kpi_mode;
  gint                  ffv_state;
  gint                  ffv_ec_ref_dev;
  gint                  ffv_channel_index;

  gint                  rate;
  gint                  channels;
};

struct _GstQahwSrcClass {
  GstAudioSrcClass parent_class;

  gchar* (*get_parameters)  (GstQahwSrc *qahw, gchar *request);

};

GType gst_qahwsrc_get_type(void);

G_END_DECLS

#endif /* __GST_QAHWSRC_H__ */
