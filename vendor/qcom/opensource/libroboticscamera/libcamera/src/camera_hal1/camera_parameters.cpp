/* Copyright (c) 2015-2017, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "camera/CameraParameters.h"
#include "camera_parameters.h"
#include "camera_log.h"
#include "qcamera_extensions.h"

#include <string>
#include <vector>
#include <cstdlib>
#include <errno.h>
#include <sstream>

using namespace std;
using namespace android;

namespace camera
{

static const int VALUE_SIZE_MAX = 32;
/* helper function to cast the private params to CameraParameters */
inline static CameraParameters* params_cast(void *priv)
{
    return static_cast<CameraParameters*> (priv);
}

/**
 * parses the comma separated values from a string and returns a
 * vector of individual strings
 *
 * @param valueStr [in] comma separated value string
 *
 * @return vector<string>
 */
static vector<string> parseCSV(const char *valueStr)
{
    vector<string> values;
    CAM_DBG("valueStr = %s", valueStr);

    string input(valueStr);
    istringstream ss(input);
    string token;
    while(getline(ss, token, ',')) {
        values.push_back(token);
    }
    return values;
}

/**
 * parses text within paranthesis and sends a vector of all such
 * text found.
 * For example with input  "(10, 20), (15, 30)", it returns
 * vector with elements "10, 20" and "15, 30"
 *
 * @param valueStr
 *
 * @return vector<string>
 */
static vector<string> parseParanthesis(const char *valueStr)
{
    vector<string> values;
    char *p = (char *) valueStr;
    char buf[VALUE_SIZE_MAX];
    int idx = 0;

    while (true) {
        idx = 0;
        /* find opening paranthesis */
        while (*p != '(' && *p != '\0') {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        p++;
        /* find closing parenthesis */
        while (*p != ')' && *p != '\0') {
            buf[idx] = *p;
            idx++;
            p++;
        }
        if (*p == '\0') {
            CAM_ERR("parse error: valueStr=%s", valueStr);
            break;
        }
        buf[idx] = '\0';
        string str(buf);
        values.push_back(str);
    }
    return values;
}

/**
 * convert hfr string value to corresponding VideoFPS enum value
 */
inline static const char *toHFRMode(VideoFPS fps)
{
    if (fps == VIDEO_FPS_1X) {
        return VIDEO_HFR_OFF;
    }
    if (fps == VIDEO_FPS_2X) {
        return VIDEO_HFR_2X;
    }
    if (fps == VIDEO_FPS_3X) {
        return VIDEO_HFR_3X;
    }
    if (fps == VIDEO_FPS_4X) {
        return VIDEO_HFR_4X;
    }
    if (fps == VIDEO_FPS_5X) {
        return VIDEO_HFR_5X;
    }
    return VIDEO_HFR_OFF;
}

/**
 * convert hfr string value to corresponding VideoFPS enum value
 */
inline static VideoFPS toVideoFPS(string hfrMode)
{
    if (hfrMode == VIDEO_HFR_OFF) {
        return VIDEO_FPS_1X;
    }
    if (hfrMode == VIDEO_HFR_2X) {
        return VIDEO_FPS_2X;
    }
    if (hfrMode == VIDEO_HFR_3X) {
        return VIDEO_FPS_3X;
    }
    if (hfrMode == VIDEO_HFR_4X) {
        return VIDEO_FPS_4X;
    }
    if (hfrMode == VIDEO_HFR_5X) {
        return VIDEO_FPS_5X;
    }
    return VIDEO_FPS_1X;
}

/* writes serialized version of parameters to ostream object
   provided by the caller*/
int CameraParams::writeObject(std::ostream& ps) const

{
    ps << params_cast(priv_)->flatten();
    return 0;
}


CameraParams::CameraParams()
{
    priv_ = (void *) new CameraParameters();
}

int CameraParams::init(ICameraDevice* device)
{
    int rc = 0;
    int paramBufSize = 0;
    uint8_t* paramBuf = 0;

    rc = device->getParameters(0, 0, &paramBufSize);
    if (0 != rc) {
        goto bail;
    }

    paramBuf = (uint8_t*)calloc(paramBufSize+1, 1);
    if (0 == paramBuf) {
        rc = ENOMEM;
        goto bail;
    }

    rc = device->getParameters(paramBuf, paramBufSize);
    if (0 != rc) {
        goto bail;
    }

    params_cast(priv_)->unflatten(String8((const char *)paramBuf));
    device_ = device;
bail:
   free(paramBuf);
   return rc;
}

CameraParams::~CameraParams()
{
    delete params_cast(priv_);
}

vector <ImageSize> CameraParams::getSupportedPreviewSizes() const
{
    Vector<Size> sizes;
    vector<ImageSize> imgSizes;
    params_cast(priv_)->getSupportedPreviewSizes(sizes);
    imgSizes.resize(sizes.size());

    for(vector<Size>::size_type i=0; i<sizes.size(); i++) {
        imgSizes[i].width = sizes[i].width;
        imgSizes[i].height = sizes[i].height;
    }
    return imgSizes;
}

void CameraParams::setPreviewSize(const ImageSize& size)
{
    params_cast(priv_)->setPreviewSize(size.width, size.height);
}

ImageSize CameraParams::getPreviewSize() const
{
    ImageSize size;
    params_cast(priv_)->getPreviewSize(&size.width, &size.height);
    return size;
}


vector<ImageSize> CameraParams::getSupportedVideoSizes() const
{
    Vector<Size> sizes;
    vector<ImageSize> imgSizes;
    params_cast(priv_)->getSupportedVideoSizes(sizes);
    imgSizes.resize(sizes.size());

    for(vector<Size>::size_type i=0; i<sizes.size(); i++) {
        imgSizes[i].width = sizes[i].width;
        imgSizes[i].height = sizes[i].height;
    }
    return imgSizes;
}

void CameraParams::setVideoSize(const ImageSize& size)
{
    params_cast(priv_)->setVideoSize(size.width, size.height);
}

ImageSize CameraParams::getVideoSize() const
{
    ImageSize size;
    params_cast(priv_)->getVideoSize(&size.width, &size.height);
    return size;
}

vector<ImageSize> CameraParams::getSupportedPictureSizes(std::string format) const
{
    Vector<Size> sizes;
    vector<ImageSize> imgSizes;
    const char *sizeStr =
        params_cast(priv_)->get(KEY_QC_SUPPORTED_LIVESNAPSHOT_SIZES);
    parseSizesList(sizeStr, sizes);
    imgSizes.resize(sizes.size());

    for(vector<Size>::size_type i=0; i<sizes.size(); i++) {
        imgSizes[i].width = sizes[i].width;
        imgSizes[i].height = sizes[i].height;
    }
    return imgSizes;
}

void CameraParams::setPictureSize(const ImageSize& size)
{
    params_cast(priv_)->setPictureSize(size.width, size.height);
}

ImageSize CameraParams::getPictureSize() const
{
    ImageSize size;
    params_cast(priv_)->getPictureSize(&size.width, &size.height);
    return size;
}

ImageSize CameraParams::getPictureThumbNailSize() const
{
    ImageSize size;
    const char *widthStr =
        params_cast(priv_)->get(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH);
    const char *heightStr =
        params_cast(priv_)->get(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT);

    size.width = atoi(widthStr);
    size.height = atoi(heightStr);
    return size;
}

void CameraParams::setPictureThumbNailSize(const ImageSize& size)
{
    if (size.width * 3 == size.height * 4) {
        params_cast(priv_)->set(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH, "320");
        params_cast(priv_)->set(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, "240");
    } else {
        params_cast(priv_)->set(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH, "512");
        params_cast(priv_)->set(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, "288");
    }
}

int CameraParams::commit()
{
    /* set the current state of paramters in camera device */
    return device_->setParameters(*this);
}

string CameraParams::get(const string& key) const
{
    string str(params_cast(priv_)->get(key.c_str()));
    return str;
}

void CameraParams::set(const string& key, const string& value)
{
    return params_cast(priv_)->set(key.c_str(), value.c_str());
}


vector<string> CameraParams::getSupportedFocusModes() const
{
    return parseCSV(
        params_cast(priv_)->get(CameraParameters::KEY_SUPPORTED_FOCUS_MODES));
}

string CameraParams::getFocusMode() const
{
    string str(params_cast(priv_)->get(CameraParameters::KEY_FOCUS_MODE));
    return str;
}

void CameraParams::setFocusMode(const string& value)
{
    params_cast(priv_)->set(CameraParameters::KEY_FOCUS_MODE, value.c_str());
}

vector<string> CameraParams::getSupportedWhiteBalance() const
{
    return parseCSV(
        params_cast(priv_)->get(CameraParameters::KEY_SUPPORTED_WHITE_BALANCE));
}

string CameraParams::getWhiteBalance() const
{
    string str(params_cast(priv_)->get(CameraParameters::KEY_WHITE_BALANCE));
    return str;
}

void CameraParams::setWhiteBalance(const string& value)
{
    params_cast(priv_)->set(CameraParameters::KEY_WHITE_BALANCE, value.c_str());
}

vector<string> CameraParams::getSupportedISO() const
{
    return parseCSV(
        params_cast(priv_)->get(KEY_QC_SUPPORTED_ISO_MODES));
}

string CameraParams::getISO() const
{
    string str(params_cast(priv_)->get(KEY_QC_ISO_MODE));
    return str;
}

void CameraParams::setISO(const string& value)
{
    params_cast(priv_)->set(KEY_QC_ISO_MODE, value.c_str());
}

Range CameraParams::getSupportedSharpness() const
{
    Range range;
    range.min =
        params_cast(priv_)->getInt(KEY_QC_MIN_SHARPNESS);
    range.max =
        params_cast(priv_)->getInt(KEY_QC_MAX_SHARPNESS);
    range.step =
        params_cast(priv_)->getInt(KEY_QC_SHARPNESS_STEP);
    return range;
}

int CameraParams::getSharpness() const
{
    return params_cast(priv_)->getInt(KEY_QC_SHARPNESS);
}

void CameraParams::setSharpness(int value)
{
    char valStr[VALUE_SIZE_MAX];
    snprintf(valStr, VALUE_SIZE_MAX, "%d", value);
    params_cast(priv_)->set(KEY_QC_SHARPNESS, valStr);
}

Range CameraParams::getSupportedBrightness() const
{
    Range range;
    range.min =
        params_cast(priv_)->getInt(KEY_QC_MIN_BRIGHTNESS);
    range.max =
        params_cast(priv_)->getInt(KEY_QC_MAX_BRIGHTNESS);
    range.step =
        params_cast(priv_)->getInt(KEY_QC_BRIGHTNESS_STEP);
    return range;
}

int CameraParams::getBrightness() const
{
    return params_cast(priv_)->getInt(KEY_QC_BRIGHTNESS);
}

void CameraParams::setBrightness(int value)
{
    char valStr[VALUE_SIZE_MAX];
    snprintf(valStr, VALUE_SIZE_MAX, "%d", value);
    params_cast(priv_)->set(KEY_QC_BRIGHTNESS, valStr);
}

Range CameraParams::getSupportedContrast() const
{
    Range range;
    range.min =
        params_cast(priv_)->getInt(KEY_QC_MIN_CONTRAST);
    range.max =
        params_cast(priv_)->getInt(KEY_QC_MAX_CONTRAST);
    range.step =
        params_cast(priv_)->getInt(KEY_QC_CONTRAST_STEP);
    return range;
}

int CameraParams::getContrast() const
{
    return params_cast(priv_)->getInt(KEY_QC_CONTRAST);
}

void CameraParams::setContrast(int value)
{
    char valStr[VALUE_SIZE_MAX];
    snprintf(valStr, VALUE_SIZE_MAX, "%d", value);
    params_cast(priv_)->set(KEY_QC_CONTRAST, valStr);
}

std::vector<Range> CameraParams::getSupportedPreviewFpsRanges() const
{
    std::vector<Range> ranges;
    int rc;

    vector<string> strRanges = parseParanthesis(params_cast(priv_)->get(
        CameraParameters::KEY_SUPPORTED_PREVIEW_FPS_RANGE));

    ranges.resize(strRanges.size());
    for (vector<Size>::size_type i=0; i<strRanges.size(); i++) {
        rc = sscanf(strRanges[i].c_str(), "%d,%d",
                    &ranges[i].min, &ranges[i].max);
        /* sscanf returns number of arguments successfully assigned.
           On success, it would be 2 */
        if (rc != 2) {
            CAM_ERR("parse failed for value: %s", strRanges[i].c_str());
            break;
        }
        ranges[i].step = 1;
    }
    return ranges;
}

Range CameraParams::getPreviewFpsRange() const
{
    //int rc;
    Range range;
    params_cast(priv_)->getPreviewFpsRange(&range.min, &range.max);
    range.step = 1;
    return range;
}

void CameraParams::setPreviewFpsRange(const Range& value)
{
    char valueStr[VALUE_SIZE_MAX];
    snprintf(valueStr, VALUE_SIZE_MAX, "%d,%d", value.min, value.max);
    params_cast(priv_)->set(CameraParameters::KEY_PREVIEW_FPS_RANGE, valueStr);
}

std::vector<VideoFPS> CameraParams::getSupportedVideoFps() const
{
    vector<VideoFPS> values;
    vector<string> strValues = parseCSV(params_cast(priv_)->get(
        KEY_QC_SUPPORTED_VIDEO_HIGH_FRAME_RATE_MODES));

    values.resize(strValues.size());
    for (vector<Size>::size_type i=0; i<values.size(); i++) {
        values[i] = toVideoFPS(strValues[i]);
    }
    return values;
}

VideoFPS CameraParams::getVideoFPS() const
{
    return toVideoFPS(params_cast(priv_)->get(KEY_QC_VIDEO_HIGH_FRAME_RATE));
}

void CameraParams::setVideoFPS(VideoFPS value)
{
    params_cast(priv_)->set(KEY_QC_VIDEO_HIGH_FRAME_RATE, toHFRMode(value));
}

string CameraParams::toString() const
{
    return (params_cast(priv_))->flatten().string();
}

vector<string> CameraParams::getSupportedPreviewFormats() const
{
    return parseCSV(
     params_cast(priv_)->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS));
}

string CameraParams::getPreviewFormat() const
{
    string str(params_cast(priv_)->get(CameraParameters::KEY_PREVIEW_FORMAT));
    return str;
}

void CameraParams::setPreviewFormat(const string& value)
{
    params_cast(priv_)->set(CameraParameters::KEY_PREVIEW_FORMAT,
                            value.c_str());
}
void CameraParams::setPictureFormat(const string& value)
{
    CAM_ERR("%s:%d setPictureFormat is not implemented for HAL1\n",__func__,__LINE__);
}

void CameraParams::setAntibanding(const string& value)
{
    params_cast(priv_)->set(CameraParameters::KEY_ANTIBANDING,
                            value.c_str());
}

void CameraParams::setManualExposure(int value)
{
    params_cast(priv_)->set(KEY_QC_EXPOSURE_MANUAL, value);
}

void CameraParams::setManualGain(int value)
{
    params_cast(priv_)->set(KEY_QC_GAIN_MANUAL, value);
}

Range64 CameraParams::getManualExposureRange(ImageSize size, int fps){
	Range64 range;
    range.min = 0, range.max = 0, range.step = 0;
    char buffer[256];
    const string str(params_cast(priv_)->get(KEY_QC_EXPOSURE_CTRL_INFO));
    CAM_INFO("%s, string: %s\n", __func__, str.c_str());

    snprintf(buffer, sizeof(buffer), "%dx%d", size.width, size.height);
    CAM_INFO("%s, string: %s\n", __func__, buffer);

    const char *start = strstr(str.c_str(), buffer);
    if (start == NULL) {
        CAM_ERR("%s, no this resolution(%s)\n", __func__, buffer);
    }

    range.min = 0;
    start = strstr(str.c_str(), "max_frame_length_lines:");
    if (start == NULL) {
        CAM_ERR("%s, cant find max_frame_length_lines\n", __func__);
    }
    range.max = atoi(start + strlen("max_frame_length_lines:"));


	return range;
}

Range CameraParams::getManualGainRange(ImageSize size, int fps){
	Range range;
    range.min = 0, range.max = 0, range.step = 0;

    char buffer[256];
    const string str(params_cast(priv_)->get(KEY_QC_EXPOSURE_CTRL_INFO));
    CAM_INFO("%s, string: %s\n", __func__, str.c_str());
    snprintf(buffer, sizeof(buffer), "%dx%d", size.width, size.height);
    CAM_INFO("%s, string: %s\n", __func__, buffer);

    const char *start = strstr(str.c_str(), buffer);
    if (start == NULL) {
        CAM_ERR("%s, no this resolution(%s)\n", __func__, buffer);
    }
    start = strstr(str.c_str(), "min_gain:");
    if (start == NULL) {
        CAM_ERR("%s, cant find min_gain\n", __func__);
    }
    range.min = atoi(start + strlen("min_gain:"));

    start = strstr(str.c_str(), "max_gain:");
    if (start == NULL) {
        CAM_ERR("%s, Cant find max_gain\n", __func__);
    }
    range.max = atoi(start + strlen("max_gain:"));
	return range;
}

int CameraParams::getExposureTime() const
{
    CAM_ERR("%s:%d getExposureTime is not implemented\n",__func__,__LINE__);
    return 0;
}

void CameraParams::setExposureTime(const string& value)
{
    params_cast(priv_)->set(KEY_QC_EXPOSURE_TIME, value.c_str());
}

void CameraParams::setVerticalFlip(bool value)
{
    params_cast(priv_)->set(KEY_QC_VERTICAL_FLIP, value);
}

void CameraParams::setHorizontalMirror(bool value)
{
    params_cast(priv_)->set(KEY_QC_HORIZONTAL_MIRROR, value);
}

void CameraParams::setStatsLoggingMask(int value)
{
    params_cast(priv_)->set(KEY_QC_STATS_LOGGING_MASK, value);
}

uint64_t CameraParams::getFrameExposureTime(ICameraFrame* frame)
{
    CAM_ERR("%s:%d getFrameExposureTime is not implemented for HAL1\n",__func__,__LINE__);
    return 0;
}

int32_t CameraParams::getFrameGainValue(ICameraFrame* frame)
{
	CAM_ERR("%s:%d getFrameGainValue is not implemented for HAL1\n",__func__,__LINE__);
   	return 0;
}
} /* namespace camera */

