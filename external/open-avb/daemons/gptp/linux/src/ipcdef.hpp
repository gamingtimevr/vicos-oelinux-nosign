/******************************************************************************

  Copyright (c) 2009-2012, Intel Corporation 
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met:
  
   1. Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
  
   2. Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
  
   3. Neither the name of the Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived from 
      this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#ifndef IPCDEF_HPP
#define IPCDEF_HPP

#include <sys/types.h>
#include <ptptypes.hpp>

/**@file*/

/**
 * Provides a data structure for gPTP time
 */
typedef struct { 
	int64_t ml_phoffset;			//!< Master to local phase offset
	int64_t ls_phoffset;			//!< Local to system phase offset
	FrequencyRatio ml_freqoffset;	//!< Master to local frequency offset
	FrequencyRatio ls_freqoffset;	//!< Local to system frequency offset
	uint64_t local_time;				//!< Local time of last update
	uint32_t sync_count;			//!< Sync messages count
	uint32_t pdelay_count;			//!< pdelay messages count
	PortState port_state;			//!< gPTP port state. It can assume values defined at ::PortState
	pid_t  process_id;				//!< Process id number
} gPtpTimeData;


/*

  Integer64  <master-local phase offset>
  Integer64  <local-system phase offset>
  LongDouble <master-local frequency offset>
  LongDouble <local-system frequency offset>
  UInteger64 <local time of last update>

  * Meaning of IPC provided values:

  master  ~= local   - <master-local phase offset>
  local   ~= system  - <local-system phase offset>
  Dmaster ~= Dlocal  * <master-local frequency offset>  
  Dlocal  ~= Dsystem * <local-system freq offset>        (where D denotes a delta)

*/

#define SHM_SIZE (sizeof(gPtpTimeData) + sizeof(pthread_mutex_t)) 	/*!< Shared memory size*/
#ifdef ANDROID
#define SHM_NAME  "/dev/ptpshm"		/*!< Shared memory name*/
#else
#define SHM_NAME  "/ptp"		/*!< Shared memory name*/
#endif
#endif/*IPCDEF_HPP*/
