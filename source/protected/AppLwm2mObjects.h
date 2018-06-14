/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee�s application development. 
* Fitness and suitability of the example code for any use within Licensee�s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*----------------------------------------------------------------------------*/

/**
* @ingroup LWM2M_EXAMPLE_CLIENT
*
* @defgroup APP_LWM2M_OBJECTS AppLwm2mObjects
* @{
*
* @brief Configuration header for the Lwm2mObjects.c file.
*
* @details Configuration header for the Lwm2mObjects.c file.
*
*
* @file
**/

#ifndef XDK110_LWM2MOBJECTS_H_
#define XDK110_LWM2MOBJECTS_H_

#include <Serval_Lwm2m.h>

/* public type and macro definitions */
#define LWM2MOBJECTS_UDP_LENGTH                 UINT32_C(1)
#define LWM2MOBJECTS_UDP_QUEUED_LENGTH          UINT32_C(2)
#define LWM2MOBJECTS_SMS_LENGTH                 UINT32_C(1)
#define LWM2MOBJECTS_SMS_QUEUED_LENGTH          UINT32_C(2)
#define LWM2MOBJECTS_UDP_AND_SMS_LENGTH         UINT32_C(2)
#define LWM2MOBJECTS_UDP_QUEUED_AND_SMS_LENGTH  UINT32_C(3)
#define STRING_RESOURCES_MAX_LENGTH             UINT32_C(50)
#define OBJECTS_IX_FIRMWARE_0                   UINT32_C(2) /**< object or instance index for \link Lwm2mObject_ConnectivityMonitoring.h \endlink in Lwm2m_URI_Path_T */
/* Object ID */
#define LWM2M_OBJECTID_DEVICE 3

/*struct DeviceResource_S
{
    Lwm2mResource_T manufacturer;
    Lwm2mResource_T modelNumber;
    Lwm2mResource_T serialNumber;
    Lwm2mResource_T FirmwareVersion;
    Lwm2mResource_T deviceReboot;
    Lwm2mResource_T factoryReset;
    Lwm2mResource_T availablePowerSources;
    Lwm2mResource_T powerSourceVoltage;
    Lwm2mResource_T powerSourceCurrent;
    Lwm2mResource_T batteryLevel;
    Lwm2mResource_T memoryFree;
    Lwm2mResource_T errorCode;
    Lwm2mResource_T resetErrorCode;
    Lwm2mResource_T currentTime;
    Lwm2mResource_T UTCOffset;
    Lwm2mResource_T Timezone;
    Lwm2mResource_T SupportedBindingAndModes;

};*/
void Objects_Init(bool ConNotifies);
void Objects_Update(void);
extern struct DeviceResource_S deviceResources;

#endif /* XDK110_LWM2MOBJECTS_H_ */
