/*
 * Copyright (c) 2016-2017, The Linux Foundation. All rights reserved.
 * Not a Contribution.
 *
 * Copyright 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TONEMAPPER_TONEMAP_LE_H__
#define __TONEMAPPER_TONEMAP_LE_H__

#define TONEMAP_FORWARD 0
#define TONEMAP_INVERSE 1

#include "EGLImageWrapperLE.h"
#include "engine.h"

class Tonemapper {
 private:
  void* engineContext;
  unsigned int tonemapTexture;
  unsigned int lutXformTexture;
  unsigned int programID;
  float lutXformScaleOffset[2];
  float tonemapScaleOffset[2];
  EGLImageWrapperLE* eglImageWrapperLE;
  Tonemapper();

 public:
  ~Tonemapper();
  static Tonemapper *build(int type, void *colorMap, int colorMapSize, void *lutXform,
                           int lutXformSize);
  int blit(void *dst, void *src, int srcFenceFd);
};

#endif  //__TONEMAPPER_TONEMAP_LE_H__
