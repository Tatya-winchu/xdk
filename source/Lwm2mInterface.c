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
 * @defgroup LWM2M_INTERFACE Lwm2mInterface
 * @{
 *
 * @brief LWM2M Interface source file
 *
 * @details  This module is integrated as the top layer of the serval stack
 * which in-turn interacts with the software platform (via. user page (or)
 * application) for required information. This is operated from the Server
 * for Read, write, observe and/or execution modes.
 *
 * @file
 **/

//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/
/* own header files */
#include "XDKAppInfo.h"

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_LWM2M_INTERFACE

#include "AppLwm2mInterface.h"
#include "AppLwm2mObjects.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "FotaDownload.h"
#include "FotaDownload.c"

#warning Enable and disable security here, by default security has disabled by defining SECURITY_ENABLE as 0.
#define SECURITY_ENABLE SECURITY_OPTION

#if SECURITY_ENABLE==0
/* "5.39.83.206" is the ip address of the eclipse leshan server for testing purpose */
#  define LWM2M_SERVER_IP            "coap://172.18.0.67:5683" /**< using standard CoAP ports unsecure 5683 secure 5684, giving an appending port will override */
#elif SECURITY_ENABLE==1
#  define LWM2M_SECURE_SERVER_IP        "coaps://0.0.0.0/0.0.0.0:5684" /**< using standard CoAP ports unsecure 5683 secure 5684, giving an appending port will override */
#endif
#if SERVAL_ENABLE_COAP_SERVER

/* additional interface header files */
#include "BCDS_CmdProcessor.h"
#include "BSP_BoardType.h"
#include "BCDS_BSP_LED.h"
#include "FotaDownload.h"
#include <Serval_Log.h>
#include <Serval_Timer.h>
#include <Serval_CoapClient.h>
#include <Serval_Clock.h>
#include "lwm2m.h"
#define LOG_MODULE "CCA"

/* global variables ********************************************************* */
static uint8_t RegistrationUpdateRetry = UINT8_C(0);

/** variable to store timer handle*/
xTimerHandle registrationUpdateTimer_ptr;
xTimerHandle timeChangeTimer_ptr;

extern Lwm2mDevice_T DeviceResourceInfo;

enum ServerRegistrationState_E
{
	SRS_NONE,
	SRS_ENABLED,
	SRS_RETRY_REGISTRATION,
	SRS_REGISTRATION_PENDING,
	SRS_REGISTRATED,
	SRS_UPDATE_PENDING,
	SRS_DEREGISTRATION_PENDING,
	SRS_SEND_FAILED,
	SRS_REGISTRATION_TIMEOUT,
	SRS_UPDATE_TIMEOUT,
};typedef enum DnsState_E DnsState_T;

typedef enum ServerRegistrationState_E ServerRegistrationState_T;

enum DnsState_E
{
	DNS_NONE,
	DNS_HOST,
	DNS_PENDING,
	DNS_RESOLVED,
	DNS_ERROR
};


/* local functions ********************************************************** */

/* @brief
 * This function is used to update the time when the device has registered first attempt into the Leshan Server.
 *
 */
void timeChanged(xTimerHandle pxTimer)
{
    BCDS_UNUSED(pxTimer);

    retcode_t rc = RC_OK;

    /* Here URI_Path is the path points to the "Current Time" Resource */
    Lwm2m_URI_Path_T currentTimeUriPath = { OBJECT_INDEX_VALUE, OBJECT_INSTANCE_INDEX_VALUE, OBJECT_RESOURCE_NUMBER };
    rc = Lwm2mReporting_resourceChanged(&currentTimeUriPath);
    if (rc != RC_OK)
    {
        printf("Could not send notification\r\n");
    }
}


/**
 * @brief API responsible to read html page content of the client and print it on the console.
 *
 * @param[in] Not used.
 *
 *
 * @param[in] Not used.
 */
static void EnqueueRegitrationUpdateEvt(void * param1, uint32_t param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    retcode_t rc = RC_OK;

    rc = Lwm2mRegistration_update(SERVER_INDEX);
    if (RC_OK == rc)
    {
        RegistrationUpdateRetry = UINT8_C(0);
    }
    else
    {
        printf("registration Update failed\r\n");
        RegistrationUpdateRetry++;
    }
    if (RegistrationUpdateRetry >= MAX_REGISTRATION_UPDATE_RETRY)
    {
        printf("Max no. of retries has been done for registration update\n\r");
        if (xTimerStop(registrationUpdateTimer_ptr, TIMERBLOCKTIME) != pdTRUE)
        {
            assert(false);
        }
    }
}


/**
 * @brief The function is to get the registration status of the Device while connection to LWM2M Server 
 *	and It will toggle the Orange LED to indicate the Registration Success State and Red LED will indicate the Registration Failure state.
 */
static void registrationCallback(retcode_t status)
{
	static bool fotainit = true;
	 Retcode_T returnValue = RETCODE_OK;
    if (status == RC_OK)
    {
        printf("Registration succeeded\r\n");


        returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_OFF);
        if(fotainit)
        		{
        	returnValue = FotaInit();

        		    if (RETCODE_OK == returnValue)
        		    {
        		    	fotainit = false;
        		        printf("Fota Init success \n");
        		    }
        		    else
        		    {
        		    	fotainit = true;
        		        printf("Fota Init Failed \n");
        		    }
        		}

        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_ON);
        }
        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (returnValue != RETCODE_OK)
        {
            printf("Setting LED state failed\r\n");
        }

        if (xTimerStart(registrationUpdateTimer_ptr, TIMERBLOCKTIME) != pdTRUE)
        {
            assert(false);
        }
    }
    else
    {
        Retcode_T returnValue = RETCODE_OK;

        returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_ON);
        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (returnValue != RETCODE_OK)
        {
            printf("Setting LED state failed\r\n");
        }

        printf("registration failed " RC_RESOLVE_FORMAT_STR "\n", RC_RESOLVE((unsigned int )status));
        /* TODO maybe resue the registrationUpdateTimer_ptr for retrying...*/
    }
}
/**
 * @brief The function is to get the Updated Registration status of the Device while re-connection to LWM2M Server.
 */
static void registrationUpdateCallback(retcode_t status)
{
    if (status == RC_OK)
    {
        printf("Registration update succeeded\r\n");
    }
    else
    {
        printf("registration update failed " RC_RESOLVE_FORMAT_STR "\n", RC_RESOLVE((unsigned int )status));
        /* TODO maybe resue the registrationUpdateTimer_ptr for retrying...*/
    }
}
/**
 * @brief The function is to deregister the device in LWM2M Server.
 */
static void deregistrationCallback(retcode_t status)
{
    if (status == RC_OK)
    {
        printf("Deregistration succeeded\r\n");
    }
    else
    {
        printf("deregistration failed " RC_RESOLVE_FORMAT_STR "\n", RC_RESOLVE((unsigned int )status));
        /* TODO maybe resue the registrationUpdateTimer_ptr for retrying...*/
    }
}

/**
 * @brief The callback will get triggerd whenever there is a Notification event.
 */
static void NotificationCallback(retcode_t status)
{
    if (status == RC_OK)
    {
        printf("Notification succeeded\r\n");
        RegistrationUpdateRetry = UINT8_C(0);
        if ( xTimerReset(registrationUpdateTimer_ptr, TIMERBLOCKTIME) != pdTRUE)
        {
            assert(false);
        }
    }
    else
    {
        printf("Notification failed " RC_RESOLVE_FORMAT_STR "\n", RC_RESOLVE((unsigned int )status));
    }
}

void RegistrationUpdate(xTimerHandle pxTimer)
{
    BCDS_UNUSED(pxTimer);
    Retcode_T returnValue = RETCODE_OK;

    returnValue = CmdProcessor_Enqueue(GetAppCmdProcessorHandle(), EnqueueRegitrationUpdateEvt, NULL, UINT32_C(0));
    if (RETCODE_OK != returnValue)
    {
        printf("En-queuing to application command processor failed\n\r");
    }
}
/**
 * @brief The function will get called when we trying to register to the LWM2M Server with configured port Number and also It will provide the RED & Orange LED indications to represent the Registration process.
 */
static void applicationCallback(Lwm2m_Event_Type_T eventType, Lwm2m_URI_Path_T *path, retcode_t status)
{
    BCDS_UNUSED(path);

    if (eventType == LWM2M_EVENT_TYPE_REGISTRATION)
    {

        printf("Registration process has started\r\n");
        Retcode_T returnValue = RETCODE_OK;

        returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_ON);
        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_ON);
        }
        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (returnValue != RETCODE_OK)
        {
            printf("Setting LED state failed\r\n");
        }

        registrationCallback(status);
    }
    else if (eventType == LWM2M_EVENT_TYPE_REGISTRATION_UPDATE)
    {
        registrationUpdateCallback(status);
    }
    else if (eventType == LWM2M_EVENT_TYPE_DEREGISTRATION)
    {
        deregistrationCallback(status);
    }
    else if (eventType == LWM2M_EVENT_TYPE_NOTIFICATION)
    {
        NotificationCallback(status);
    }
}

/* global functions ********************************************************* */

/* temporary, function will be exported by lwm2m_security in future version */
static inline retcode_t Lwm2mInterface_copyToBuffer(OutputBuffer_T* buffer, const char* data, uint16_t len)
{
    if (buffer->writableLength == 0)
    {
        buffer->length = len;
        buffer->content = (char*) data;
        return RC_OK;
    }
    else if (buffer->writableLength >= len)
    {
        buffer->length = len;
        memcpy(buffer->content, data, len);
        return RC_OK;
    }
    else
    {
        return RC_DTLS_INSUFFICIENT_MEMORY;
    }
}
/**********
 *
 * just testing a function
 *
 */
static retcode_t Lwm2mInterface_securityCallback(SecurityToken_T token, SecurityData_T* tokenData)
{
    if (token.type != PSK_PEER_KEY_AND_ID)
        return RC_DTLS_UNSUPPORTED_TOKEN;

    retcode_t rc = Lwm2mSecurity_defaultCallback(token, tokenData);
    if (RC_DTLS_PEER_REJECTED == rc)
    {
        PeerKeyAndIdData_T* data = (PeerKeyAndIdData_T*) tokenData;
        Lwm2mServer_T* Server = Lwm2m_getServer(0);
        rc = Lwm2mInterface_copyToBuffer(&data->ourIdentity, Server->securityInfo.my_identity, strlen(Server->securityInfo.my_identity));
        if (RC_OK == rc)
        {
            /* Serval 1.7 uses a string for Lwm2mSecurityInfo_T.secret_key. */
            /* This limitation is removed in serval 1.8, */
            /* Therefore the code below must be adjusted when updating to serval 1.8! */
            rc = Lwm2mInterface_copyToBuffer(&data->key, (const char*)Server->securityInfo.secret_key, Server->securityInfo.secret_key_length);
        }
    }
    return rc;
}
/* API documentation is in the header file lwm2mInterface.h*/
retcode_t Lwm2mInterfaceStart(Ip_Port_T port)
{
    retcode_t rc = RC_OK;
    Retcode_T LedReturnValue = RETCODE_OK;

    Security_setCallback(Lwm2mInterface_securityCallback);

    Lwm2mServer_T* server = Lwm2m_getServer(SERVER_INDEX);

#if SECURITY_ENABLE==0
    printf("coap without encryption\r\n");
    strncpy(server->serverAddress, LWM2M_SERVER_IP, strlen(LWM2M_SERVER_IP));
#elif SECURITY_ENABLE==1
    printf("coap with encryption\r\n");
    strncpy(server->serverAddress, LWM2M_SECURE_SERVER_IP, strlen(LWM2M_SECURE_SERVER_IP));
    strncpy(server->securityInfo.peer_identity, "Leshan", strlen("Leshan"));
    strncpy(server->securityInfo.my_identity, "XDK110_0", strlen("XDK110_0"));
    strncpy(server->securityInfo.secret_key, "Leshan", strlen("Leshan"));
    /* Serval 1.7 uses a string for Lwm2mSecurityInfo_T.secret_key. */
    /* This limitation is removed in serval 1.8, */
    /* Therefore the code below must be adjusted when updating to serval 1.8! */
    server->securityInfo.secret_key_length = strlen("Leshan");
#endif
    server->permissions[0] = LWM2M_READ_ALLOWED;
    Lwm2m_setNumberOfServers(NUMBER_OF_SERVERS);

    /* LED created to represent the registration status */
    LedReturnValue = BSP_LED_Connect();
    if (RETCODE_OK != LedReturnValue)
    {
        printf("Mapping to the GPIO Pins to enable LED Failed \r\n");
    }

    LedReturnValue = BSP_LED_EnableAll();
    if (RETCODE_OK != LedReturnValue)
    {
        printf("Enabling the Control over the LEDs Failed \r\n");
    }

    rc = Lwm2m_start(port, applicationCallback);

    if (rc != RC_OK)
    {
        printf("Starting the Lightweight M2M module failed\r\n");
        assert(false);
    }
    uint32_t Ticks = pdMS_TO_TICKS(TIME_CHANGE_NOTIFICATION_DELAY); /* Ticks in millisecond */

    timeChangeTimer_ptr = xTimerCreate((const char * const ) "timeChangeTimer_ptr", // text name, used for debugging.
            Ticks, // The timer period in ticks.
            pdTRUE, // The timers will auto-reload themselves when they expire.
            (void *) NULL, // Assign each timer a unique id equal to its array index.
            timeChanged // On expiry of the timer this function will be called.
            );
    if (timeChangeTimer_ptr == NULL) /* Timer was not created */
    {
        assert(false);
    }
    if (xTimerStart(timeChangeTimer_ptr, TIMERBLOCKTIME) != pdTRUE)
    {
        assert(false);
    }

    uint32_t RegUpdateTicks = pdMS_TO_TICKS(REGISTRATION_UPDATE_DELAY); /* Ticks in millisecond */

    registrationUpdateTimer_ptr = xTimerCreate((const char * const ) "RegistrationUpdate_ptr", // text name, used for debugging.
            RegUpdateTicks, // The timer period in ticks.
            pdTRUE, // The timers will auto-reload themselves when they expire.
            (void *) NULL, // Assign each timer a unique id equal to its array index.
			RegistrationUpdate // On expiry of the timer this function will be called.
            );
    if (registrationUpdateTimer_ptr == NULL) /* Timer was not created */
    {
        assert(false);
    }

    rc = Lwm2mRegistration_register(SERVER_INDEX);
    if (RC_OK != rc)
    {
        printf("Registration Failed\r\n");
    }
    return rc;
}

/* API documentation is in the header file lwm2mInterface.h*/
retcode_t Lwm2mInterfaceInitialize(void)
{
    retcode_t rc = RC_OK;
#if SECURITY_ENABLE==0
    DeviceResourceInfo.name = "XDK110_UNSECURE_1";
    DeviceResourceInfo.secure = false;
#elif SECURITY_ENABLE==1
    deviceResourceInfo.name = "XDK110_SECURE_DTLS";
    deviceResourceInfo.secure = true;
#endif

    printf("Lwm2m_initialize\r\n");
    rc = Lwm2m_initialize(&DeviceResourceInfo);
    printf("done\r\n");
    //Objects_Init(enableConNotifies);
    return rc;
}

#endif /* SERVAL_ENABLE_COAP_SERVER */
/**@} */
