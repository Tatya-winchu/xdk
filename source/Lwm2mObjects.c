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
* @defgroup LWM2M_OBJECTS Lwm2mObjects
* @{
*
* @brief LWM2M Objects source file
*
* @details  Here, the ID s of the objects, resources and object instances are received and the URI is created from
* these data. The created URI can be used for getting / updating data from / to server.
* The created URI can be sent as a payload to the communication layer for further processing.
*
* @file
**/

//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/

/* own header files */
#include "XDKAppInfo.h"

#undef BCDS_MODULE_ID  /* Module ID define before including Basics package*/
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_LWM2M_OBJECTS

#include "AppLwm2mObjects.h"
#include "AppLwm2mInterface.h"
#include "BCDS_Fota.h"
#include <BCDS_FWContainer.h>
#include <FotaDownload.h>
/* additional interface header files */
#include <Serval_Clock.h>

/* global variables ********************************************************* */

extern Lwm2mDevice_T deviceResourceInfo;
#define STRING_LENGTH UINT8_C(19)
/* local variables ********************************************************* */
static int32_t offset = UINT32_C(0);
static char tz[20] = "Europe/Berlin";

/* local functions ********************************************************** */

/* @brief
 * This function is used to update the deviceReboot Resource value into the LWM2M Server*
 */

/*
static retcode_t firmwareUpdateFunc(Lwm2mSerializer_T *serializer_ptr,Lwm2mParser_T*parser_ptr)
{
	   BCDS_UNUSED(parser_ptr);
	    BCDS_UNUSED(serializer_ptr);
	    printf("Update\r\n");
	    return (RC_OK);

}

static retcode_t deviceRebootFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    BCDS_UNUSED(parser_ptr);
    BCDS_UNUSED(serializer_ptr);

    printf("deviceReboot\r\n");
    return (RC_OK);
}*/
/* @brief
 * This function is used to update the factoryReset Resource value into the LWM2M Server*

static retcode_t factoryResetFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    BCDS_UNUSED(parser_ptr);
    BCDS_UNUSED(serializer_ptr);

    printf("factoryReset\r\n");
    return (RC_OK);
}*/
/* @brief
 * This function is used to update the batteryLevel Resource value into the LWM2M Server*

static retcode_t batteryLevelFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    if (parser_ptr != NULL)
    {
        return (RC_LWM2M_METHOD_NOT_ALLOWED);
    }
    printf("batteryLevel\r\n");

    return (Lwm2mSerializer_serializeInt(serializer_ptr, 100));
}*/
/* @brief
 * This function is used to update the memoryFree Resource value into the LWM2M Server*

static retcode_t memoryFreeFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    if (parser_ptr != NULL)
    {
        return (RC_LWM2M_METHOD_NOT_ALLOWED);
    }

    printf("memoryFree\r\n");

    return (Lwm2mSerializer_serializeInt(serializer_ptr, 50));
}*/
/* @brief
 * This function is used to update the errorCode Resource value into the LWM2M Server*

static retcode_t errorCodeFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    if (parser_ptr != NULL)
    {
        return (RC_LWM2M_METHOD_NOT_ALLOWED);
    }

    printf("errorCode\r\n");

    return (Lwm2mSerializer_serializeInt(serializer_ptr, 0));
}*/
/* @brief
 * This function is used to update the resetErrorCode Resource value into the LWM2M Server*

static retcode_t resetErrorCodeFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    BCDS_UNUSED(parser_ptr);
    BCDS_UNUSED(serializer_ptr);


    printf("resetErrorCode\r\n");

    return (RC_OK);
}*/
/* @brief
 * This function is used to update the currentTime Resource value into the LWM2M Server*

static retcode_t getCurrentTime(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    retcode_t rc = RC_CLOCK_ERROR_FATAL;

    uint32_t time;
    rc = Clock_getTime(&time);
    if (rc == RC_OK)
    {
        printf("currentTime\r\n");
    }
    else
    {
        printf("error in getting time\r\n");
    }

    if (parser_ptr != NULL)
    {

        int32_t newTime;
        char newTimeString[STRING_RESOURCES_MAX_LENGTH] = { 0 };

        StringDescr_T strDescr;
        StringDescr_set(&strDescr, newTimeString, STRING_RESOURCES_MAX_LENGTH);
        rc = Lwm2mParser_getString(parser_ptr, &strDescr);

        if (rc != RC_OK)
        {
            return (rc);
        }

        rc = Lwm2mParser_convertStringToInt(newTimeString, strlen(newTimeString), &newTime);
        if (rc != RC_OK)
        {
            return (rc);
        }

        offset = newTime - time;

        Lwm2m_URI_Path_T currentTimeUriPath = { OBJECT_INDEX_VALUE, OBJECT_INSTANCE_INDEX_VALUE, OBJECT_RESOURCE_NUMBER };
        rc = Lwm2mReporting_resourceChanged(&currentTimeUriPath);
        if (rc != RC_OK)
        {
            return (rc);
        }
        return (RC_OK);
    }

    return (Lwm2mSerializer_serializeTime(serializer_ptr, time + offset));
}*/ /* Update current time */
/* @brief
 * This function is used to update the tzone Resource value into the LWM2M Server*

static retcode_t getTimeZone(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    StringDescr_T strDescr_tz;
    StringDescr_set(&strDescr_tz, tz, strlen(tz));

    printf("tzone\r\n");

    if (parser_ptr == NULL)
    {
        return (Lwm2mSerializer_serializeString(serializer_ptr, &strDescr_tz));
    }
    else
    {
        retcode_t rc;
        char newTz[STRING_RESOURCES_MAX_LENGTH]={0};

        StringDescr_T strDescr = {0};
        StringDescr_set(&strDescr, newTz, STRING_RESOURCES_MAX_LENGTH);
        rc = Lwm2mParser_getString(parser_ptr, &strDescr);
        if (rc != RC_OK)
        {
            return (rc);
        }
        if (strlen(newTz) > STRING_LENGTH)
        {
            return (RC_LWM2M_INTERNAL_ERROR);
        }
        strncpy(tz, newTz, strlen(newTz));
        tz[strlen(newTz)] = '\0';
        return (RC_OK);
    }
}*/
/* @brief
 * This function is used to update the getBinding Resource value into the LWM2M Server*

static retcode_t getBindingFunc(Lwm2mSerializer_T *serializer_ptr, Lwm2mParser_T *parser_ptr)
{
    if (parser_ptr != NULL)
    {
        return (RC_LWM2M_METHOD_NOT_ALLOWED);
    }

    char* binding="U";
    uint8_t length = LWM2MOBJECTS_UDP_LENGTH;

    printf("getBinding\r\n");

    switch (deviceResourceInfo.binding)
    {
    case UDP:
        binding = "U";
        length = LWM2MOBJECTS_UDP_LENGTH;
        break;

    case UDP_QUEUED:
        binding = "UQ";
        length = LWM2MOBJECTS_UDP_QUEUED_LENGTH;
        break;

    case SMS:
        binding = "S";
        length = LWM2MOBJECTS_SMS_LENGTH;
        break;

    case SMS_QUEUED:
        binding = "SQ";
        length = LWM2MOBJECTS_SMS_QUEUED_LENGTH;
        break;

    case UDP_AND_SMS:
        binding = "US";
        length = LWM2MOBJECTS_UDP_AND_SMS_LENGTH;
        break;

    case UDP_QUEUED_AND_SMS:
        binding = "UQS";
        length = LWM2MOBJECTS_UDP_QUEUED_AND_SMS_LENGTH;
        break;
    }

    StringDescr_T strDescr;

    StringDescr_set(&strDescr, binding, length);

    return (Lwm2mSerializer_serializeString(serializer_ptr, &strDescr));
}*/

/* TODO need a way to define inactive resources.
struct DeviceResource_S deviceResources =
        {
                { 0, LWM2M_STRING_RO( "Bosch Connected Devices and Solutions GmbH" ) },
                { 1, LWM2M_STRING_RO( "XDK110" ) },
                { 2, LWM2M_STRING_RO( "1" ) },
                { 3, LWM2M_STRING_RO( "1" ) },
                { 4, LWM2M_FUNCTION( deviceRebootFunc ) },
                { 5, LWM2M_FUNCTION( factoryResetFunc ) },
                { 6, LWM2M_INTEGER( 0 ) },
                { 7, LWM2M_INTEGER( 0 ) },
                { 8, LWM2M_INTEGER( 0 ) },
                { 9, LWM2M_DYNAMIC( batteryLevelFunc ) },
                { 10, LWM2M_DYNAMIC( memoryFreeFunc ) },
                { 11, LWM2M_DYNAMIC( errorCodeFunc ) },
                { 12, LWM2M_FUNCTION( resetErrorCodeFunc ) },
                { 13, LWM2M_DYNAMIC( getCurrentTime ) }, //LWM2M_TIME( -6000 ) },
                { 14, LWM2M_STRING_RO( "UTC+2" ) },
                { 15, LWM2M_DYNAMIC( getTimeZone ) },
                { 16, LWM2M_DYNAMIC( getBindingFunc ) },
                };*/
/**@} */
