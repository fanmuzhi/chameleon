/*! @file vcsPalProfiler.h
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (C) 2006-2016 Synaptics Incorporated. All rights reserved.
 **
 **
 ** This file contains information that is proprietary to Synaptics
 ** Incorporated ("Synaptics"). The holder of this file shall treat all
 ** information contained herein as confidential, shall use the
 ** information only for its intended purpose, and shall not duplicate,
 ** disclose, or disseminate any of this information in any manner unless
 ** Synaptics has otherwise provided express, written permission.
 ** Use of the materials may require a license of intellectual property
 ** from a third party or from Synaptics. Receipt or possession of this
 ** file conveys no express or implied licenses to any intellectual
 ** property rights belonging to Synaptics.
 **
 **
 ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
 ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
 ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
 ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
 ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
 ** OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY TO ANY PARTY
 ** SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
*/


#ifndef __palProfiler_h__
#define __palProfiler_h__

typedef enum prfilerTypes_e {

    startNopePrf = 0,

    deviceInitPrf = 0,
    getPrintPrf,
    
    endNopePrf
}ProfilerTypes_t;

/*Make sure every operation ID is unique */

#define OP_CAPTURE_START  0x00000001
#define OP_DEV_INIT       0x00000002
#define OP_WINUSB_INIT    0x00000003



#if VCS_FEATURE_PROFILING
/* FIXME: Temporary disable this code, because it was not included in any existing integration */
#if 0

/* Initialize global profiler context. All other profiler function 
   call will be ignored when global profiler isn't initialized*/
void palStartUpProfiler(void);

/*Cleanup global profiler context.*/
void palShutDownProfiler(void);

/* Register specific profiler for separate device. It will contain all
   operations informations which will be profiled through palStartOpProfiling 
   and palStopOpProfiling functions call*/
void palRegisterSpecificDeviceProfiler(ProfilerTypes_t prType, vcsString_t devName);

/* Unregister specific profiler for specific device. After this function call 
   all operations informations which were profiled will be lost*/
void palUnRegisterSpecificDeviceProfiler(ProfilerTypes_t prType, vcsString_t devName);

/* Start operation profiling. The specified operation profile will be attached  
   to the specific device profiler which had to be registered before. The function
   call will be skipped if either the palStartUpProfiler and 
   palRegisterSpecificDeviceProfiler were no called before */
void palStartOpProfiling(ProfilerTypes_t prType, vcsString_t devName, vcsUint32_t opId);

/* Stop operation profiling. The specified operation profile will be filled with
   corresponding information e.g. operation duration.*/
void palStopOpProfiling(ProfilerTypes_t prType, vcsString_t devName, vcsUint32_t opId);


/*Retrieve the system resuming time which was registered before*/
vcsUint32_t palProfilerGetSystemResumeTime(void);
void palProfilerRegisterSysResumeTime(void);
void TimeLineCptStartAfterResume(vcsString_t devName);


#define PRF_STARTUP()                              palStartUpProfiler();
#define PRF_SHUTDOWN()                             palShutDownProfiler();
#define PRF_REGISTER_DEVICE(prType, devName)       palRegisterSpecificDeviceProfiler(prType, devName)
#define PRF_UNREGISTER_DEVICE(prType, devName)     palUnRegisterSpecificDeviceProfiler(prType, devName);
#define PRF_START_OPERATION(prType, devName, opId) palStartOpProfiling(prType, devName, opId);
#define PRF_STOP_OPERATION(prType, devName, opId)  palStopOpProfiling(prType, devName, opId);
#define PRF_REGISTER_SYSRESUME()                   palProfilerRegisterSysResumeTime();

#define PRF_CPT_START_BREACKDOWN(devName)          TimeLineCptStartAfterResume(devName);

#endif /*0*/

#define PRF_STARTUP()
#define PRF_SHUTDOWN()
#define PRF_REGISTER_DEVICE(prType, devName)
#define PRF_UNREGISTER_DEVICE(prType, devName)
#define PRF_START_OPERATION(prType, devName, opId)
#define PRF_STOP_OPERATION(prType, devName, opId)
#define PRF_REGISTER_SYSRESUME()

#define PRF_CPT_START_BREACKDOWN(devName);


#define PROFILING_TAG                              "PROFILING:"
#define PRF_REGISTER_2()                           vcsUint32_t startTime = 0, stopTime = 0;
#define PRF_START_OPERATION_2()                    startTime = palGetTickCount();
#define PRF_STOP_OPERATION_AND_PRINT_2(A, S)                                        \
    stopTime = palGetTickCount();                                                   \
    if (0 != startTime)                                                             \
    {                                                                               \
        LOG_PROFILING(A, S);                                                        \
        LOG_PROFILING(A, ("%s Time = %d\n", PROFILING_TAG, stopTime - startTime));  \
    }


#else/* VCS_FEATURE_PROFILING */

#define PROFILING_TAG               ""
#define PRF_REGISTER_2()
#define PRF_START_OPERATION_2()
#define PRF_STOP_OPERATION_AND_PRINT_2(A, S)


#define PRF_STARTUP()
#define PRF_SHUTDOWN()
#define PRF_REGISTER_DEVICE(prType, devName)
#define PRF_UNREGISTER_DEVICE(prType, devName)
#define PRF_START_OPERATION(prType, devName, opId)
#define PRF_STOP_OPERATION(prType, devName, opId)
#define PRF_REGISTER_SYSRESUME()

#define PRF_CPT_START_BREACKDOWN(devName);


#endif /*VCS_FEATURE_PROFILING */

#endif /* __palProfiler_h__ */
