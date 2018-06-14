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
*
* @brief Configuration header for the lwm2m.c file.
*
* @file
**/


/* header definition ******************************************************** */
#ifndef XDK110_LWM2MCLIENTAPP_CH_H_
#define XDK110_LWM2MCLIENTAPP_CH_H_

/* local interface declaration ********************************************** */
#include "BCDS_Basics.h"
#include "BCDS_CmdProcessor.h"

/* local type and macro definitions */

#define CONNECT_TIME_INTERVAL           UINT32_C(3000)          /**< Macro to represent connect time interval */
#define TIMERBLOCKTIME                  UINT32_C(0xffff)        /**< Macro used to define blocktime of a timer*/
#define ZERO                            UINT8_C(0x00)           /**< Macro to define value zero*/
#define ONE                             UINT8_C(0x01)           /**< Macro to define value one*/
#define TASK_STACK_SIZE                 UINT32_C(2048)          /**< Macro to represent the task stack size */
#define TASK_PRIORITY                   UINT32_C(2)             /**< Macro to represent the task priority */
#warning Please enter WLAN configurations with valid SSID & WPA key in below Macros and remove this line to avoid warnings.
#define WLAN_CONNECT_WPA_SSID           "WIFISSID"         /**< Macros to define WPA/WPA2 network settings */
#define WLAN_CONNECT_WPA_PASS           "WIFIPASS"      /**< Macros to define WPA/WPA2 network settings */
/* Server Connecting Port is user configurable */
#define CLIENT_CONNECT_PORT             UINT32_C(12345)
#if SERVAL_ENABLE_TLS_CLIENT
#define DEST_PORT_NUMBER        		UINT16_C(443)            /**< Macro to define the secure Server Port number */
#else
#define DEST_PORT_NUMBER        		UINT16_C(80)            /**< Macro to define the unsecure Server Port number */
#endif

/* local module global variable declarations */

/* local inline function definitions */
/**
 * @brief This is a template function where the user can write his custom application.
 *
 * @param[in] CmdProcessorHandle Handle of the main commandprocessor
 *
 * @param[in] param2  Currently not used will be used in future
 *
 */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2);

/**
 * @brief This function returns the application command processor handle
 *
 * @retVal returns application command processor handle
 *
 */
CmdProcessor_T * GetAppCmdProcessorHandle(void);

#endif /* XDK110_LWM2MEXAMPLECLIENTAPP_H_ */

/** ************************************************************************* */
