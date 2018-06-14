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
* @defgroup APP_LWM2M_INTERFACE AppLwm2mInterface
* @{
*
* @brief Configuration header for the Lwm2mObjects.c file.
*
* @details Configuration header for the Lwm2mObjects.c file.
*
*
* @file
**/

#ifndef XDK110_APPLWM2MINTERFACE_H_
#define XDK110_APPLWM2MINTERFACE_H_

#if SERVAL_ENABLE_LWM2M
#include <Serval_Network.h>

/* public type and macro definitions */

#define OBJECT_INDEX_VALUE                   INT32_C(0)        /**< Macro used to define the object index*/
#define OBJECT_INSTANCE_INDEX_VALUE          INT32_C(0)        /**< Macro used to define object Instance number*/
#define OBJECT_RESOURCE_NUMBER               INT32_C(13)       /**< Macro used to define the resource number*/
 /**<Macro used to define the server index which can take any value between 0 to (NUMBER_OF_SERVERS -1 ).Here number of servers is configured as 1,
  *  hence to get memory of that server,the index value is passed as 0 */
#define SERVER_INDEX                         UINT32_C(0)
#define NUMBER_OF_SERVERS                    UINT32_C(1)        /**< Macro used to define number of servers*/
/**< Macro used to define the time interval to send notification to the server about the time change*/
#define TIME_CHANGE_NOTIFICATION_DELAY       5000
#define REGISTRATION_UPDATE_DELAY            20000              /**< Macro used to define delay for registration update*/
#define TIMERBLOCKTIME                       UINT32_C(0xffff)   /**< Macro used to define block time of a timer*/
#define MAX_REGISTRATION_UPDATE_RETRY        UINT8_C(10)        /**< Macro used to define maximum retry for registration update*/

/** enum to represent LED return status */
typedef enum led_return_e
{
    RETURN_FAILURE,
    RETURN_SUCCESS,
} led_return_t;

/**
 * @brief The function initializes the LWM2M Interface between Device and the LWM2M Server.
 *
 * @return  RC_OK Interface Initialization happend successfully.
 * @return  RC_SERVAL_ERROR Initialization Failure.
 */
retcode_t Lwm2mInterfaceInitialize(void);

/**
 * @brief The function Starts the LWM2M Interface to the LWM2M Server to the configured port Number.
 * @param[in] port port number of LWM2M Interface between Device and LWM2M Server
 *
 * @return  RC_OK Interface Started between Device and LWM2M Server.
 * @return  RC_SERVAL_ERROR Interface Not started to the Configured Port Number.
 */
retcode_t Lwm2mInterfaceStart(Ip_Port_T port);

#endif /* SERVAL_ENABLE_LWM2M */

#endif /* XDK110_APPLWM2MINTERFACE_H_ */
