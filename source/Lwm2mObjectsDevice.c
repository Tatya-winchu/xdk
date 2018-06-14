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
* @defgroup LWM2M_OBJECTS_DEVICE Lwm2mObjectsDevice
* @{
*
* @brief LWM2M Device Object source file
*
* @details LWM2M Device Object source file
*
* @file
**/

//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/

/* own header files */
#include "XDKAppInfo.h"
#include "BCDS_FotaStateMachine.h"
#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_LWM2M_OBJECTS_DEVICE
#include "FotaDownload.h"
#include "AppLwm2mObjects.h"
#include <Serval_Lwm2m.h>
/* additional interface header files */
#define LWM2M_OBJECTID_FIRMWARE 5
Lwm2mObjectInstance_T ObjectInstances[] =
        {
                { LWM2M_OBJECTID_FIRMWARE, /* 5 */
                LWM2M_SINGLE_INSTANCE,
                        LWM2M_RESOURCES(FotaStateMachine_FotaResource),
                        .permissions = { LWM2M_ACCESS_CONTROL_OWNER, 0x0, 0x0, 0x0 }
                },
        };


Lwm2mDevice_T DeviceResourceInfo =
        {
                .name = NULL,
                .binding = UDP,
                .sms = NULL,
                .numberOfObjectInstances = LWM2M_OBJECT_INSTANCE_COUNT(ObjectInstances),
                .objectInstances = ObjectInstances,
        };


static void Objects_EnableConNotifies(Lwm2mObjectInstance_T* Object)
{
    int Index = 0;
    Lwm2mResource_T* Resources = Object->resources;
    for (; Index < Object->maxNumberOfResources; ++Index)
    {
        /*lint -e(641) Suppressing as this is related to serval stack implementation*/
        if (0 == (LWM2M_NOT_READABLE & Resources[Index].type))
        {
            Resources[Index].type |= LWM2M_CONFIRM_NOTIFY;
        }
    }
}

void Objects_Init(bool ConNotifies)
{
    if (ConNotifies)
    {
        int Index = 0;
        for (; Index < DeviceResourceInfo.numberOfObjectInstances; ++Index)
        {
            Objects_EnableConNotifies(&(DeviceResourceInfo.objectInstances[Index]));
        }
    }
}
/**@} */
