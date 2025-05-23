/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef GEOFENCE_H
#define GEOFENCE_H

#include <stdint.h>
#include <stdbool.h>

#include <platform_lib_log_util.h>
#include "loc_log.h"

typedef enum {
    ADD_GEOFENCE,
    REMOVE_GEOFENCE,
    PAUSE_GEOFENCE,
    RESUME_GEOFENCE,
    MODIFY_GEOFENCE
} GeofenceResp;

typedef struct {
    void* client;
    uint32_t afwId;
    uint32_t breachMask;
    uint8_t confidence;
    uint32_t responsiveness;
    double latitude;
    double longitude;
    double radius;
    bool paused;
    uint32_t dwellTime; // in seconds
    uint8_t dwellTimeMask;
}GeoFenceData;

typedef struct {
    int last_transition;
    int monitor_transitions;
    int notification_responsivenes_ms;
    int unknown_timer_ms;
    uint32_t sources_to_use;
} GeofenceExtOptions;

#define GEOFENCE_BREACH_ENTERED            (1<<0L)
#define GEOFENCE_BREACH_EXITED             (1<<1L)
#define GEOFENCE_BREACH_UNCERTAIN          (1<<2L)
#define GEOFENCE_DWELL_INSIDE_REPORT       (1<<3L)
#define GEOFENCE_DWELL_OUTSIDE_REPORT      (1<<4L)

#define GF_DWELL_TYPE_INSIDE       1
#define GF_DWELL_TYPE_OUTSIDE      2

#define GF_CONFIDENCE_LOW          1
#define GF_CONFIDENCE_MED          2
#define GF_CONFIDENCE_HIGH         3

//These should match the LOW, MEDIUM, HIGH values in flp.conf and izat.conf
#define GF_NOTIF_RESPONSIVENESS_LOW       1
#define GF_NOTIF_RESPONSIVENESS_MEDIUM    2
#define GF_NOTIF_RESPONSIVENESS_HIGH      3

/*Changing these time values will have no effect until the
  pre-built binaries are re-compiled*/
#define GF_NOTIF_RESPONSIVENESS_MSEC_LOW      1200000 //20 mins
#define GF_NOTIF_RESPONSIVENESS_MSEC_MEDIUM   300000  //5 mins
#define GF_NOTIF_RESPONSIVENESS_MSEC_HIGH     5000    //5 secs

#define GF_RESPONSIVENESS_THRESHOLD_MSEC_HIGH   120000 //2 mins
#define GF_RESPONSIVENESS_THRESHOLD_MSEC_MEDIUM 900000 //15 mins

#endif /* GEOFENCE_H */
