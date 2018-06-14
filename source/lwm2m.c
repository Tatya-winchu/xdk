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
 * @ingroup APPS_LIST
 *
 * @defgroup LWM2M_EXAMPLE_CLIENT lwm2m
 * @{
 *
 * @brief LWM2M Client for connecting to a LWM2M server.
 *
 * @details This module will act as the client of the LWM2M protocol.
 *
 * This example demonstrates how to register with a LWM2M Server including the standard LWM2M Objects.
 * Unsecure and encrypted communication are supported. The example will connect to the <a href="http://leshan.eclipse.org">leshan.eclipse.org</a> LWM2M Server.
 *
 * You need to enter your WLAN-Credetials in \ref lwm2m.h.
 * Once the XDK is connected to the network and to the Leshan Server the orange LED will turn on.
 * From the website you can now click on the desired client and read the objects.
 *
 * \image html LeshanClient.png  "Leshan Client – User Interface to the Leshan Server; Object Read" width=\textwidth
 * @file
 *
 **/

/* module includes ********************************************************** */
//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/
/* own header files */
#include "XDKAppInfo.h"

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_LWM2M_EXAMPLE_CLIENT

#include "AppLwm2mInterface.h"
#include "AppLwm2mObjects.h"
#include "lwm2m.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "timers.h"
#include "BCDS_FWContainer.h"

/* system header files */

/* additional interface header files */
#include "BCDS_CmdProcessor.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "BCDS_ServalPal.h"
#include "BCDS_ServalPalWiFi.h"
#include <Serval_Lwm2m.h>
#include <Serval_Log.h>
#include "FotaDownload.h"


/* constant definitions ***************************************************** */
#define LOG_MODULE "CCA"

/* local variables ********************************************************** */
	//#define xTimerHandle TimerHandle_t
/* global variables ********************************************************* */

/*Application Command Processor Instance */
CmdProcessor_T *AppCmdProcessor;

static CmdProcessor_T CommandProcessorHandle;

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief This API is called when the LWM2M Client
 *      Connecting to a WLAN Access point.
 *       This function connects to the required AP (SSID_NAME).
 *       The function will return once we are connected and have acquired IP address
 *   @warning
 *      If the WLAN connection fails or we don't acquire an IP address, We will be stuck in this function forever.
 *      Check whether the callback "SimpleLinkWlanEventHandler" or "SimpleLinkNetAppEventHandler" hits once the
 *      sl_WlanConnect() API called, if not check for proper GPIO pin interrupt configuration or for any other issue
 *
 * @retval     RC_OK       IP address returned successfully
 *
 * @retval     RC_PLATFORM_ERROR         Error occurred in fetching the ip address
 *
 */

static retcode_t WlanConnect(void)
{
    NetworkConfig_IpSettings_T myIpSettings;
    char ipAddress[SERVAL_IP_ADDR_LEN] = { 0 };
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    WlanConnect_SSID_T connectSSID;
    WlanConnect_PassPhrase_T connectPassPhrase;
    Retcode_T ReturnValue = (Retcode_T) RETCODE_FAILURE;
    int32_t Result = INT32_C(-1);

    if (RETCODE_OK != WlanConnect_Init())
    {
        return (RC_PLATFORM_ERROR);
    }

    connectSSID = (WlanConnect_SSID_T) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WlanConnect_PassPhrase_T) WLAN_CONNECT_WPA_PASS;

    ReturnValue = NetworkConfig_SetIpDhcp(NULL);
    if (ReturnValue != RETCODE_OK)
    {
        printf("Error in setting IP to DHCP\n\r");
        return (RC_PLATFORM_ERROR);
    }
    if (sl_WlanRxStatStart() != 0) {
                            printf("Error initiliazing RSSI stat collection");
    }
    if (RETCODE_OK == WlanConnect_WPA(connectSSID, connectPassPhrase, NULL))
    {
        ReturnValue = NetworkConfig_GetIpSettings(&myIpSettings);
        if (RETCODE_OK == ReturnValue)
        {
            *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
            Result = Ip_convertAddrToString(IpaddressHex, ipAddress);
            if (Result < INT32_C(0))
            {
                printf("Couldn't convert the IP address to string format \r\n ");
                return (RC_PLATFORM_ERROR);
            }
            printf("Connected to WPA network successfully \r\n ");
            printf(" Ip address of the device %s \r\n ", ipAddress);
            return (RC_OK);
        }

        else
        {
            printf("Error in getting IP settings\n\r");
            return (RC_PLATFORM_ERROR);
        }
    }
    else
    {
        return (RC_PLATFORM_ERROR);
    }

}

/**
 * @brief This will initialize the ServalPAL module and its necessary resources.
 *
 * @return Status of initialization
 */
static Retcode_T ServalPalSetup(void)
{
    Retcode_T returnValue = RETCODE_OK;
    returnValue = CmdProcessor_Initialize(&CommandProcessorHandle, "Serval PAL", TASK_PRIORITY_SERVALPAL_CMD_PROC, TASK_STACK_SIZE_SERVALPAL_CMD_PROC, TASK_QUEUE_LEN_SERVALPAL_CMD_PROC);
    /* serval pal common init */
    if (RETCODE_OK == returnValue)
    {
        returnValue = ServalPal_Initialize(&CommandProcessorHandle);
    }
    if (RETCODE_OK == returnValue)
    {
        returnValue = ServalPalWiFi_Init();
    }
    if (RETCODE_OK == returnValue)
    {
        ServalPalWiFi_StateChangeInfo_T stateChangeInfo = { SERVALPALWIFI_OPEN, INT16_C(0) };
        returnValue = ServalPalWiFi_NotifyWiFiEvent(SERVALPALWIFI_STATE_CHANGE, &stateChangeInfo);
    }
    return returnValue;
}
/* global functions ********************************************************** */

/** @brief This function does the initialization of required setup to interact with lwm2m Server and the objects that are defined for this project.
 *  It includes providing the necessary data for the objects like object and resource ID, object name,
 *  its instances.
 */
void Init(void)
{
    char buffer[128];
    retcode_t rc = RC_OK;
	Retcode_T retcode = RETCODE_OK;
    int32_t Result = INT32_C(-1);

    printf("INIT Starting up\r\n");

    printf("WlanConnect\r\n");
    rc = WlanConnect();

    if (RC_OK != rc)
    {


        printf("Network Init/connection failed %i \r\n", rc);
        return;
    }
    printf("Network Initialization done\r\n");

    printf("ServalPal Setup\r\n");
    retcode = ServalPalSetup();
    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
        printf("ServalPal Setup failed with %d \r\n", retcode);
        return;
    }

    printf("PAL Initialization done\r\n");

    LOG_INFO("Logging enabled\r\n");

    /* IP address */
    Ip_Address_T * my_ip = Ip_getMyIpAddr();
    Result = Ip_convertAddrToString(my_ip, buffer);
    if (Result < 0)
    {
        printf("Couldn't convert the IP address to string format \r\n ");
        return;
    }
    printf("IP Address of the Device is %s\r\n", buffer);

    printf("Lwm2mInterface Initialization start\r\n");
    rc = Lwm2mInterfaceInitialize();
    if (RC_OK != rc)
    {
        assert(false);
    }
    printf("Lwm2m Interface done\r\n");

    printf("Lwm2m Interface start with given port\r\n");
    rc = Lwm2mInterfaceStart(Ip_convertIntToPort(CLIENT_CONNECT_PORT));
    if (RC_OK != rc)
    {
        assert(false);
    }
    printf("Lwm2m Interface started\r\n");

    printf("LCA Initialization Done\r\n");
}

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;
    BCDS_UNUSED(param2);
     Init();
}

CmdProcessor_T * GetAppCmdProcessorHandle(void)
{
    return AppCmdProcessor;
}

/**@} */
/** ************************************************************************* */
