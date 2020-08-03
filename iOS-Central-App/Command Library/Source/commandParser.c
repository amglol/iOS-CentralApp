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
* COMPILER:    GCC
*
* TOOLS:       Microsoft Visual Studio
*
* DESCRIPTION: Command processing logic
*
* NOTES:       None
*
********************************************************************************/
/********************************************************************************
*                         I N C L U D E    F I L E S                         
********************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandParser.h"
#include "commandBuilder.h"
#include "commandFramework.h"
#include "commandParameters.h"
/********************************************************************************
*                          D E F I N I T I O N S
********************************************************************************/
// number of parameters in a set config command payload
// used for loop processing
#define SET_CONFIG_PARAM_CNT 6

/*********************************************************************************
*                               D A T A
*********************************************************************************/


/*********************************************************************************
*                    F U N C T I O N   P R O T O T Y P E S
*********************************************************************************/
/********************************************************************************
 *
 * Name:    GetTagAndValue
 *
 * Purpose: Retrieve a single tag and value from command payload
 *
 * Inputs:  pBuffer - reference to command payload buffer
 *          
 * Outputs: szTag    - populated with parameter tag
 *          szValue  - populated with 
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
static void GetTagAndValue(char** pBuffer, char* szTag, char* szValue)
{
   // tokenize string, split at the equals
   StrTokenize(*pBuffer, "=");

   // copy parameter tag from payload
   strcpy(szTag, *pBuffer);

   RestoreDelimiterAndMovePast(&*pBuffer, '=');

   // copy value from paylaod
   strcpy(szValue, *pBuffer);  
}

/********************************************************************************
 *
 * Name:    HandleNoParameterCommand
 *
 * Purpose: Handles execution of commands without parameters
 *
 * Inputs:  fpHandle - command handler with no parameters
 *
 * Outputs: None.
 *
 * Returns: EResponseOk if successful, error code otherwise.
 *
 * Notes:   None
 *
 *******************************************************************************/
static EHandlerResponse HandleNoParameterCommand(FnHandleNoParameterCommand fpHandler)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (fpHandler)
   {
      eResponse = fpHandler();
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    HandleNoParameterCommand
 *
 * Purpose: Handles execution of commands without parameters
 *
 * Inputs:  fpHandle - command handler with no parameters
 *
 * Outputs: None.
 *
 * Returns: EResponseOk if successful, error code otherwise.
 *
 * Notes:   None
 *
 *******************************************************************************/
static EHandlerResponse HandleToggleCommand(MsgPayload* pPayload, 
                                            char* pszTag, 
                                            FnHandleSetBoolCommand fpHandler)
{
   EHandlerResponse eResponse = EInputBufferError;
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   char szTag[TMP_STR_SIZE]   = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   bool bEnabled              = 0;

   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EBuffSizeError;

      if (strlen(pPayload->szPayload) <= PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;
        
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         // grab tag and value from payload
         GetTagAndValue(&pReader, szTag, szValue);

         // if parameter tag is for scavenger enable
         if(strcmp(pszTag, szTag) == 0)
         {
            if (fpHandler)
            {
               bEnabled = (bool)strtol(szValue, &pEnd, 10);
               eResponse = fpHandler(bEnabled);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetLogList
 *
 * Purpose: Returns a list of all log entries
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer size
 *          fpHandle       - command handler function pointer
 *          
 * Outputs: pEntries       - populated with an array of log entries.
 *          pCount         - number of log entries in array.
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetLogList(FnHandleGetLogList fpHandle, 
                            int nIndex,
                            int nOffset,
                            ProcedureLog* pEntries,
                            int* pCount)
{
   EHandlerResponse eResponse = EInputBufferError;

   if (pEntries != NULL)
   {
      eResponse = EOpNotAllowed;
      if (fpHandle)
      {
         eResponse = fpHandle(nIndex, nOffset, pEntries, pCount);
      }
   }
 

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetLogEntry
 *
 * Purpose: Returns a single log entry
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer size
 *          fpHandle       - command handler function pointer
 *          
 * Outputs: pDetails       - populated with log entry details.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetLogEntry(MsgPayload* pPayload, 
                             FnHandleGetLogEntry fpHandle, 
                             LogDetails* pDetails)
{
   EHandlerResponse eResponse    = EInputBufferError;
   char* pReader                 = NULL;
   char* pEnd                    = NULL;
   char szTag[TMP_BUFF_SIZE] = { 0 };
   char szValue[TMP_BUFF_SIZE] = { 0 };
   size_t nIndex                 = 0;

   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EBuffSizeError;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // retrieve the tag and value from the payload
         GetTagAndValue(&pReader, szTag, szValue);

         // if the tag is log entry index
         if(strcmp(szTag, TAG_LOG_IDX) == 0)
         {
            // check command handler
           
            if(fpHandle)
            {
               nIndex = strtol(szValue, &pEnd, 10);

               // call command handler with index read from payload
               //eResponse = fpHandle(nIndex, pDetails);
            }
         }
      }
   }

   return eResponse;
}

/*********************************************************************************
*                    F U N C T I O N   D E F I N I T I O N S
*********************************************************************************/
/********************************************************************************
 *
 * Name:    Increases vacuum flow rate
 *
 * Purpose: Increase vaccum flow rate of the scavenger.
 *
 * Inputs:  m_cmdBuffer         - reference to command payload
 *          cmdBufferSize     - size of payload buffer
 *
 * Outputs: None
 *
 * Returns: None.
 *
 * Notes:   EHandlerResponse EResponseOk if successful, error code otherwise
 *
 *******************************************************************************/
EHandlerResponse IncreaseVacuumFlow()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleIncreaseVacuumFlow);
}

/********************************************************************************
 *
 * Name:    DecreaseVacuumFlow
 *
 * Purpose: Decreases vacuum flow rate
 *
 * Inputs:  m_cmdBuffer      - command payload
 *          cmdBufferSize  - payload buffer size
 *
 * Outputs: None.
 *
 * Returns: None.
 *
 * Notes:   EHandlerResponse EResponseOk if successful, error code otherwise
 *
 *******************************************************************************/
EHandlerResponse DecreaseVacuumFlow()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleDecreaseVacuumFlow);
}

/********************************************************************************
 *
 * Name:    GetProcedureLogCount
 *
 * Purpose: Gets the number of procedure log entries
 *
 * Inputs:  pCount
 *
 * Outputs: None.
 *
 * Returns: None.
 *
 * Notes:   EHandlerResponse EResponseOk if successful, error code otherwise
 *
 *******************************************************************************/
EHandlerResponse GetProcedureLogCount(int* pCount)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetProcedureLogCount)
   {
      eResponse = m_pHandlers.fpHandleGetProcedureLogCount(pCount);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetProcedureLogList
 *
 * Purpose: Returns a list of all procedure logs available on the TSF
 *			   controller
 *
 * Inputs:  nIndex   - procedure log ID
 *          nOffset  - starting log to load
 *          nCount   - the number of logs to load begining at nOffset
 *
 * Outputs: pEntries - array of log entries
 *
 * Returns: None.
 *
 * Notes:   EHandlerResponse EResponseOk if successful, error code otherwise
 *
 *******************************************************************************/
EHandlerResponse GetProcedureLogList(MsgPayload* pPayload, ProcedureLog* pEntries, int* pCount)
{
   EHandlerResponse eResponse = EInputBufferError;
   char* pReader = pPayload->szPayload;
   char* pEnd    = NULL;
   char szTag[TMP_BUFF_SIZE] = { 0 };
   char szValue[TMP_BUFF_SIZE] = { 0 };
   int nOffset = 0;
   int nCount  = 0;

   if(pPayload != NULL && pEntries != NULL)
   {
      eResponse = EInvalidParameters;

      // skip over the command code
      StrTokenize(pReader, ",");
      RestoreDelimiterAndMovePast(&pReader, ',');

      // split first parameter on equals
      StrTokenize(pReader, "=");

      // read tag
      strcpy(szTag, pReader);

      // restore and move past the equals
      RestoreDelimiterAndMovePast(&pReader, '=');

      // split on comma to get next parameter
      StrTokenize(pReader, ",");
      strcpy(szValue, pReader);
      RestoreDelimiterAndMovePast(&pReader, ',');

      // if tag offset
      if(strcmp(szTag, TAG_OFFSET) == 0)
      {
         // grab offset value
         nOffset = strtol(szValue, &pEnd, 10);

         // clear temp buffers
         memset(szTag, 0, TMP_BUFF_SIZE);
         memset(szValue, 0, TMP_BUFF_SIZE);

         // Tokenize string
         StrTokenize(pReader, "=");
         strcpy(szTag, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');
         strcpy(szValue, pReader);

         if(strcmp(szTag, TAG_COUNT) == 0)
         {
            nCount = strtol(szValue, &pEnd, 10);
            eResponse = EOpNotAllowed;

            if (m_pHandlers.fpHandleGetProcedureList)
            {
               eResponse = m_pHandlers.fpHandleGetProcedureList(nOffset, nCount, pEntries, pCount);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetProcedureLogEntryList
 *
 * Purpose: Returns a list of all procedure logs entries available on the TSF
 *			   controller for a given procedure log
 *
 * Inputs:  nIndex   - procedure log ID
 *          nOffset  - starting log to load
 *          nCount   - the number of logs to load begining at nOffset
 *
 * Outputs: pEntries - array of log entries
 *
 * Returns: None.
 *
 * Notes:   EHandlerResponse EResponseOk if successful, error code otherwise
 *
 *******************************************************************************/
LIB_API
EHandlerResponse GetProcedureLogEntryList(MsgPayload* pPayload, LogEntry* pEntries, int* pCount)
{
   EHandlerResponse eResponse = EInputBufferError;
   char* pReader = pPayload->szPayload;
   char* pEnd    = NULL;
   char szTag[TMP_BUFF_SIZE]     = { 0 };
   char szValue[TMP_BUFF_SIZE]   = { 0 };
   int nIndex  = 0;
   int nOffset = 0;
   int nCount  = 0;

   if (pPayload != NULL && pEntries != NULL && pCount)
   {
      eResponse = EInvalidParameters;

      // skip over the command code
      StrTokenize(pReader, ",");
      RestoreDelimiterAndMovePast(&pReader, ',');

      // split first parameter on equals
      // copy index tag
      StrTokenize(pReader, "=");
      strcpy(szTag, pReader);
      RestoreDelimiterAndMovePast(&pReader, '=');

      // copy index value
      StrTokenize(pReader, ",");
      strcpy(szValue, pReader);
      RestoreDelimiterAndMovePast(&pReader, ',');

      if(strcmp(szTag, TAG_INDEX) == 0)
      {
         // convert index value
         nIndex = strtol(szValue, &pEnd, 10);

         // clear buffers for next tag, value pair
         memset(szTag, 0, TMP_BUFF_SIZE);
         memset(szValue, 0, TMP_BUFF_SIZE);
        

         // copy the offset tag
         StrTokenize(pReader, "=");
         strcpy(szTag, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');

         // copy offset value
         StrTokenize(pReader, ",");
         strcpy(szValue, pReader);
         RestoreDelimiterAndMovePast(&pReader, ',');

         if(strcmp(szTag, TAG_OFFSET) == 0)
         {
            // convert count value
            nOffset = strtol(szValue, &pEnd, 10);

            // clear buffers for next tag, value pair
            memset(szTag, 0, TMP_BUFF_SIZE);
            memset(szValue, 0, TMP_BUFF_SIZE);

            // copy count tag
            StrTokenize(pReader, "=");
            strcpy(szTag, pReader);
            RestoreDelimiterAndMovePast(&pReader, '=');

            // copy the count value
            strcpy(szValue, pReader);

            if(strcmp(szTag, TAG_COUNT) == 0)
            {
               nCount = strtol(szValue, &pEnd, 10);

               eResponse = EOpNotAllowed;

               if (m_pHandlers.fpHandleGetProcedureEntryList)
               {
                  eResponse = m_pHandlers.fpHandleGetProcedureEntryList(nIndex, nOffset, nCount, pEntries, pCount);
               }
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetProcedureLogEntry
 *
 * Purpose: Returns details of a procedure log entry
 *
 * Inputs:  m_cmdBuffer - command payload
 *          cmdBufferSize - size of payload buffer
 *
 * Outputs: pDetails - populated with procedure log info
 *
 * Returns: None.
 *
 * Notes:   EHandlerResponse EResponseOk if successful, error code otherwise
 *
 *******************************************************************************/
EHandlerResponse GetProcedureLogEntry(MsgPayload* pPayload, LogDetails* pDetails)
{
   EHandlerResponse eResponse    = EInputBufferError;
   char* pReader                 = NULL;
   char* pEnd                    = NULL;
   char szTag[TMP_BUFF_SIZE]     = {0};
   char szValue[TMP_BUFF_SIZE]   = {0};
   size_t nIndex                 = 0;

   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EBuffSizeError;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // retrieve the tag and value from the payload
         GetTagAndValue(&pReader, szTag, szValue);

         // if the tag is log entry index
         if (strcmp(szTag, TAG_LOG_IDX) == 0)
         {
            // check command handler
           
            if(m_pHandlers.fpHandleGetProcedureEntryList)
            {
               nIndex = strtol(szValue, &pEnd, 10);

               // call command handler with index read from payload
               //eResponse = m_pHandlers.fpHandleGetProcedureEntryList(nIndex, pDetails);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetAlarmLogList
 *
 * Purpose: Returns the value for a single alarm log entry
 *
 * Inputs:  m_cmdBuffer - command payload buffer
 *          cmdBufferSize - payload buffer size
 *          
 * Outputs: pEntries - populated with log entry details
 *          pCount   - number of entries log entry array
 *
 * Returns: EResponseOk if successful, error code otherwise.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
EHandlerResponse GetAlarmLogList(int nIndex, int nOffset, LogEntry* pEntries, size_t nCount)
{
   return EOpNotAllowed;
   //return GetLogList(m_pHandlers.fpHandleGetAlarmLogList, nIndex, nOffset, nCount, pEntries);
}

/********************************************************************************
 *
 * Name:    GetSystemLogEntry
 *
 * Purpose: Returns the value for a single system log entry
 *
 * Inputs:  m_cmdBuffer - command payload buffer
 *          cmdBufferSize - payload buffer size
 *          
 * Outputs: pDetails - populated with log entry details
 *
 * Returns: EResponseOk if successful, error code otherwise.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
EHandlerResponse GetAlarmLogEntry(MsgPayload* pPayload, LogDetails* pDetails)
{
   return GetLogEntry(pPayload, m_pHandlers.fpHandleGetAlarmLogEntry, pDetails);
}
/********************************************************************************
 *
 * Name:    WriteManufacturerField
 *
 * Purpose: Writes a configuration value into flash memory into a
 *          specific location addressed by field name
 *
 * Inputs:  m_cmdBuffer      - command payload
 *          cmdBufferSize  - payload buffer size.
 *
 * Outputs: None
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   TODO: Check max field size. Will the string fit?
 *
 *******************************************************************************/
EHandlerResponse WriteManufacturerField(MsgPayload* pPayload)
{
   EHandlerResponse eResponse       = EInputBufferError;
   char* pTokenizer                 = NULL;
   char* pReader                    = NULL;
   char  szFieldName[TMP_STR_SIZE]  = { 0 };
   char  szFieldValue[TMP_STR_SIZE] = { 0 };

   // check input buffer and data size before processing
   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EBuffSizeError;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // split at the comma
         StrTokenize(pReader, ",");

         // skip the command code
         RestoreDelimiterAndMovePast(&pReader, ',');

         // split at the equals
         StrTokenize(pReader, "=");
    
         // grab field name
         strcpy(szFieldName, pReader);

         RestoreDelimiterAndMovePast(&pReader, '=');

         // grab field value
         strcpy(szFieldValue, pReader);

         // call command handler if it exists
         if(m_pHandlers.fpHandleWriteManufacturerField != NULL)
         {
            eResponse = m_pHandlers.fpHandleWriteManufacturerField(szFieldName, szFieldValue);
         }
         else
         {
            eResponse = EOpNotAllowed;
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    ReadManufacturerField
 *
 * Purpose: Reads a configuration value from  flash memory
 *
 * Inputs:  m_cmdBuffer      - command payload
 *          cmdBufferSize  - length payload buffer
 *
 * Outputs: pRead - contains field read from flash.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse ReadManufacturerField(MsgPayload* pPayload, char* pField)
{
   EHandlerResponse eResponse = EInputBufferError;

   // store starting address for length comparison
   char* pReader = NULL;
   char  szFieldName[TMP_STR_SIZE] = { 0 };
   char  szFieldValue[TMP_STR_SIZE] = { 0 };

   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize, split at the ,
         StrTokenize(pReader, ",");

         // skip the command code
         // advance pointer to the inserted NULL
         RestoreDelimiterAndMovePast(&pReader, ',');
      
         // split at the =
         StrTokenize(pReader, "=");

         // verify tag
         strcpy(szFieldName, pReader);

         RestoreDelimiterAndMovePast(&pReader, '=');

         // grab field name
         strcpy(szFieldValue, pReader);

         // if parameter is the field name
         if(strcmp(szFieldName, TAG_MF_FIELD) == 0)
         {
            // call the command handler
            if(m_pHandlers.fpHandleReadManufacturerField != NULL)
            {
               eResponse = m_pHandlers.fpHandleReadManufacturerField(szFieldValue, pField);
            }
            else
            {
               eResponse = EOpNotAllowed;
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    RestoreDefaultSettings
 *
 * Purpose: Restores the system back to default settings
 *
 * Inputs:  m_cmdBuffer - command payload buffer
 *          cmdBufferSize - length of command payload buffer
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse RestoreDefaultSettings()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleRestoreDefaultSettings);
}

/********************************************************************************
 *
 * Name:    GetScreenReadyData
 *          
 *
 * Purpose: Returns a message that gathers all parameters displayed on the
 *          main screen and functions as a kind of initializtion message
 *
 * Inputs:  m_cmdBuffer      - command payload buffer.
 *          cmdBufferSize  - payload buffer length.
 *
 * Outputs: pScreenReady - populated with data for command response.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetScreenReadyData(ScreenReady* pScreenReady)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleScreenReady)
   {
      eResponse = m_pHandlers.fpHandleScreenReady(pScreenReady);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetSyncData
 *
 * Purpose: Keeps the touchscreen data values in sync with the mobile application.
 *          Touchscreen values are updated in response to system changes
 *
 * Inputs:  cmbBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer length 
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetSyncData(SyncDataInfo* pSyncData)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleSyncData)
   {
      eResponse = m_pHandlers.fpHandleSyncData(pSyncData);        
   }
      
   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetFirmwareVersion
 *
 * Purpose: Request the firmware version from and external device
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer length
 *
 * Outputs: pVersion - populated with firmware version info.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetFirmwareVersion(FirmwareVersion* pVersion)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetFirmwareVersion)
   {
      eResponse = m_pHandlers.fpHandleGetFirmwareVersion(pVersion);
   }
      
   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetFirmwareInfo
 *
 * Purpose: Handles request for firmware info.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer length
 *
 * Outputs: pInfo - populated with firmware info data.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetFirmwareInfo(FirmwareInfo* pInfo)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if(m_pHandlers.fpHandleGetFirmwareInfo)
   {
      eResponse = m_pHandlers.fpHandleGetFirmwareInfo(pInfo);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetConfigurationData
 *
 * Purpose: Returns the configuration data when it is changed
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer length
 *
 * Outputs: pInfo - populated with firmware info data.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetConfigurationData(ConfigData* pData)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if(m_pHandlers.fpHandleGetConfigurationData)
   {
      eResponse = m_pHandlers.fpHandleGetConfigurationData(pData);
   }
  
   return eResponse;
}
/********************************************************************************
 *
 * Name:    SetConfigurationData
 *
 * Purpose: Loads configuation data from TSF controller
 *
 * Inputs:  m_cmdBuffer - command payload buffer
 *          cmdBufferSize - payload buffer length
 *
 * Outputs: pConfig - populated with configuration data
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetConfigurationData(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   ConfigData data = { 0 };
   char* pEnd                 = NULL;
   bool bStop                 = false;

   // create a buffer for each parameter value
   char szParam[TMP_STR_SIZE] = { 0 };
   char szTag[TMP_STR_SIZE]   = { 0 };
   char szValue[TMP_STR_SIZE] = {0};
   char* pReader              = NULL;
   char* pParamReader         = NULL;
   int nCnt                   = 0;

   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;

      eResponse = EBuffSizeError;
      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string, split at comma
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');
         
         for (nCnt = 0; nCnt < SET_CONFIG_PARAM_CNT; nCnt++)
         {
            StrTokenize(pReader, ",");
            strcpy(szParam, pReader);
            pParamReader = szParam;
            GetTagAndValue(&pParamReader, szTag, szValue);
            switch (nCnt)
            {
            case 0:
               if (strcmp(TAG_N2O_MAX, szTag) == 0)
               {
                  data.nMaxN20 = strtol(pParamReader, &pEnd, 10);
               }
               else
               {
                  bStop = true;
               }
               break;
            case 1:
               if (strcmp(TAG_MIX_STEP, szTag) == 0)
               {
                  data.eMixStepSize = strtol(pParamReader, &pEnd, 10);
               }
               else
               {
                  bStop = true;
               }
               break;
            case 2:
               if (strcmp(TAG_FLOW_STEP, szTag) == 0)
               {
                  data.eFlowStepSize = strtol(pParamReader, &pEnd, 10);
               }
               break;
            case 3:
               if (strcmp(TAG_CLOCK_FORMAT, szTag) == 0)
               {
                  data.eClockFormat = strtol(pParamReader, &pEnd, 10);
               }
               else
               {
                  bStop = true;
               }
               break;
            case 4:
               if (strcmp(TAG_LANGUAGE, szTag) == 0)
               {
                  data.eLanguage = strtol(pParamReader, &pEnd, 10);
               }
               else
               {
                  bStop = true;
               }
               break;
            case 5:
               if (strcmp(TAG_T_AND_D, szTag) == 0)
               {
                  strcpy(data.szTime, pParamReader);
               }
               else
               {
                  bStop = true;
               }
               break;
            default:
               // nothing to do here
               break;
            }

            if (bStop)
            {
               // invalid parameter found, stop processing
               break;
            }

            if (nCnt < SET_CONFIG_PARAM_CNT - 1)
               RestoreDelimiterAndMovePast(&pReader, ',');
         }

         if (bStop == false)
         {
            if (m_pHandlers.fpHandleSetConfigurationData)
            {
               eResponse = m_pHandlers.fpHandleSetConfigurationData(&data);
            }
         }
      }
   }

   return eResponse;
}
/********************************************************************************
 *
 * Name:    SetConfigurationData
 *
 * Purpose: Returns the time and date
 *
 * Inputs:  m_cmdBuffer - command payload buffer.
 *          cmdBufferSize - payload buffer length.
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse GetTimeAndDate(char* pTime)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetTimeAndDate)
   {
      eResponse = m_pHandlers.fpHandleGetTimeAndDate(pTime);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetTimeAndDate
 *
 * Purpose: Sets the time and date for the system
 *
 * Inputs:  m_cmdBuffer - command payload buffer.
 *          cmdBufferSize - payload buffer length.
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse SetTimeAndDate(MsgPayload* pPayload)
{
   EHandlerResponse eResponse    = EInputBufferError;
   char szTag[TMP_STR_SIZE]      = {0};
   char szTime[TMP_STR_SIZE]     = {0};
   char* pReader                 = NULL;

   // check buffer and data size before processing
   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string, split at the comma
         StrTokenize(pReader, ",");

         // skip command code and advance to next token
         RestoreDelimiterAndMovePast(&pReader, ',');

         // tokenize string, split on the equals
         StrTokenize(pReader, "=");

         strcpy(szTag, pReader);

         // if the parameter tag is time & date
         if(strcmp(TAG_T_AND_D, szTag) == 0)
         {
            
            RestoreDelimiterAndMovePast(&pReader, '=');

            // grab the values and call the command handler
            strcpy(szTime, pReader);

            if(m_pHandlers.fpHandleSetTimeAndDate)
            {
               eResponse = m_pHandlers.fpHandleSetTimeAndDate(szTime);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    StopGasFlow
 *
 * Purpose: Sets the time and date for the system
 *
 * Inputs:  m_cmdBuffer      - command payload buffer.
 *          cmdBufferSize  - payload buffer length.
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse StopGasFlow()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleStopGasFlow);
}

/********************************************************************************
 *
 * Name:    SetO2MixPercentage
 *
 * Purpose: Parses parameters from command payload and calls command handler.
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetO2MixPercentage(MsgPayload* pPayload)
{
   EHandlerResponse  eResponse = EInvalidParameters;
   ECommandCode      eType;

   char* pReader           = NULL;
   // replace all commas with null terminator
   char* pszCommand        = NULL;
   char* pszPercentage     = NULL;
   int   nPercentage       = 0;

  

   if (pPayload)
   {
      pReader = pPayload->szPayload;
      pszCommand = StrTokenize(pReader, ",");

      // read command code from buffer
      eType = atoi(pszCommand);

      // find null in buffer
      while(*pszCommand != '\0')
      {
         pszCommand++;
      }

      // restore delimiter after destructive call to strtok & advance pointer
      *pszCommand = ',';
      pszCommand++;

      if (strstr(pszCommand, TAG_O2_MIX))
      {
         pszPercentage = StrTokenize(pszCommand, "=");

         // if token is the O2 tag
         if(strcmp(pszPercentage, "O2") == 0)
         {
            while (*pszPercentage != '\0')
            {
               pszPercentage++;
            }

            // restore delimiter after destuctive call to strtok
            *pszPercentage = '=';
            pszPercentage++;

            // remaining string should be the parameter
            nPercentage = atoi(pszPercentage);

            // if handler has been set
            if(m_pHandlers.fpHandleSetO2MixPercent != NULL)
            {
               eResponse = m_pHandlers.fpHandleSetO2MixPercent(nPercentage);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetO2MixPercentage
 *
 * Purpose: Gets the O2 mix percentage.
 *
 * Inputs:  None
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetO2MixPercentage(int* pPercentage)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetO2MixPercent)
   {
      eResponse = m_pHandlers.fpHandleGetO2MixPercent(pPercentage);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetTotalFlowRate
 *
 * Purpose: Sets the gas flow rate in LPM.
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetTotalFlowRate(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   char szTag[TMP_STR_SIZE]   = {0};
   char szValue[TMP_STR_SIZE] = {0};
   int nFlowRate              = 0;

   // check buffer and size before processing
   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string, split at comma & skip command code
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         // Read tag and value from the command payload
         GetTagAndValue(&pReader, szTag, szValue);

         // if the parameter is flow rate
         if(strcmp(szTag, TAG_FLOW_RATE) == 0)
         {
            // convert string to decimal value
            nFlowRate = strtol(szValue, &pEnd, 10);

            if (m_pHandlers.fpHandleSetTotalFlowRate)
            {
               eResponse = m_pHandlers.fpHandleSetTotalFlowRate(nFlowRate);
            }
            else
            {
               eResponse = EOpNotAllowed;
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetTotalFlowRate
 *
 * Purpose: Returns value for total flow rate.
 *
 * Inputs:  None
 *
 * Outputs: pTotalFlowRate - populated with total flow rate.
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   TODO:
 *
 *******************************************************************************/
EHandlerResponse GetTotalFlowRate(int* pTotalFlowRate)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetTotalFlowRate)
   {
      eResponse = m_pHandlers.fpHandleGetTotalFlowRate(pTotalFlowRate);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetFlowRates
 *
 * Purpose: Returns value for total flow rate.
 *
 * Inputs:  None
 *
 * Outputs: pO2FlowRate - populated with O2 flow rate
 *          pN2OFlowRate - populated with N2O flow rate
 *          pScavFlowRate - populated with scavenger flow rate
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetFlowRates(int* pO2FlowRate, int* pN2OFlowRate, int* pScavFlowRate)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetFlowRates)
   {
      eResponse = m_pHandlers.fpHandleGetFlowRates(pO2FlowRate, pN2OFlowRate, pScavFlowRate);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetLanguage
 *
 * Purpose: Sets the default display language.
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetLanguage(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szTag[TMP_STR_SIZE]   = {0};
   char szValue[TMP_STR_SIZE] = {0};
   char* pReader = NULL;
   int nLanguage = 0;

   // check reference buffer
   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EBuffSizeError;

      // check data length
      if(strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         GetTagAndValue(&pReader, szTag, szValue);

         // if the tag is for language
         if(strcmp(TAG_LANGUAGE, szTag) == 0)
         {
            if (m_pHandlers.fpHandleSetLanguage)
            {
               nLanguage = atoi(szValue);
               eResponse = m_pHandlers.fpHandleSetLanguage(nLanguage);
            }
            else
            {
               eResponse = EOpNotAllowed;
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetLanguage
 *
 * Purpose: Get the display language.
 *
 * Inputs:  None 
 *
 * Outputs: pLanguage - populated with currently selected language.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetLanguage(ELanguage* pLanguage)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetLanguage)
   {
      eResponse = m_pHandlers.fpHandleGetLanguage(pLanguage);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    StartProcedure
 *
 * Purpose: Prepares the system to start the gas procedure
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse StartProcedure()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleStartProcedure);
}

/********************************************************************************
 *
 * Name:    EndProcedure
 *
 * Purpose: Ends the gas procedure
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   This command instructs the TSF controller that the procedure
 *          is coming to an end
 *******************************************************************************/
EHandlerResponse EndProcedure()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleStartProcedure);
}

/********************************************************************************
 *
 * Name:    EndProcedure
 *
 * Purpose: This command enables or disables the PIN security for dispensing N2O gas
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EHandlerResponse EResponseOk if successful, error code otherwise
 *
 * Notes:   This command instructs the TSF controller that the procedure
 *          is coming to an end
 *******************************************************************************/
EHandlerResponse EnableDisablePin(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szTag[TMP_STR_SIZE]   = {0};
   char szValue[TMP_STR_SIZE] = {0};
   bool bEnabled              = false;
   char* pReader              = NULL;
   char* pEnd                 = NULL;

   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;

      eResponse = EBuffSizeError;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize payload, split at the comma
         StrTokenize(pReader, ",");

         // skip the command code, restore delimiter
         RestoreDelimiterAndMovePast(&pReader, ',');

         // tokenize parameters, split at the equals
         StrTokenize(pReader, "=");

         // copy the tag from the payload
         strcpy(szTag, pReader);

         RestoreDelimiterAndMovePast(&pReader, '=');

         // copy the value from the payload
         strcpy(szValue, pReader);

         // if the tag matches PIN_EN
         if (strcmp(TAG_TOGGLE_PIN, szTag) == 0)
         {
            eResponse = EInvalidParameters;
            bEnabled = (bool) strtol(szValue, &pEnd, 10);

            if(m_pHandlers.fpHandleEnableDisablePin)
            {
               eResponse = m_pHandlers.fpHandleEnableDisablePin(bEnabled);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    ChangePin
 *
 * Purpose: Updates the current security PIN.
 *
 * Inputs:  m_cmdBuffer - contains the command payload
 *          cmdBufferSize - length of payload buffer
 *
 * Outputs: None.
 *
 * Returns: EResponseOk is successful, error code otherwise
 *
 * Notes:   TODO: Determine if the change PIN command should be protected
 *
 *******************************************************************************/
EHandlerResponse ChangePin(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szTag[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   bool bEnabled              = false;
   char* pReader              = NULL;
   char* pEnd                 = NULL;

   if(pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EBuffSizeError;

      // check buffer length before processing
      if(strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string, split on the comma
         StrTokenize(pReader, ",");

         // skip command code, run to next token
         RestoreDelimiterAndMovePast(&pReader, ',');

         // tokenize string, split on the equals
         StrTokenize(pReader, "=");

         // copy tag from payload
         strcpy(szTag, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');

         // copy value from payload
         strcpy(szValue, pReader);

         // check parameter tag
         if(strcmp(TAG_CHANGE_PIN, szTag) == 0)
         {
            eResponse = EOpNotAllowed;

            if (m_pHandlers.fpHandleChangePin)
            {
               eResponse = m_pHandlers.fpHandleChangePin(szValue);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    Heartbeat
 *
 * Purpose: Handles heartbeat command. Forwards request to command handler
 *
 * Inputs:  None
 *
 * Outputs: None.
 *
 * Returns: EResponseOk is successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse Heartbeat()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleHeartbeat);
}

/********************************************************************************
 *
 * Name:    
 *
 * Purpose: m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
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
EHandlerResponse SetValvePosition(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szParam1[TMP_STR_SIZE]      = {0};
   char szParam2[TMP_STR_SIZE]      = {0};
   char* pParam1 = szParam1;
   char* pParam2 = szParam2;

   char szPositionTag[TMP_STR_SIZE] = {0};
   char szGasTag[TMP_STR_SIZE]      = {0};
   char szPosition[TMP_STR_SIZE]    = {0};
   char szGasId[TMP_STR_SIZE]       = {0};
   EGasId eId;
   int nPosition                    = 0;
   char* pReader                    = NULL;
   char* pEnd                       = NULL;

   if(pPayload != NULL)
   {
      eResponse = EBuffSizeError;
      pReader = pPayload->szPayload;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string, split at the comma
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         StrTokenize(NULL, ",");

         // grab first parameter from payload
         strcpy(szParam1, pReader);
         RestoreDelimiterAndMovePast(&pReader, ',');

         strcpy(szParam2, pReader);

         StrTokenize(pParam1, "=");
         strcpy(szGasTag, pParam1);

         RestoreDelimiterAndMovePast(&pParam1, '=');

         // convert gas ID 
         eId = strtol(szParam1, &pEnd, 10);

         StrTokenize(pParam2, "=");
         strcpy(szPositionTag, szParam2);

         RestoreDelimiterAndMovePast(&pParam2, '=');

         // convert valve position
         nPosition = strtol(pParam2, &pEnd, 10);

         // if tags match expected parameters
         if (strcmp(TAG_VALVE_POS, szPositionTag) == 0
         &&  strcmp(TAG_GAS_SELECTION, szGasTag) == 0)
         {
            eResponse = EOpNotAllowed;

            if (m_pHandlers.fpHandleSetValve)
            {
               eResponse = m_pHandlers.fpHandleSetValve(nPosition, eId);
            }
         }
       
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetValvePosition
 *
 * Purpose: Reads gas valve postion from TSF controller
 *
 * Inputs:  m_cmdBuffer      - message payload
 *          cmdBufferSize  - total length of payload buffer
 *          
 * Outputs: pPositon       - populated with gas valve position
 *
 * Returns: None.
 *
 * Notes:   Reads gas valve position for O2 gas for N2O gas
 *
 *******************************************************************************/
EHandlerResponse GetValvePosition(EGasId* pId, int* pPosition)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if(m_pHandlers.fpHandleGetValve)
   {
      eResponse = m_pHandlers.fpHandleGetValve(pId, pPosition);  
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetConfiguration
 *
 * Purpose: Loads configuation data from TSF controller
 *
 * Inputs:  m_cmdBuffer - command payload buffer
 *          cmdBufferSize - payload buffer length
 *
 * Outputs: pConfig - populated with configuration data
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   No parameters needed to execute command
 *
 *******************************************************************************/
EHandlerResponse GetConfiguration(ConfigData* pConfig)
{
   EHandlerResponse eResponse = EInputBufferError;

   // check buffer reference
   if(pConfig != NULL)
   {
      eResponse = EBuffSizeError;
      // check data length before processing

      eResponse = EOpNotAllowed;

      if(m_pHandlers.fpHandleGetConfigurationData)
      {
         eResponse = m_pHandlers.fpHandleGetConfigurationData(pConfig);
      }

   }

   return eResponse;
}



/********************************************************************************
 *
 * Name:    EnableDisableVacuum
 *
 * Purpose: Turns scavenger vacuum on/off
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer length
 *
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse EnableDisableVacuum(MsgPayload* pPayload)
{

   return HandleToggleCommand(pPayload, TAG_SCAV_ENABLED, m_pHandlers.fpHandleEnableDisableVacuum);
}

/********************************************************************************
 *
 * Name:    MuteAlarm
 *
 * Purpose: Mutes audible alarm.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - payload buffer length
 *
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse MuteAlarm()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleMuteAlarm);
}

/********************************************************************************
 *
 * Name:    EnableDisableTouchscreenPower
 *
 * Purpose: Turns touchscreen power on/off
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   TODO:
 *
 *******************************************************************************/
EHandlerResponse EnableDisableTouchscreenPower(MsgPayload* pPayload)
{
   return HandleToggleCommand(pPayload, TAG_TS_POWER, m_pHandlers.fpHandleEnableDisablePower);
}

/********************************************************************************
 *
 * Name:    EnableDisableTouchscreenPower
 *
 * Purpose: Turns touchscreen power on/off
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   TODO:
 *
 *******************************************************************************/
EHandlerResponse FirmwareDownload()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleFirmwareDownload);
}

/********************************************************************************
 *
 * Name:    EnableGasFlow
 *
 * Purpose: Enabled/disable gas flow for selected gas type.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   Parameters expected
 *          TAG_GAS_SELECTION (0 or 1)
 *          TAG_ENABLE_GAS_FLOW (0 or 1)
 *
 *******************************************************************************/
EHandlerResponse EnableGasFlow(MsgPayload* pPayload)
{
   EHandlerResponse eResponse    = EInputBufferError;
   char szParam1[TMP_STR_SIZE]   = {0};
   char szParam2[TMP_STR_SIZE]   = {0};
   char szTag1[TMP_STR_SIZE]     = {0};
   char szTag2[TMP_STR_SIZE]     = {0};
   char szValue1[TMP_STR_SIZE]   = {0};
   char szValue2[TMP_STR_SIZE]   = {0};
   EGasId eId;
   bool bEnabled                 = false;
   char* pReader                 = NULL;
   char*  pEnd                   = NULL;

   if(pPayload != NULL)
   {
      eResponse   = EBuffSizeError;
      pReader     = pPayload->szPayload;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string. Split at the comma
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         StrTokenize(NULL, ",");

         // copy the first parameter from the payload
         strcpy(szParam1, pReader);
         RestoreDelimiterAndMovePast(&pReader, ',');

         // copy the second parameters from the payload
         strcpy(szParam2, pReader);

         pReader = szParam1;

         // split param1 at the equals
         StrTokenize(pReader, "=");

         // copy the first parameter tag
         strcpy(szTag1, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');

         // copy the first parameter value from payload
         strcpy(szValue1, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');

         pReader = szParam2;
         StrTokenize(pReader, "=");
         strcpy(szTag2, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');

         strcpy(szValue2, pReader);
         RestoreDelimiterAndMovePast(&pReader, '=');

         // if parameter tags are correct
         if (strcmp(szTag1, TAG_GAS_SELECTION) == 0
         &&  strcmp(szTag2, TAG_ENABLE_GAS_FLOW) == 0)
         {
            eResponse = EOpNotAllowed;
            if(m_pHandlers.fpHandleEnableGasFlow)
            {
               eId = strtol(szValue1, &pEnd, 10);
               bEnabled = (bool)strtol(szValue2, &pEnd, 10);
               eResponse = m_pHandlers.fpHandleEnableGasFlow(eId, bEnabled);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    BTFirmwareDownload
 *
 * Purpose: Sets BT chip to receive firmware download
 *
 * Inputs:  None
 *
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse BTFirmwareDownload()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleBtFirmwareDownload);
}

/********************************************************************************
 *
 * Name:    GetScavengerInfo
 *
 * Purpose: Returns values for the get scavenger info command.
 *
 * Inputs:  None.
 *
 * Outputs: pInfo - Populated with scavenger info.
 *
 * Returns: EResponseOk if successful, error code otherwise.
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetScavengerInfo(ScavengerInfo* pInfo)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetScavengerInfo)
   {
      eResponse = m_pHandlers.fpHandleGetScavengerInfo(pInfo);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetGasVolumeInfo
 *
 * Purpose: Retrieves info about the total volume of gas dispensed.
 *
 * Inputs:  None.
 *
 * Outputs: pInfo - Populated with scavenger info.
 *
 * Returns: EResponseOk if successful, error code otherwise.
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetGasVolumeInfo(GasVolumeInfo* pInfo)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetGasVolumeInfo)
   {
      eResponse = m_pHandlers.fpHandleGetGasVolumeInfo(pInfo);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    FlushO2
 *
 * Purpose: Handles O2 flush command. Sets O2 mix to 100%
 *
 * Inputs:  None
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse FlushO2(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   bool  bEnabled             = false;

   if(pPayload)
   {
      eResponse = EBuffSizeError;
      pReader = pPayload->szPayload;

      // check buffer length before processing
      if(strlen(pPayload->szPayload) <= PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;
         // tokenize string, split at the comma
         // skip the comand code
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         // read the field name and value from payload
         GetTagAndValue(&pReader, szField, szValue);

         // if the tag is for O2 flush
         if(strcmp(szField, TAG_FLUSH_O2) == 0)
         {
            bEnabled = (bool)strtol(szValue, &pEnd, 10);
            if(m_pHandlers.fpHandleO2Flush)
            {
               eResponse = m_pHandlers.fpHandleO2Flush(bEnabled);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    BtFirmwareDownload
 *
 * Purpose: Prepares BT chip for firmware download
 *
 * Inputs:  None
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse BtFirmwareDownload()
{
   return HandleNoParameterCommand(m_pHandlers.fpHandleBtFirmwareDownload);
}

/********************************************************************************
 *
 * Name:    WriteManufacturersField
 *
 * Purpose: Handles the write manufacturer field command.
 *
 * Inputs:  None
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse WriteManufacturersField(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = {0};
   char szValue[TMP_STR_SIZE] = {0};
   char* pReader              = NULL;

   // check buffer before processing
   if(pPayload != NULL)
   {
      eResponse = EBuffSizeError;
      pReader   = pPayload->szPayload;

      // check buffer length before processing
      if(strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         // tokenize string, split at the comma
         // skip the comand code
         StrTokenize(pReader, ",");
         RestoreDelimiterAndMovePast(&pReader, ',');

         // read the field name and value from payload
         GetTagAndValue(&pReader, szField, szValue);

         // check command handler before executing
         if(m_pHandlers.fpHandleWriteManufacturerField)
         {
            eResponse = m_pHandlers.fpHandleWriteManufacturerField(szField, szValue);
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    ReadManufacturersField
 *
 * Purpose: Handles reading manufacturers field command from flash.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: pszValue - populated with field value, read from flash
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None.
 *
 *******************************************************************************/
EHandlerResponse ReadManufacturersField(MsgPayload* pPayload, char* pszValue)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   
   if(pPayload != NULL)
   {
      eResponse = EBuffSizeError;
      pReader = pPayload->szPayload;

      if (strlen(pPayload->szPayload) < PAYLOAD_LENGTH)
      {
         eResponse = EInvalidParameters;

         // tokenize string, split at the comma
         StrTokenize(pReader, ",");

         // skip command code
         RestoreDelimiterAndMovePast(&pReader, ',');

         // get the parameter tag and value from command buffer
         GetTagAndValue(&pReader, szField, szValue);

         // if the tag is manufacturer field name
         if (strcmp(TAG_MF_FIELD, szField) == 0)
         {
            eResponse = EOpNotAllowed;

            if(m_pHandlers.fpHandleReadManufacturerField)
            {
               eResponse = m_pHandlers.fpHandleReadManufacturerField(szValue, pszValue);
            }
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetMaxN2O
 *
 * Purpose: Sets the max value for N2O gas percentage.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: pszValue - populated with field value, read from flash
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   Must be 70 or below
 *
 *******************************************************************************/
EHandlerResponse SetMaxN2O(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   uint8_t nMaxValue              = 0;
   
   if (pPayload != NULL)
   {

      pReader = pPayload->szPayload;
      eResponse = EInvalidParameters;

      // tokenize string, split at the comma
      StrTokenize(pReader, ",");

      // skip command code
      RestoreDelimiterAndMovePast(&pReader, ',');

      // get the parameter tag and value from command buffer
      GetTagAndValue(&pReader, szField, szValue);

      // if the tag is manufacturer field name
      if(strcmp(TAG_N2O_MAX, szField) == 0)
      {
         eResponse = EOpNotAllowed;
         nMaxValue = (uint8_t)strtol(szValue, &pEnd, 10);
         if (m_pHandlers.fpHandleSetMaxN2OMixPercent)
         {
            eResponse = m_pHandlers.fpHandleSetMaxN2OMixPercent(nMaxValue);
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetMaxN2O
 *
 * Purpose: Sets the max value for N2O gas percentage.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: pszValue - populated with field value, read from flash
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   Must be 70 or below
 *
 *******************************************************************************/
EHandlerResponse GetMaxN2O(int* pMaxValue)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetMaxN2OMixPercent)
   {
      eResponse = m_pHandlers.fpHandleGetMaxN2OMixPercent(pMaxValue);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetMixStepSize
 *
 * Purpose: Sets the max value for N2O gas percentage.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: pszValue - populated with field value, read from flash
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   Must be 70 or below
 *
 *******************************************************************************/
EHandlerResponse GetMixStepSize(EMixStepSize* pStepSize)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetMixStepSize)
   {
      eResponse = m_pHandlers.fpHandleGetMixStepSize(pStepSize);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetMixStepSize
 *
 * Purpose: Sets the mix step size value.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetMixStepSize(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   EMixStepSize eStepSize;
   
   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EInvalidParameters;

      // tokenize string, split at the comma
      StrTokenize(pReader, ",");

      // skip command code
      RestoreDelimiterAndMovePast(&pReader, ',');

      // get the parameter tag and value from command buffer
      GetTagAndValue(&pReader, szField, szValue);

      // if the tag is manufacturer field name
      if(strcmp(TAG_MIX_STEP, szField) == 0)
      {
         eResponse = EOpNotAllowed;
         eStepSize = (EMixStepSize)strtol(szValue, &pEnd, 10);
         if (m_pHandlers.fpHandleSetMixStepSize)
         {
            eResponse = m_pHandlers.fpHandleSetMixStepSize(eStepSize);
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetFlowRateStepSize
 *
 * Purpose: Sets the flow rate step size value.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetFlowRateStepSize(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   EFlowRateStepSize          eStepSize;
   
   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EInvalidParameters;

      // tokenize string, split at the comma
      StrTokenize(pReader, ",");

      // skip command code
      RestoreDelimiterAndMovePast(&pReader, ',');

      // get the parameter tag and value from command buffer
      GetTagAndValue(&pReader, szField, szValue);

      // if the tag is manufacturer field name
      if(strcmp(TAG_FLOW_STEP, szField) == 0)
      {
         eResponse = EOpNotAllowed;
         eStepSize = (EFlowRateStepSize)strtol(szValue, &pEnd, 10);
         if (m_pHandlers.fpHandleSetFlowRateStepSize)
         {
            eResponse = m_pHandlers.fpHandleSetFlowRateStepSize(eStepSize);
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetFlowRateStepSize
 *
 * Purpose: Gets the flow rate step size value.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetFlowRateStepSize(EFlowRateStepSize* pStepSize)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetFlowRateStepSize)
   {
      eResponse = m_pHandlers.fpHandleGetFlowRateStepSize(pStepSize);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetFlowRateStepSize
 *
 * Purpose: Gets the flow rate step size value.
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetClockFormat(EClockFormat* pFormat)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetClockFormat)
   {
      eResponse = m_pHandlers.fpHandleGetClockFormat(pFormat);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    SetClockFormat
 *
 * Purpose: Sets the clock format
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse SetClockFormat(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   EClockFormat eFormat;
   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EInvalidParameters;

      // tokenize string, split at the comma
      StrTokenize(pReader, ",");

      // skip command code
      RestoreDelimiterAndMovePast(&pReader, ',');

      // get the parameter tag and value from command buffer
      GetTagAndValue(&pReader, szField, szValue);

      // if the tag is manufacturer field name
      if(strcmp(TAG_CLOCK_FORMAT, szField) == 0)
      {
         eResponse = EOpNotAllowed;
         eFormat = (EClockFormat)strtol(szValue, &pEnd, 10);
         if (m_pHandlers.fpHandleSetClockFormat)
         {
            eResponse = m_pHandlers.fpHandleSetClockFormat(eFormat);
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    GetBtStatus
 *
 * Purpose: Sets the clock format
 *
 * Inputs:  m_cmdBuffer      - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse GetBtStatus(EBtStatus* pStatus)
{
   EHandlerResponse eResponse = EOpNotAllowed;

   if (m_pHandlers.fpHandleGetBtStatus)
   {
      eResponse = m_pHandlers.fpHandleGetBtStatus(pStatus);
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    EnableDisableBt
 *
 * Purpose: Enables or disables the BT connectivity on the device.
 *
 * Inputs:  m_cmdBuffer    - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse EnableDisableBt(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   bool bEnable               = false;

   if (pPayload != NULL)
   {
      pReader = pPayload->szPayload;
      eResponse = EInvalidParameters;

      // tokenize string, split at the comma
      StrTokenize(pReader, ",");

      // skip command code
      RestoreDelimiterAndMovePast(&pReader, ',');

      // get the parameter tag and value from command buffer
      GetTagAndValue(&pReader, szField, szValue);

      // if the tag is manufacturer field name
      if(strcmp(TAG_BT_ENABLED, szField) == 0)
      {
         eResponse = EOpNotAllowed;
         bEnable = (bool)strtol(szValue, &pEnd, 10);
         if (m_pHandlers.fpHandleEnableDisableBt)
         {
            eResponse = m_pHandlers.fpHandleEnableDisableBt(bEnable);
         }
      }
   }

   return eResponse;
}

/********************************************************************************
 *
 * Name:    ResetGasVolume
 *
 * Purpose: Resets values for gas volume dispensed
 *
 * Inputs:  m_cmdBuffer    - command payload buffer
 *          cmdBufferSize  - command buffer size
 *          
 * Outputs: None
 *
 * Returns: EResponseOk if successful, error code otherwise
 *
 * Notes:   None
 *
 *******************************************************************************/
EHandlerResponse ResetGasVolume(MsgPayload* pPayload)
{
   EHandlerResponse eResponse = EInputBufferError;
   char szField[TMP_STR_SIZE] = { 0 };
   char szValue[TMP_STR_SIZE] = { 0 };
   char* pReader              = NULL;
   char* pEnd                 = NULL;
   EGasId eId                 = EGasIdMax;

   if (pPayload != NULL)
   {
      eResponse = EBuffSizeError;
      pReader = pPayload->szPayload;

      eResponse = EInvalidParameters;

      // tokenize string, split at the comma
      StrTokenize(pReader, ",");

      // skip command code
      RestoreDelimiterAndMovePast(&pReader, ',');

      // get the parameter tag and value from command buffer
      GetTagAndValue(&pReader, szField, szValue);

      // if the tag is manufacturer field name
      if(strcmp(TAG_GAS_SELECTION, szField) == 0)
      {
         eResponse = EOpNotAllowed;
         eId = (EGasId)strtol(szValue, &pEnd, 10);
         if (m_pHandlers.fpHandleResetGasVolumeInfo)
         {
            eResponse = m_pHandlers.fpHandleResetGasVolumeInfo(eId);
         }
      }
   }

   return eResponse;
}
