/*********************************************************************************
*
*                          Proprietary Information of
*
*                            Precision Systems, Inc.
*                    1355 Business Center Drive, Suite C
*                                 Horsham, PA
*                               (215) 672-1860
*
*                   Copyright (C) 2020, Precision Systems, Inc.
*                            All Rights Reserved
*
*             The information and design as detailed in this document is
*     the property of Precision Systems, Inc., and/or its Associates
*     and must be returned on demand. It is issued on the strict
*     condition that except with our written permission it must not be
*     reproduced, copied or communicated to any third party, nor be
*     used for any purpose other than that stated in the particular
*     inquiry, order or contract with which it is issued. The
*     reservation of copyright in this document extends from each date
*     appearing thereon and in respect of the subject matter as it
*     appeared at the relevant date.
*
********************************************************************************/
/********************************************************************************
*
* PROJECT:     Project MIDAS
*
* COMPILER:    
*
* TOOLS:       Microsoft Visual Studio 2019
*
* DESCRIPTION: Contains functions to build command payloads
*
* NOTES:       None
*
* REVIEWS:     1
*
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "commandBuilder.h"

#define RSP            "RSP"
#define PARAM_FORMAT   "%d,%s="
#define FW_FORMAT      "%X.%X.%X"

/*********************************************************************************
*                        H E L P E R   F U N C T I O N   
*********************************************************************************/
/********************************************************************************
 *
 * Name:    BuildCommandWithNoParameter
 *
 * Purpose: Build command payload with a command only.
 *
 * Inputs:  eId - command code id
 *
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None
 *
 * Notes:   None.
 *
 *******************************************************************************/
void BuildCommandWithNoParameter(ECommandCode eId, MsgPayload* pPayload)
{
   if (pPayload != NULL)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload, "%d", eId);
      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildCommandWithIntParameter
 *
 * Purpose: Build command payload with a single integer parameter
 *
 * Inputs:  eId   - command code id
 *          pTag  - tag name of the parameter
 *
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None
 *
 * Notes:   None.
 *
 *******************************************************************************/
void BuildCommandWithIntParameter(ECommandCode eId, char* pTag, int nValue, MsgPayload* pPayload)
{
   char szFormat[TMP_BUFF_SIZE] = { 0 };
   if (pPayload != NULL && pTag != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      // build message payload
      sprintf(pPayload->szPayload, "%d,%s=%d", eId, pTag, nValue);
      pPayload->nLength = strlen(pPayload->szPayload);

      // compute the checksum and write the data to the buffer
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildCommandWithFloatParameter
 *
 * Purpose: Build command payload with a single float parameter
 *
 * Inputs:  eId   - command code id
 *          pTag  - tag name of the parameter
 *
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None
 *
 * Notes:   None.
 *
 *******************************************************************************/
void BuildCommandWithFloatParameter(ECommandCode eId, char* pTag, float dValue, MsgPayload* pPayload)
{
   if (pPayload != NULL && pTag != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      // build message payload
       sprintf(pPayload->szPayload,  "%d,%s=%f", eId, pTag, dValue);

      pPayload->nLength = strlen(pPayload->szPayload);

      // compute the checksum and write the data to the buffer
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildCommandWithStringParameter
 *
 * Purpose: Build command with a single string parameter.
 *
 * Inputs:  eId      - unique id for the command
 *          pTag     - identifer for the parameter
 *          szValue  - command string parameter
 *          
 * Outputs: pPayload - populated with the command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
void BuildCommandWithStringParameter(ECommandCode eId, char* pTag, const char* szValue, MsgPayload* pPayload)
{
   if (pPayload != NULL && pTag != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      // build message payload
      sprintf(pPayload->szPayload,  "%d,%s=%s", eId, pTag, szValue);

      pPayload->nLength = strlen(pPayload->szPayload);

      // compute the checksum and write the data to the buffer
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}
/*********************************************************************************
*                    F U N C T I O N   D E F I N I T I O N S
*********************************************************************************/

void BuildCommandResponseWithIntParameter(EHandlerResponse eResponse, 
                                          ECommandCode eCode, 
                                          const char* pTag, 
                                          int nValue, 
                                          MsgPayload* pPayload)
{
   // check buffer before processing
   if(pPayload)
   {
      // clear previous data
      // format message payload
      memset(pPayload, 0, sizeof(MsgPayload));

      if (eResponse == EResponseOk)
      {
         sprintf(pPayload->szPayload,
            "%s,%d,%s=%d",
            RESPONSE_PREFIX,
            eCode,
            pTag,
            nValue);
      }
      else
      {
         BuildCommandErrorResponse(eResponse, eCode, pPayload);
      }

      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildCommandEchoResponse
 *
 * Purpose: Builds generic command response. Prefixes RSP to original payload.
 *
 * Inputs:  eResponse      - the response to the received command
 *          pReceived      - received command payload
 *          pLowErroMsg    - error string for parameter too low response
 *          pHighErrorMsg  - pHighErrorMsg  
 *
 * Outputs: pResponse     - command response payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildCommandEchoResponse(EHandlerResponse eResponse, ECommandCode eCode, MsgPayload* pReceived, MsgPayload* pResponse)
{
  
   if(pReceived != NULL && pResponse != NULL)
   {
      if (eResponse == EResponseOk)
      {
         if (pReceived->nLength + strlen(RESPONSE_PREFIX) < PAYLOAD_LENGTH)
         {
            sprintf(pResponse->szPayload,  "%s,%s", RESPONSE_PREFIX, pReceived->szPayload);
            pResponse->nLength   = strlen(pResponse->szPayload);
            pResponse->nChecksum = CalculateChecksum(pResponse);
         }
      }
      else
      {
         BuildCommandErrorResponse(eResponse, eCode, pResponse);
      }
   }
}

/********************************************************************************
 *
 * Name:    BuildCommandErrorResponse
 *
 * Purpose: Build generic command error response.
 *
 * Inputs:  eResponse      - the response to the received command
 *          pReceived      - received command payload
 *          pLowErroMsg    - error string for parameter too low response
 *          pHighErrorMsg  - pHighErrorMsg  
 *
 * Outputs: pResponse     - command response payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildCommandErrorResponse(EHandlerResponse eResponse,  ECommandCode eCode, MsgPayload* pResponse)
{
   // check buffer before processing
   if(pResponse)
   {
      // clear previous data
      // format response
      memset(pResponse, 0, sizeof(MsgPayload));

      sprintf(pResponse->szPayload,
         "%s,%d,%s=%d",
         RESPONSE_PREFIX,
         (int)eCode,
         ERROR_PREFIX,
         (int)eResponse);

      pResponse->nLength = strlen(pResponse->szPayload);
      pResponse->nChecksum = CalculateChecksum(pResponse);
   }
}


/********************************************************************************
 *
 * Name:    BuildGetFirmwareInfoCommandResponse
 *
 * Purpose: Build payload for the get firmware info command
 *
 * Inputs:  pInfo - contains firmware information for all system components
 *          
 * Outputs: pPayload - populated with command payload, length & checksum.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetFirmwareInfoCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetFirmwareInfo, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetFirmwareInfoCommandResponse
 *
 * Purpose: Build payload for the get firmware info command
 *
 * Inputs:  pInfo - contains firmware information for all system components
 *          
 * Outputs: pPayload - populated with command payload, length & checksum.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetFirmwareInfoCommandResponse(EHandlerResponse eResponse, FirmwareInfo* pInfo, MsgPayload* pPayload)
{
   char szMainFW[10] = { 0 };
   char szSoft[10] = { 0 };
   char szBt[10] = { 0 };
   char szGui[10] = { 0 };
   char szScavenger[10] = { 0 };

   if (pInfo != NULL && pPayload != NULL)
   {
      memset(pPayload, 0, sizeof(MsgPayload));

      if (eResponse == EResponseOk)
      {
         sprintf(szMainFW, 
            "%X.%X.%X", 
            pInfo->mainController.nMajor,
            pInfo->mainController.nMinor,
            pInfo->mainController.nRevision);

         sprintf(szBt, 
            "%X.%X.%X", 
            pInfo->blueTooth.nMajor,
            pInfo->blueTooth.nMinor,
            pInfo->blueTooth.nRevision);

         sprintf(szGui, 
            "%X.%X.%X", 
            pInfo->gui.nMajor,
            pInfo->gui.nMinor,
            pInfo->gui.nRevision);

         sprintf(szScavenger, 
            "%X.%X.%X", 
            pInfo->scavenger.nMajor,
            pInfo->scavenger.nMinor,
            pInfo->scavenger.nRevision);


         // format command payload
         sprintf(pPayload->szPayload,
            "RSP,%d,%s=%s,%s=%s,%s=%s,%s=%s",
            EGetFirmwareInfo,
            TAG_FW_MAIN,
            szMainFW,
            TAG_FW_BT,
            szBt,
            TAG_FW_GUI,
            szGui,
            TAG_FW_SCAV,
            szScavenger);
      }
      else
      {
         BuildCommandErrorResponse(eResponse, EGetFirmwareInfo, pPayload);
      }

      pPayload->nLength    = strlen(pPayload->szPayload);
      pPayload->nChecksum  = CalculateChecksum(pPayload);
   }
}
/********************************************************************************
 *
 * Name:    BuildVacuumIncreaseCommand
 *
 * Purpose: Builds vacuumIncrease command payload
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with message text, length and checksum.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildVacuumIncreaseCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EVacuumIncrease, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildVacuumDecreaseCommand
 *
 * Purpose: Build command payload for the vacuum decrease command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command paylaod
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildVacuumDecreaseCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EVacuumDecrease, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildEnableDisablePinCommand
 *
 * Purpose: Builds payload for the toggle PIN command.
 *
 * Inputs:  bEnable  = enable/disable flag for security PIN
 *          pPayload = contains payload, length and checksum for message.
 *
 * Outputs: None.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEnableDisablePinCommand(bool bEnabled, MsgPayload* pPayload)
{
   // convert boolean to integer and build command
   int nEnabled = bEnabled ? ENABLED : DISABLED;
   BuildCommandWithIntParameter(EEnableDisablePin, TAG_TOGGLE_PIN, nEnabled, pPayload);  
}

/********************************************************************************
 *
 * Name:    BuildChangePinCommand
 *
 * Purpose: Builds payload for the change PIN command.
 *
 * Inputs:  pszNewPin - string for new PIN
 *          pPayload = contains payload, length and checksum for message.
 *
 * Outputs: None.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildChangePinCommand(char* pszNewPin, MsgPayload* pPayload)
{
   BuildCommandWithStringParameter(EChangePin, TAG_CHANGE_PIN, pszNewPin, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildStartProcedureCommand
 *
 * Purpose: Builds payload for the start procedure command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - contains the payload for command message.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildStartProcedureCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EStartProcedure, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildEndProcedureCommand
 *
 * Purpose: Builds payload for the end procedure command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - contains the payload for command message.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEndProcedureCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EEndProcedure, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetProcedureEntryCommand
 *
 * Purpose: Builds payload for get procedure entry command
 *
 * Inputs:  nIndex - index of requested entry in procedure list command response.
 *
 * Outputs: pPayload - Populated with message payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureEntryCommand(size_t nIndex, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(EGetProcedureEntry, "IDX", nIndex, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetAlarmLogListCommand
 *
 * Purpose: Builds payload for get system log list command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - Populated with message payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetAlarmLogListCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetAlarmLogList, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetAlarmLogEntryCommand
 *
 * Purpose: Builds payload for get alarm log entry command.
 *
 * Inputs:  None.
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetAlarmLogEntryCommand(size_t nIndex, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(EGetAlarmLogEntry, TAG_LOG_IDX, nIndex, pPayload);
}


/********************************************************************************
 *
 * Name:    BuildResetLogsCommand
 *
 * Purpose: Builds reset logs command payload
 *
 * Inputs:  eLogId - unique identifier for log types
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildResetLogsCommand(ELogId eLogId, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(EResetLogs, TAG_LOG_IDX, eLogId, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildSetLanguageCommand
 *
 * Purpose: Builds the set language command payload
 *
 * Inputs:  eLanguage - language selection enum
 *          
 * Outputs: pPayload  - populated with command payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetLanguageCommand(ELanguage eLanguage, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(ESetLanguage, TAG_SET_LANGUAGE, eLanguage, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetLanguageCommand
 *
 * Purpose: Build payload for the get language command.
 *
 * Inputs:  None.
 *
 * Outputs: pPayload - populated with command payload, length & checksum.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetLanguageCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetLanguage, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetLanguageCommandResponse
 *
 * Purpose: Build payload for the get language command response.
 *
 * Inputs:  eLang - current language setting.
 *
 * Outputs: pPayload - populated with command payload, length & checksum.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetLanguageCommandResponse(EHandlerResponse eResponse, ELanguage eLang, MsgPayload* pPayload)
{
   // build commands with a single Int parameter
   BuildCommandResponseWithIntParameter(eResponse, EGetLanguage, TAG_LANGUAGE, eLang, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildSetTotalFlowRateCommand
 *
 * Purpose: Builds payload for the set total flow rate command
 *
 * Inputs:  dFlowRate
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   There is no value checking when building the payload.
 *          Error checking happens in the command handler.
 *
 *******************************************************************************/
LIB_API
void BuildSetTotalFlowRateCommand(int nFlowRate, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(ESetTotalFlowRate, TAG_FLOW_RATE, nFlowRate, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetTotalFlowRateCommand
 *
 * Purpose: Builds payload for the set total flow rate command
 *
 * Inputs:  dFlowRate
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   There is no value checking when building the payload.
 *          Error checking happens in the command handler.
 *
 *******************************************************************************/
LIB_API
void BuildGetTotalFlowRateCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetTotalFlowRate, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetFlowRatesCommand
 *
 * Purpose: Builds payload for the get flow rates command
 *
 * Inputs:  None
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   There is no value checking when building the payload.
 *          Error checking happens in the command handler.
 *
 *******************************************************************************/
LIB_API
void BuildGetFlowRatesCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetFlowRates, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetFlowRatesCommandResponse
 *
 * Purpose: Builds payload for the get flow rates command response
 *
 * Inputs:  None
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   There is no value checking when building the payload.
 *          Error checking happens in the command handler.
 *
 *******************************************************************************/
LIB_API
void BuildGetFlowRatesCommandResponse(EHandlerResponse eResponse, 
                                      int nO2FlowRate, 
                                      int nN2OFlowRate,
                                      int nScavengerFlowRate, 
                                      MsgPayload* pPayload)
{
   if(pPayload)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      if (eResponse == EResponseOk)
      {
         sprintf(pPayload->szPayload, 
            "%s,%d,%s=%d,%s=%d,%s=%d",
            RESPONSE_PREFIX,
            EGetFlowRates,
            TAG_O2_FLOW,
            nO2FlowRate,
            TAG_N2O_FLOW,
            nN2OFlowRate,
            TAG_SCAV_FLOW,
            nScavengerFlowRate);
      }
      else
      {
         BuildCommandErrorResponse(eResponse, EGetFlowRates, pPayload);
      }
      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetTotalFlowRateCommandResponse
 *
 * Purpose: Builds payload for the get total flow rate command response.
 *
 * Inputs:  eResponse - response from command handler
 *          eCode     - the command code
 *
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   There is no value checking when building the payload.
 *          Error checking happens in the command handler.
 *
 *******************************************************************************/
LIB_API
void BuildGetTotalFlowRateCommandResponse(EHandlerResponse eResponse, 
                                          int flowRate,
                                          MsgPayload* pPayload)
{
   // check buffer before processing
   if(pPayload)
   {
      // clear previous data
      // format message payload
      memset(pPayload, 0, sizeof(MsgPayload));
      if (eResponse == EResponseOk)
      {
         sprintf(pPayload->szPayload,
            "%s,%d,%s=%d",
            RESPONSE_PREFIX,
            EGetTotalFlowRate,
            TAG_FLOW_RATE,
            flowRate);
      }
      else
      {
         BuildCommandErrorResponse(eResponse, EGetTotalFlowRate, pPayload);
      }

      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildSetO2MixCommand
 *
 * Purpose: Builds payload for the set O2 mix percentage command.
 *
 * Inputs:  nPercentage - new O2 percentage. Should be between 30 - 100
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   There is no value checking when building the payload.
 *          Error checking happens in the command handler.
 *
 *******************************************************************************/
LIB_API
void BuildSetO2MixCommand(int nPercentage, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(ESetO2MixPercentage, "O2", nPercentage, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetO2MixCommand
 *
 * Purpose: Builds payload for the set O2 mix percentage command.
 *
 * Inputs:  None
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetO2MixCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetO2MixPercentage, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetO2MixCommandResponse
 *
 * Purpose: Builds payload for the set O2 mix percentage command response.
 *
 * Inputs:  eResponse - the response from command handler
 *          nPercentage - the O2 mix percentage
 *
 * Outputs: pPayload    - buffer is populated with the command code, 
 *                        new percentage, message length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetO2MixCommandResponse(EHandlerResponse eResponse, int nPercentage, MsgPayload* pPayload)
{
   BuildCommandResponseWithIntParameter(eResponse, EGetO2MixPercentage, TAG_O2_MIX, nPercentage, pPayload);
}


/********************************************************************************
 *
 * Name:    BuildStopGasCommand
 *
 * Purpose: Builds payload for the stop gas command.
 *
 * Inputs:  None
 *          
 * Outputs: pPayload    - buffer is populated with the command code
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildStopGasCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EStopGas, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildEnableDisableBTCommand
 *
 * Purpose: Builds toggle BT command payload.
 *
 * Inputs:  bEnable - enable/disable flag
 *          
 * Outputs: pPayload - buffer is populated with the command code
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEnableDisableBTCommand(bool bEnabled, MsgPayload* pPayload)
{
   int nEnabled = bEnabled ? ENABLED : DISABLED;
   BuildCommandWithIntParameter(EEnableDisableBT, "BT_EN", nEnabled, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildRestoreDefaultsCommand
 *
 * Purpose: Builds restore defaults command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - buffer is populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildRestoreDefaultsCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(ERestoreDefaults, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildHeartbeatCommand
 *
 * Purpose: Builds heartbeat command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with command payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildHeartbeatCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EHeartbeat, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildSyncDataCommand
 *
 * Purpose: Builds sync data command payload.
 *
 * Inputs:  pSyncData - reference to structure with sync data parameters
 *          
 * Outputs: pPayload - populated with command payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSyncDataCommand(SyncDataInfo* pSyncData, MsgPayload* pPayload)
{
   int nPowerState = pSyncData->bTouchscreenPowerState ? 1 : 0;
   int nInProgress = pSyncData->bInProgress ? 1 : 0;
   int nStopGas    = pSyncData->bStopGas ? 1 : 0;
   int nEnding     = pSyncData->bEnding ? 1 : 0;

   if (pPayload != NULL && pSyncData != NULL)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload,
         "%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d", 
         ESyncData,
         TAG_O2_MIX,
         pSyncData->nO2MixPercentage,
         TAG_FLOW_RATE,
         pSyncData->nTotalFlowRate,
         TAG_SCAV_FLOW,
         pSyncData->nScavengerFlowRate,
         TAG_TS_POWER,
         nPowerState,
         TAG_IN_PROGRESS,
         nInProgress,
         TAG_ENDING,
         nEnding,
         TAG_STOP_GAS,
         nStopGas,
         TAG_LANGUAGE,
         pSyncData->eDefaultLanguage);

      pPayload->nLength   = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetFirmwareVersionCommand
 *
 * Purpose: Build payload for the get firmware version command
 *
 * Inputs:  pVersion - firmware version info
 *          
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetFirmwareVersionCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetFirmwareVersion, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetFirmwareVersionCommandResponse
 *
 * Purpose: Build payload for the get firmware version command
 *
 * Inputs:  pVersion - firmware version info
 *          
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetFirmwareVersionCommandResponse(EHandlerResponse eResponse, FirmwareVersion* pVersion, MsgPayload* pPayload)
{
   // check values and buffer before processing
   if(pPayload != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      if (eResponse == EResponseOk)
      {
         // format the payload with the command code and FW version information
       sprintf(pPayload->szPayload,
            "RSP,%d,%s=%X.%X.%X",
            EGetFirmwareVersion,
            TAG_FW_VERSION,
            pVersion->nMajor,
            pVersion->nMinor,
            pVersion->nRevision);
      }
      else
      {
         BuildCommandErrorResponse(eResponse, EGetFirmwareVersion, pPayload);
      }

      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetConfigurationDataCommand
 *
 * Purpose: Builds get configuration data command payload
 *
 * Inputs:  None.
 *          
 * Outputs: pPayload - populated with command payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetConfigurationDataCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetConfigData, pPayload);
}


/********************************************************************************
 *
 * Name:    BuildGetConfigurationDataCommand
 *
 * Purpose: Builds get configuration data command payload
 *
 * Inputs:  None.
 *          
 * Outputs: pPayload - populated with command payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetConfigurationDataCommandResponse(EHandlerResponse eResponse, ConfigData* pConfig, MsgPayload* pPayload)
{

      if (pConfig != NULL
      && pPayload != NULL)
      {
         memset(pPayload, 0, sizeof(MsgPayload));
         memset(pPayload, 0, sizeof(MsgPayload));

         if (eResponse == EResponseOk)
         {
            sprintf(pPayload->szPayload,
               "%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%s",
               EGetConfigData,
               TAG_N2O_MAX,
               pConfig->nMaxN20,
               TAG_MIX_STEP,
               pConfig->eMixStepSize,
               TAG_FLOW_STEP,
               pConfig->eFlowStepSize,
               TAG_CLOCK_FORMAT,
               pConfig->eClockFormat,
               TAG_LANGUAGE,
               pConfig->eLanguage,
               TAG_T_AND_D,
               pConfig->szTime);
         }
         else
         {
            BuildCommandErrorResponse(eResponse, EGetConfigData, pPayload);
         }

         pPayload->nLength    = strlen(pPayload->szPayload);
         pPayload->nChecksum = CalculateChecksum(pPayload);
      }
 

}

/********************************************************************************
 *
 * Name:    BuildSetConfigurationDataCommand
 *
 * Purpose: Builds set configuration data command payload
 *
 * Inputs:  pConfig - reference to config parameters
 *          
 * Outputs: pPayload - populated with the command payload.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetConfigurationDataCommand(ConfigData* pConfig, MsgPayload* pPayload)
{
   if(pPayload != NULL && pConfig != NULL)
   {
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload,
         "%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%s",
         EGetConfigData,
         TAG_N2O_MAX,
         pConfig->nMaxN20,
         TAG_MIX_STEP,
         pConfig->eMixStepSize,
         TAG_FLOW_STEP,
         pConfig->eFlowStepSize,
         TAG_CLOCK_FORMAT,
         pConfig->eClockFormat,
         TAG_LANGUAGE,
         pConfig->eLanguage,
         TAG_T_AND_D,
         pConfig->szTime);

      pPayload->nLength    = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetTimeAndDateCommand
 *
 * Purpose: Gets time and date
 *
 * Inputs:  
 *          
 * Outputs: None
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetTimeAndDateCommand(MsgPayload* pPayload)
{
	BuildCommandWithNoParameter(EGetTimeAndDate, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetTimeAndDateCommandResponse
 *
 * Purpose: Builds response for get time & date command
 *
 * Inputs:  eResponse - response from command handler
 *          pDateTime - time & date returned from command handler
 *
 * Outputs: pPayload - populated with command response payload
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetTimeAndDateCommandResponse(EHandlerResponse eResponse, char* pDateTime, MsgPayload* pPayload)
{
   if (pPayload != NULL && pDateTime != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      // build message payload
      sprintf(pPayload->szPayload,  "RSP,%d,%s=%s", EGetTimeAndDate, TAG_T_AND_D, pDateTime);

      pPayload->nLength = strlen(pPayload->szPayload);

      // compute the checksum and write the data to the buffer
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildSetTimeAndDateCommand
 *
 * Purpose: Builds the set time and date
 *
 * Inputs:  pDateTime - time and date formatted as MM/dd/yyyy hh:mm tt"
 *          
 * Outputs: None
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetTimeAndDateCommand(const char* pDateTime, MsgPayload* pPayload)
{
   BuildCommandWithStringParameter(ESetTimeAndDate, TAG_T_AND_D, pDateTime, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildSetValveCommand
 *
 * Purpose: Builds set valve command payload
 *
 * Inputs:  nPosition - valve position 0 -100. 0 = Fully closed, 100 = fully open
 *          
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetValveCommand(int nPosition, EGasId eId, MsgPayload* pPayload)
{
   if (pPayload != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      // build message payload
      sprintf(pPayload->szPayload,
         "%d,%s=%d,%s=%d",
         ESetValve,
         TAG_GAS_SELECTION,
         eId,
         TAG_VALVE_POS,
         nPosition);

      pPayload->nLength = strlen(pPayload->szPayload);

      // compute the checksum and write the data to the buffer
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetValveCommand
 *
 * Purpose: Builds get valve command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetValveCommand(EGasId eId, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(EGetValve, TAG_GAS_SELECTION, eId, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetValveCommand
 *
 * Purpose: Builds get valve command response payload
 *
 * Inputs:  eResponse - command handler response code
 *          nPosition - valve position between 0 - 100
 *          
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetValveCommandResponse(EHandlerResponse eResponse, EGasId eId, int nPosition, MsgPayload* pPayload)
{
   // check buffer before processing
   if(pPayload)
   {
      // clear previous data
      // format message payload
      memset(pPayload, 0, sizeof(MsgPayload));

      if (eResponse == EResponseOk)
      {
         sprintf(pPayload->szPayload,
            "%s,%d,%s=%d,%s=%d",
            RESPONSE_PREFIX,
            EGetValve,
            TAG_GAS_SELECTION,
            eId,
            TAG_VALVE_POS,
            nPosition);
      }
      else
      {
         BuildCommandErrorResponse(eResponse, EGetValve, pPayload);
      }

      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildFirmwareDownloadCommand
 *
 * Purpose: Builds firmware download command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildFirmwareDownloadCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EFirmwareDownload, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildEnableGasFlowCommand
 *
 * Purpose: Builds enable gas flow command payload
 *
 * Inputs:  None
 *          
 * Outputs: None
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEnableGasFlowCommand(EGasId eId, bool bEnabled, MsgPayload* pPayload)
{
   const int nEnabled = bEnabled ? ENABLED : DISABLED;

   char szFormat[TMP_BUFF_SIZE] = { 0 };

   if (pPayload != NULL)
   {
      // clear buffer
      memset(pPayload, 0, sizeof(MsgPayload));

      // build message payload
      sprintf(pPayload->szPayload,  
         "%d,%s=%d,%s=%d", 
         EEnableGasFlow,
         TAG_GAS_SELECTION,
         eId,
         TAG_ENABLE_GAS_FLOW,
         nEnabled);

      pPayload->nLength = strlen(pPayload->szPayload);

      // compute the checksum and write the data to the buffer
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildBTFirmwareDownloadCommand
 *
 * Purpose: Builds bt firmware download command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildBTFirmwareDownloadCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EBtFirmwareDownload, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildWriteManufacturerFieldCommand
 *
 * Purpose: Builds write manufacturer field command payload
 *
 * Inputs:  pField - the name of the field to write
 *          pValue - field value to write
 *          
 * Outputs: pPayload - populated with command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildWriteManufacturerFieldCommand(const char* pField, const char* pValue, MsgPayload* pPayload)
{
   // check pointers before formatting payload
   if(pPayload != NULL
   && pField   != NULL 
   && pValue   != NULL)
   {
      // clear buffer and format the command payload
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload,
         "%d,%s=%s",
         EWriteManufacturerField,
         pField,
         pValue);

      pPayload->nLength    = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildReadManufacturerFieldCommand
 *
 * Purpose: Builds the read manufacturer field command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with the command payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildReadManufacturerFieldCommand(const char* pField, MsgPayload* pPayload)
{
   if(pField   != NULL
   && pPayload != NULL)
   {
      sprintf(pPayload->szPayload,
         "%d,%s=%s",
         EReadManufacturerField,
         TAG_MF_FIELD,
         pField);

      pPayload->nLength   = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildReadManufacturerFieldCommandResponse
 *
 * Purpose: Builds the read manufacturer field command response payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with the command response payload
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildReadManufacturerFieldCommandResponse(EHandlerResponse eResponse, 
                                               const char* pValue, 
                                               MsgPayload* pPayload, 
                                               MsgPayload* pResponse)
{
   char* pReader = pPayload->szPayload;
   char szFieldName[TMP_STR_SIZE] = { 0 };

   if (eResponse == EResponseOk)
   {
      if (pPayload  != NULL
      &&  pResponse != NULL
      &&  pValue    != NULL)
      {
         // tokenize, split at the ,
          StrTokenize(pReader, ",");

         // skip the command code
         // advance pointer to the inserted NULL
         RestoreDelimiterAndMovePast(&pReader, ',');
      
         // split at the =
         StrTokenize(pReader, "=");

         RestoreDelimiterAndMovePast(&pReader, '=');

         // copy tag
         strcpy(szFieldName, pReader);

         memset(pResponse, 0, sizeof(MsgPayload));

         sprintf(pResponse->szPayload,
            "%s,%d,%s=%s",
            RESPONSE_PREFIX,
            EReadManufacturerField,
            szFieldName,
            pValue);

         pResponse->nLength = strlen(pResponse->szPayload);
         pResponse->nChecksum = CalculateChecksum(pResponse);

      }
   }
   else
   {
      // TODO: Build error response
   }
}

/********************************************************************************
 *
 * Name:    BuildAckCommand
 *
 * Purpose: Builds an ACK command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildAckCommand(MsgPayload* pPayload)
{
   if (pPayload != NULL)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload, "%c", (char)EAck);
      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildNakCommand
 *
 * Purpose: Builds an NAK command payload
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildNakCommand(int ErrorCode, MsgPayload* pPayload)
{
   if (pPayload != NULL)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload, "%c,%d", (char)ENak, ErrorCode);
      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildEnableDisableVacuumCommand
 *
 * Purpose: Builds the toggle vacuum command payload
 *
 * Inputs:  bEnabled - enable/disable scavenger vacuum.
 *          
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEnableDisableVacuumCommand(bool bEnabled, MsgPayload* pPayload)
{
   int nEnabled = bEnabled ? ENABLED : DISABLED;
   BuildCommandWithIntParameter(EEnableDisableVacuum, TAG_SCAV_ENABLED, nEnabled, pPayload);
}


/********************************************************************************
 *
 * Name:    BuildScreenReadyCommand
 *
 * Purpose: Builds screen ready command payload
 *
 * Inputs:  pScreenReady - reference to structure with parameter to send
 *          
 * Outputs: pPayload - 
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildScreenReadyCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EScreenReady, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildScreenReadyCommandResponse
 *
 * Purpose: Builds screen ready command response payload
 *
 * Inputs:  eResponse - the response from the command handler.
 *          pScreenReady - reference to structure with parameter to send
 *          
 * Outputs: pPayload - populated with command response.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildScreenReadyCommandResponse(EHandlerResponse eResponse, ScreenReady* pScreenReady, MsgPayload* pPayload)
{
   int nPowerState = 0;

   if (pPayload != NULL && pScreenReady != NULL)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      nPowerState = pScreenReady->bTouchscreenPowerState ? 1 : 0;

      sprintf(pPayload->szPayload,
         "%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%s,%s=%d", 
         EScreenReady,
         TAG_N2O_MAX,
         pScreenReady->configData.nMaxN20,
         TAG_MIX_STEP,
         pScreenReady->configData.eMixStepSize,
         TAG_FLOW_STEP,
         pScreenReady->configData.eFlowStepSize,
         TAG_CLOCK_FORMAT,
         pScreenReady->configData.eClockFormat,
         TAG_LANGUAGE,
         pScreenReady->configData.eLanguage,
         TAG_T_AND_D,
         pScreenReady->configData.szTime,
         TAG_TS_POWER,
         nPowerState);

      pPayload->nLength    = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildMuteAlarmCommand
 *
 * Purpose: Builds payload for the mute alarm command
 *
 * Inputs:  None
 *          
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   Mutes the audible beep of an active alarm.
 *
 *******************************************************************************/
LIB_API
void BuildMuteAlarmCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EMuteAlarm, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildEnableDisableScreenPowerCommand
 *
 * Purpose: Builds payload for the toggle screen power command
 *
 * Inputs:  bEnabled - turns touchscreen on/off
 *          
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEnableDisableScreenPowerCommand(bool bEnabled, MsgPayload* pPayload)
{
   int nEnabled = bEnabled ? ENABLED : DISABLED;
   BuildCommandWithIntParameter(EEnableDisablePower, TAG_TS_POWER, nEnabled, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetScavengerInfoCommand
 *
 * Purpose: Build payload for hte get scavenger info command
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetScavengerInfoCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetScavengerInfo, pPayload);
}
/********************************************************************************
 *
 * Name:    BuildGetScavengerInfoCommandResponse
 *
 * Purpose: Builds get scavenger info command response.
 *
 * Inputs:  eResponse - response from command handler
 *          pInfo     - scavenger info data for response payload
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetScavengerInfoCommandResponse(EHandlerResponse eResponse, ScavengerInfo* pInfo, MsgPayload* pPayload)
{
   int nValveOpen    = 0;
   int nSensorStatus = 0;

   // check buffers before processing
   if(pInfo != NULL && pPayload != NULL)
   {
      if (eResponse == EResponseOk)
      {
         nValveOpen = pInfo->bValveOpen == true ? 1 : 0;
         nSensorStatus = pInfo->bSensorStatus == true ? 1 : 0;

         memset(pPayload, 0, sizeof(MsgPayload));
         sprintf(pPayload->szPayload, 
            "RSP,%d,%s=%d,%s=%d,%s=%d",
            EGetScavengerInfo,
            TAG_SCAV_VALVE,
            nValveOpen,
            TAG_SCAV_SENSOR,
            nSensorStatus,
            TAG_SCAV_FLOW,
            pInfo->nFlowRate);

         pPayload->nLength = strlen(pPayload->szPayload);
         pPayload->nChecksum = CalculateChecksum(pPayload);
      }
      else
      {
         // TODO: prepare error response
         
      }
   }
}


/********************************************************************************
 *
 * Name:    BuildGetGasVolumeInfoCommand
 *
 * Purpose: Build payload for hte get scavenger info command
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetGasVolumeInfoCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetGasVolume, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetGasVolumeInfoCommand
 *
 * Purpose: Build payload for hte get scavenger info command
 *
 * Inputs:  eResponse - response from command handler
 *          pInfo     - gas volume info for response payload
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetGasVolumeInfoCommandResponse(EHandlerResponse eResponse, GasVolumeInfo* pInfo, MsgPayload* pPayload)
{
   if(pInfo != NULL && pPayload != NULL)
   {
      if (eResponse == EResponseOk)
      {
         memset(pPayload, 0, sizeof(MsgPayload));
         sprintf(pPayload->szPayload, 
            "RSP,%d,%s=%d,%s=%d,%s=%s,%s=%s",
            EGetGasVolume,
            TAG_GV_O2,
            pInfo->nO2VolumeDispensed,
            TAG_GV_N2O,
            pInfo->nN2OVolumeDispensed,
            TAG_GV_RESET_O2,
            pInfo->szO2LastReset,
            TAG_GV_RESET_N2O,
            pInfo->szN2OLastReset);

         pPayload->nLength = strlen(pPayload->szPayload);
         pPayload->nChecksum = CalculateChecksum(pPayload);
      }
      else
      {
         // TODO: build error response
      }
   }
}

/********************************************************************************
 *
 * Name:    BuildResetGasVolumeInfoCommand
 *
 * Purpose: Build payload for the reset gas volume info command
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildResetGasVolumeInfoCommand(EGasId eId, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(EResetGasVolume, TAG_GAS_SELECTION, eId, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildFlushO2Command
 *
 * Purpose: Build payload for the flush O2 command.
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildFlushO2Command(bool bEnabled, MsgPayload* pPayload)
{
   int nEnabled = bEnabled ? ENABLED : DISABLED;
   BuildCommandWithIntParameter(EFlushO2, TAG_FLUSH_O2, nEnabled, pPayload);
}

/********************************************************************************
 *
 * Name:    SetLogDateTime
 *
 * Purpose: Sets the 6 byte date & time for procedure log info.
 *
 * Inputs:  year     - the current year   (0-99)
 *          month    - the current month  (1-12)
 *          day      - the current day    (1-31)
 *          hour     - the current hour   (0-23)
 *          min      - the current minute (0-59)
 *          second   - the current second (0-59)
 *
 * Outputs: pBuffer  - populated with 6 bytes of date time data
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void SetLogDateTime(uint8_t year, 
                    uint8_t month, 
                    uint8_t day, 
                    uint8_t hour, 
                    uint8_t min, 
                    uint8_t second, 
                    char* pBuffer)
{
   if (pBuffer != NULL)
   {
      pBuffer[0] = year;
      pBuffer[1] = month;
      pBuffer[2] = day;
      pBuffer[3] = hour;
      pBuffer[4] = min;
      pBuffer[5] = second;
   }
}
/********************************************************************************
 *
 * Name:    BuildGetProcedureLogCount
 *
 * Purpose: Build payload for get procedure log count command
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogCount(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetProcedureLogCount, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetCountResponse
 *
 * Purpose: Build payload for get procedure log count command
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogCountResponse(EHandlerResponse eResponse, int nCount, MsgPayload* pPayload)
{
   BuildCommandResponseWithIntParameter(eResponse, EGetProcedureLogCount, TAG_P_LOG_COUNT, nCount, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetProcedureLogList
 *
 * Purpose: Builds payload to request procedure logs from the controller.
 *
 * Inputs:  nOffset - 
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogList(int nOffset, int nCount, MsgPayload* pPayload)
{
   if(pPayload != NULL)
   {
      memset(pPayload, 0, sizeof(MsgPayload));
      sprintf(pPayload->szPayload,
        "%d,%s=%d,%s=%d",
         EGetProcedureList,
         TAG_OFFSET,
         nOffset,
         TAG_COUNT,
         nCount);

      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}
      
/********************************************************************************
 *
 * Name:    BuildGetProcedureLogListResponse
 *
 * Purpose: Builds payload for the ger procedure log list response.
 *          Returns the requested number of procedure logs and a count for the
 *          number of logs actually returned.
 *
 * Inputs:  eResponse   - command handler response code
 *          nCount      - the number of records in the response
 *          pEntries    - pointer to array of nCount number of records.
 *
 * Outputs: TODO:
 *
 * Returns: TODO:
 *
 * Notes:   TODO:
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogListResponse(EHandlerResponse eResponse, int nCount, ProcedureLog* pEntries, MsgPayload* pPayload)
{
   char szEntries[PAYLOAD_LENGTH] = { 0 };
   char* pWriter = szEntries;
   int nBytesWritten = 0;
   int nTotalWritten = 0;
   int nIdx = 0;

   // check buffers before processing
   if(eResponse == EResponseOk)
   {
      if (pPayload != NULL && pEntries != NULL)
      {
         // clear previous data and format response message
         memset(pPayload, 0, sizeof(MsgPayload));

         // for each entry
         for(nIdx = 0 ; nIdx < nCount ; nIdx++)
         {
            // Serialize the data
            nBytesWritten = sprintf(pWriter,
               "{%s,%d,%d,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu},",
               pEntries[nIdx].szName,
               pEntries[nIdx].nDuration,
               pEntries[nIdx].nEntryCount,
               pEntries[nIdx].szDate[0],
               pEntries[nIdx].szDate[1],
               pEntries[nIdx].szDate[2],
               pEntries[nIdx].szDate[3],
               pEntries[nIdx].szDate[4],
               pEntries[nIdx].szDate[5]);

            pWriter += nBytesWritten;
            nTotalWritten += nBytesWritten;
         }
         ;

         if (nTotalWritten > 0)
         {
            // remove trailing comma
            *(pWriter - 1) = '\0';
         }
        

         // format the message payload
         sprintf(pPayload->szPayload,
            "%s,%d,%s=%d,%s=%s",
            RESPONSE_PREFIX,
            EGetProcedureList,
            TAG_COUNT,
            nCount,
            TAG_LOG_ENTRIES,
            szEntries);

         pPayload->nLength = strlen(pPayload->szPayload);
         pPayload->nChecksum = CalculateChecksum(pPayload);
      }
   }
   else
   {
      BuildCommandErrorResponse(eResponse, EGetProcedureEntryList, pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetProcedureLogCommand
 *
 * Purpose: Build payload for get procedure log command
 *
 * Inputs:  None  
 *
 * Outputs: pPayload - populated with the command payload, length & checksum
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogCommand(size_t nIndex, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(EGetProcedureLog, TAG_INDEX, nIndex, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetProcedureLogEntryListCommand
 *
 * Purpose: Builds payload for the start procedure command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - contains the payload for command message.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogEntryListCommand(int nIndex, int nOffset, int nCount, MsgPayload* pPayload)
{
   if (pPayload != NULL)
   {
      // clear buffer
      // format message
      // calculate length & checksum
      memset(pPayload, 0, sizeof(MsgPayload));

      sprintf(pPayload->szPayload, 
         "%d,%s=%d,%s=%d,%s=%d", 
         EGetProcedureEntryList,
         TAG_INDEX,
         nIndex,
         TAG_OFFSET,
         nOffset,
         TAG_COUNT,
         nCount);

      pPayload->nLength = strlen(pPayload->szPayload);
      pPayload->nChecksum = CalculateChecksum(pPayload);
   }
}

/********************************************************************************
 *
 * Name:    BuildGetProcedureLogListCommandResponse
 *
 * Purpose: Builds payload for the start procedure command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - contains the payload for command message.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetProcedureLogEntryListCommandResponse(EHandlerResponse eResponse, int nCount, LogEntry* pEntries, MsgPayload* pPayload)
{
   char szEntries[PAYLOAD_LENGTH] = { 0 };
   char* pWriter = szEntries;
   int nBytesWritten = 0;
   int nTotalWritten = 0;
   int nIdx = 0;

   // check buffers before processing
   if(eResponse == EResponseOk)
   {
      if (pPayload != NULL && pEntries != NULL)
      {
         // clear previous data and format response message
         memset(pPayload, 0, sizeof(MsgPayload));

         // for each entry
         for(nIdx = 0 ; nIdx < nCount ; nIdx++)
         {
            // Serialize the data
            nBytesWritten = sprintf(pWriter,
               "{%d,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu},",
               pEntries[nIdx].eId,
               pEntries[nIdx].nValues[0],
               pEntries[nIdx].nValues[1],
               pEntries[nIdx].nValues[2],
               pEntries[nIdx].nValues[3],
               pEntries[nIdx].nValues[4],
               pEntries[nIdx].nValues[5],
               pEntries[nIdx].nValues[6],
               pEntries[nIdx].nValues[7]);

            pWriter += nBytesWritten;
            nTotalWritten += nBytesWritten;
         }
         

         if (nTotalWritten > 0)
         {
            // remove trailing comma
            *(pWriter - 1) = '\0';
         }
        

         // format the message payload
         sprintf(pPayload->szPayload,
            "%s,%d,%s=%d,%s=%s",
            RESPONSE_PREFIX,
            EGetProcedureEntryList,
            TAG_COUNT,
            nCount,
            TAG_LOG_ENTRIES,
            szEntries);

         pPayload->nLength = strlen(pPayload->szPayload);
         pPayload->nChecksum = CalculateChecksum(pPayload);
      }
   }
   else
   {
      BuildCommandErrorResponse(eResponse, EGetProcedureEntryList, pPayload);
   }
}
/********************************************************************************
 *
 * Name:    BuildSetMaxN20Command
 *
 * Purpose: Builds payload for the set max N2O command.
 *
 * Inputs:  nMaxN2O - max value for N2O
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetMaxN20Command(int nMaxN2O, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(ESetN2OMax, TAG_N2O_MAX, nMaxN2O, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetMaxN2OCommand
 *
 * Purpose: Builds payload for the get max N2O command.
 *
 * Inputs:  nMaxN2O
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetMaxN2OCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetN2OMax, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetMaxN2OCommandResponse
 *
 * Purpose: Builds reset logs command payload
 *
 * Inputs:  nMaxN2O
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetMaxN2OCommandResponse(EHandlerResponse eResponse, int nMaxN2O, MsgPayload* pPayload)
{
   // populate buffer with command response
   BuildCommandResponseWithIntParameter(eResponse, EGetN2OMax, TAG_N2O_MAX, nMaxN2O, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildSetMixStepSizeCommand
 *
 * Purpose: Builds set mix step size command payload
 *
 * Inputs:  eStepSize - mix step size selection
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetMixStepSizeCommand(EMixStepSize eStepSize, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(ESetMixStepSize, TAG_MIX_STEP, eStepSize, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetMixStepSizeCommand
 *
 * Purpose: Builds get mix step size command payload
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetMixStepSizeCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetMixStepSize, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetMixStepSizeCommandResponse
 *
 * Purpose: Builds get mix step size command response payload
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetMixStepSizeCommandResponse(EHandlerResponse eResponse,
                                        EMixStepSize eStepSize, 
                                        MsgPayload* pResponse)
{
   // Build command response
   BuildCommandResponseWithIntParameter(eResponse, EGetMixStepSize, TAG_MIX_STEP, eStepSize, pResponse);
}

/********************************************************************************
 *
 * Name:    BuildSetFlowRateStepSizeCommand
 *
 * Purpose: Builds set mix step size command command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetFlowRateStepSizeCommand(EFlowRateStepSize eStepSize, MsgPayload* pPayload)
{
   BuildCommandWithIntParameter(ESetFlowRateStepSize, TAG_FLOW_STEP, eStepSize, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetFlowRateStepSizeCommand
 *
 * Purpose: Builds get mix step size command command
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetFlowRateStepSizeCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetFlowRateStepSize, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetFlowRateStepSizeCommandResponse
 *
 * Purpose: Builds get flow rate step size command response.
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetFlowRateStepSizeCommandResponse(EHandlerResponse eResponse, 
                                             EFlowRateStepSize eStepSize,
                                             MsgPayload* pPayload)
{
   // build command response
   BuildCommandResponseWithIntParameter(eResponse, EGetFlowRateStepSize, TAG_FLOW_STEP, eStepSize, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildSetClockFormatCommand
 *
 * Purpose: Builds set clock format command.
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildSetClockFormatCommand(EClockFormat eFormat, MsgPayload* pPayload)
{
   // build the command
   BuildCommandWithIntParameter(ESetClockFormat, TAG_CLOCK_FORMAT, eFormat, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetClockFormatCommand
 *
 * Purpose: Builds get clock format command.
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetClockFormatCommand(MsgPayload* pPayload)
{
   // build command payload
   BuildCommandWithNoParameter(EGetClockFormat, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetClockFormatCommand
 *
 * Purpose: Builds get clock format command response.
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetClockFormatCommandResponse(EHandlerResponse eResponse, EClockFormat eFormat, MsgPayload* pPayload)
{
   // build command response
   BuildCommandResponseWithIntParameter(eResponse, EGetClockFormat, TAG_CLOCK_FORMAT, eFormat, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetBtStatusCommand
 *
 * Purpose: Builds payload for the get Bluetooth status command.
 *
 * Inputs:  None
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetBtStatusCommand(MsgPayload* pPayload)
{
   BuildCommandWithNoParameter(EGetBtStatus, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildGetBtStatusCommandResponse
 *
 * Purpose: Builds payload for get Bluetooth command response.
 *
 * Inputs:  eResponse - command handler response code
 *          eStatus   - the Bluetooth status
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildGetBtStatusCommandResponse(EHandlerResponse eResponse, EBtStatus eStatus, MsgPayload* pPayload)
{
   BuildCommandResponseWithIntParameter(eResponse, EGetBtStatus, TAG_BT_STATUS, eStatus, pPayload);
}

/********************************************************************************
 *
 * Name:    BuildEnableDisableBtCommand
 *
 * Purpose: Builds payload for get Bluetooth command response.
 *
 * Inputs:  eResponse - command handler response code
 *          eStatus   - the Bluetooth status
 *
 * Outputs: pPayload - populated with command payload, length & checksum
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
void BuildEnableDisableBtCommand(bool bEnable, MsgPayload* pPayload)
{
   int nEnabled = bEnable ? ENABLED : DISABLED;
   BuildCommandWithIntParameter(EEnableDisableBT, TAG_BT_ENABLED, nEnabled, pPayload);
}
