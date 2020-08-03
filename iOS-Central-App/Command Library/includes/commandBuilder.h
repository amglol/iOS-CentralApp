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
* DESCRIPTION: Module for building message payloads.
*
* NOTES:       None
*
* REVIEWS:     1
*
********************************************************************************/
#ifndef COMMAND_BUILDER_H
#define COMMAND_BUILDER_H

// C++ guard
#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
*                         I N C L U D E    F I L E S
********************************************************************************/
#include "commandFramework.h"
#include "commandHandlers.h"
#include "commandParameters.h"

#define ENABLED 1
#define DISABLED 0
#define ON ENABLED
#define OFF DISABLED

#define RESPONSE_PREFIX       "RSP"
#define ERROR_PREFIX          "ERR"

// Parameter tags
#define TAG_O2_MIX            "O2"
#define TAG_TOGGLE_PIN        "PIN_EN"
#define TAG_PIN_RESP          "PIN"
#define TAG_CHANGE_PIN        "NEW_PIN"
#define TAG_FLOW_RATE         "FLOW_RATE"
#define TAG_O2_FLOW           "O2_FLOW"
#define TAG_N2O_FLOW          "N2O_FLOW"
#define TAG_SCAV_FLOW         "SCAV_FLOW"
#define TAG_N2O_MAX           "N2O_MAX"
#define TAG_MIX_STEP          "MIX_STEP"
#define TAG_FLOW_STEP         "FLOW_STEP"
#define TAG_CLOCK_FORMAT      "CLOCK_F"
#define TAG_LANGUAGE          "LANG"
#define TAG_T_AND_D           "T_AND_D"
#define TAG_TS_POWER          "TS_POWER"
#define TAG_SET_LANGUAGE      "LANG"
#define TAG_IN_PROGRESS       "IN_PROGRESS"
#define TAG_ENDING            "ENDING"
#define TAG_STOP_GAS          "STOP_GAS"
#define TAG_FW_CONFIG         "SW_CFG_VR"
#define TAG_FW_MAIN           "MAIN_FW_VR"
#define TAG_FW_BT             "BT_FW_VR"
#define TAG_BT_STATUS         "BT_STATUS"
#define TAG_BT_ENABLED        "BT_EN"
#define TAG_FW_GUI            "GUI_FW_VR"
#define TAG_FW_SCAV           "SCAV_FW_VR"
#define TAG_FW_VERSION        "FW_VER"
#define TAG_VALVE_POS         "VALVE_POS"
#define TAG_SEND_MSG          "MSG"
#define TAG_ENABLE_GAS_FLOW   "GF_EN"
#define TAG_MF_FIELD          "FIELD"
#define TAG_LOG_IDX           "LOG_INDEX"
#define TAG_SCAV_ENABLED      "SCAV_EN"
#define TAG_SCAV_VALVE        "SCAV_VALVE"
#define TAG_SCAV_SENSOR       "SCAV_SENSOR"
#define TAG_GAS_SELECTION     "GAS_TYPE"
#define TAG_GV_O2             "O2_DISP"
#define TAG_GV_N2O            "N2O_DISP"
#define TAG_GV_RESET_O2       "O2_RESET"
#define TAG_GV_RESET_N2O      "N2O_RESET"
#define TAG_FLUSH_O2          "O2_FLUSH"
#define TAG_P_LOG_COUNT       "LOG_CNT"
#define TAG_LOG_ENTRIES       "LOG_ENTRIES"
#define TAG_INDEX             "IDX"
#define TAG_OFFSET            "OFFSET"
#define TAG_COUNT             "COUNT"

   /*********************************************************************************
   *                           F U N C T I O N S
   *********************************************************************************/
   LIB_API
void BuildCommandNoParameter(ECommandCode eId, MsgPayload* pPayload);
   // Builds command response
   // Inputs:  eResponse - result from command handler-
   //          pReceived - payload from received command
   // Outputs: pResponse - payload for command response
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildCommandEchoResponse(EHandlerResponse eResponse, ECommandCode eCode,  MsgPayload* pReceived, MsgPayload* pResponse);

   // Builds command error response
   // Inputs:  eResponse - result from command handler
   //          pReceived - payload from received command
   // Outputs: pResponse - payload for command response
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildCommandErrorResponse(EHandlerResponse eResponse, ECommandCode eCode, MsgPayload* pResponse);

   // Builds vacuum increase command payload
   // Inputs:  None
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildVacuumIncreaseCommand(MsgPayload* pPayload);

   // Builds vacuum decrease command payload
   // Inputs:  pPayload - populated the message buffer, length & checksum
   // Outputs: None.
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildVacuumDecreaseCommand(MsgPayload* pPayload);

   // Build start procedure command
   // Inputs:  None
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildStartProcedureCommand(MsgPayload* pPayload);

   // Builds end procedure command
   // Inputs:  None
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildEndProcedureCommand(MsgPayload* pPayload);

   // Builds toggle PIN procedure command payload
   // Inputs: bEnabled - enable/disable PIN flag
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildEnableDisablePinCommand(bool bEnabled, MsgPayload* pPayload);

   // Build change PIN command payload
   // Inputs:  pszNewPin - new PIN string
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Returns: None
   // Notes:   Determine if the change PIN command requires security
   LIB_API
   void BuildChangePinCommand(char* pszNewPin, MsgPayload* pPayload);

   // Builds payload for the set language command
   // Inputs:  eLanguage - unique ID for display language
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildSetLanguageCommand(ELanguage eLanguage, MsgPayload* pPayload);

   // Builds payload for the get language command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetLanguageCommand(MsgPayload* pPayload);

   // Builds payload for the get language command response
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetLanguageCommandResponse(EHandlerResponse eResponse, ELanguage eLang, MsgPayload* pPayload);

   // Builds payload for set total flow rate command
   // Inputs:  dFlowRate - flow rate in LPM
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildSetTotalFlowRateCommand(int nFlowRate, MsgPayload* pPayload);

   // Builds payload for the get total flow rate command
   // Inputs:  None.
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetTotalFlowRateCommand(MsgPayload* pPayload);

   // Builds payload for the get flow rates command (O2 & N2O flow rates)
   // Inputs:  None.
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetFlowRatesCommand(MsgPayload* pPayload);

   // Builds payload for the get flow rates command response (O2 & N2O flow rates)
   // Inputs:  eResponse - handler response code
   //          nO2FlowRate - the O2 flow rate
   //          nN2OFlowRate - the N2O flow rate
   //          nScavengerFlowRate - nScavengerFlowRate
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetFlowRatesCommandResponse(EHandlerResponse eResponse, int nO2FlowRate, int nN2OFlowRate, int nScavengerFlowRate, MsgPayload* pPayload);

   // Builds payload for the get total flow rate command response
   // Inputs:  eResponse - command handlers response code
   //          flowRate  - the total flow rate (all gases)
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:   None
   LIB_API
   void BuildGetTotalFlowRateCommandResponse(EHandlerResponse eResponse, int flowRate, MsgPayload* pPayload);

   // Builds payload for set O2 Mix command
   // Inputs:  nPercentage - mix percentage. Between 30 - 100
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes: Minimum O2 setting is 30 %
   LIB_API
   void BuildSetO2MixCommand(int nPercentage, MsgPayload* pPayload);

   // Builds payload for the get O2 mix command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes: None
   LIB_API
   void BuildGetO2MixCommand(MsgPayload* pPayload);

   // Builds payload for the get O2 mix command response
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes: None
   LIB_API
   void BuildGetO2MixCommandResponse(EHandlerResponse eResponse, int nPercentage, MsgPayload* pPayload);

   // Build payload for the stop gas command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildStopGasCommand(MsgPayload* pPayload);

   // Builds payload for the toggle Bluetooth command
   // Inputs:  bEnabled - Enables/disables Bluetooth communications
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildEnableDisableBTCommand(bool bEnabled, MsgPayload* pPayload);

   // Builds payload for restore default settings command.
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildRestoreDefaultsCommand(MsgPayload* pPayload);

   // Builds payload for the PIN challenge command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload
   // Returns: None
   // Notes:   None
   
   LIB_API
   void BuildHeartbeatCommand(MsgPayload* pPayload);

   // Buidls payload for the sync data command
   // Inputs:  pSyncData - contains sync data for external display
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildSyncDataCommand(SyncDataInfo* pSyncData, MsgPayload* pPayload);

   // Build payload for get firmware version command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetFirmwareVersionCommand(MsgPayload* pPayload);

   // Builds payload for the get firmware version command response.
   // Inputs:  pVersion - firmware version info
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetFirmwareVersionCommandResponse(EHandlerResponse eResponse, FirmwareVersion* pVersion, MsgPayload* pPayload);

   // Builds payload for the get firmware info command
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetFirmwareInfoCommand(MsgPayload* pPayload);

   //
   // Inputs:
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildGetFirmwareInfoCommandResponse(EHandlerResponse eResponse, FirmwareInfo* pInfo, MsgPayload* pPayload);
	
   // 
   // Inputs:
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildGetConfigurationDataCommand(MsgPayload* pPayload);

   // 
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetConfigurationDataCommandResponse(EHandlerResponse eResponse, ConfigData* pConfig, MsgPayload* pPayload);

   //
   // Inputs:
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildSetConfigurationDataCommand(ConfigData* pConfig, MsgPayload* pPayload);
	
	//
	// Inputs:
	// Outputs: pPayload - populated with command payload, length & checksum
	// Returns:
	// Notes:
   LIB_API
   void BuildGetTimeAndDateCommand(MsgPayload* pPayload);

   // Builds response for get time & date command
   // Inputs:  eResponse - response from command handler
   //          pDateTime - time & date returned from command handler
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetTimeAndDateCommandResponse(EHandlerResponse eResponse, char* pDateTime, MsgPayload* pPayload);

   // Builds payload for the set time & date command
   // Inputs:  pDataTime - date time formatted string (MM/DD/YYYY hh:mm:ss *tt) optional tt 
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildSetTimeAndDateCommand(const char* pDateTime, MsgPayload* pPayload);

   // Builds payload for the set valve command
   // Inputs:  nPosition - valve position between 0 - 100
   //          eId       - unique id for gas valve (O2 or N2O)
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildSetValveCommand(int nPosition, EGasId eId, MsgPayload* pPayload);

   // Builds payload for the get valve command
   // Inputs:  eId - unique id for gas valve (O2 or N2O)
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildGetValveCommand(EGasId eId, MsgPayload* pPayload);

   // Builds payload for the get valve command response
   // Inputs:  eResponse - command handler response
   //          eId        - unique id for gas valve (O2 or N2O)\
   //          nPosition  - the position as a percentage 0 - 100
   // Outputs: pPayload  - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildGetValveCommandResponse(EHandlerResponse eResponse, EGasId eId, int nPosition, MsgPayload* pPayload);

   // Builds payload for the firmware download command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum 
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildFirmwareDownloadCommand(MsgPayload* pPayload);

   // Builds payload for the enable gas flow command
   // Inputs:  eId - unique ID for gas selection (O2 or N2O)
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildEnableGasFlowCommand(EGasId eId, bool bEnabled, MsgPayload* pPayload);

   // Builds payload for the Bluetooth firmware download
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildBTFirmwareDownloadCommand(MsgPayload* pPayload);

   // Builds payload for the write manufacturer field command
   // Inputs:  pField - the name of the field to write to flash memory
   //          pValue - the field value to write to flash memory
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildWriteManufacturerFieldCommand(const char* pField, const char* pValue, MsgPayload* pPayload);

   // 
   // Inputs:
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildReadManufacturerFieldCommand(const char* pField, MsgPayload* pPayload);

   // Prepares Read manufacturer field command response.
   // Inputs:  eResponse - response from command handler
   //          pField    - name of the field read from flash
   //          pValue    - value of the field to return in response
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildReadManufacturerFieldCommandResponse(EHandlerResponse eResponse, 
                                                  const char* pValue,
                                                  MsgPayload* pPayload, 
                                                  MsgPayload* pResponse);

   // Builds the ACK command
   // Inputs:
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:
   LIB_API
   void BuildAckCommand(MsgPayload* pPayload);

   // Builds the NAK command
   // Inputs:
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns:
   // Notes:

   LIB_API
   void BuildNakCommand(int nErrorCode, MsgPayload* pPayload);

   // Builds the toggle vacuum command
   // Inputs:  bEnabled - enable/disable flag
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   Turns scavenger vacuum on/off
   
   LIB_API
   void BuildEnableDisableVacuumCommand(bool bEnabled, MsgPayload* pPayload);

   // Builds the screen ready command
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   Response contains all values needed for external display initialization

   LIB_API
   void BuildScreenReadyCommand(MsgPayload* pPayload);

   // Builds the screen ready command response
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   Response for screen ready command.
   //          Contains all values needed for external display initialization
   
   LIB_API
   void BuildScreenReadyCommandResponse(EHandlerResponse eResponse, 
                                        ScreenReady* pScreenReady, 
                                        MsgPayload* pPayload);


   // Builds payload for the mute alarm command
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildMuteAlarmCommand(MsgPayload* pPayload);

   // Turns the touchscreen on or off
   // Inputs:  bEnabled - turns touchscreen on/off
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None.
   LIB_API
   void BuildEnableDisableScreenPowerCommand(bool bEnabled, MsgPayload* pPayload);

   // Builds get scavenger info command payload.
   // Inputs:  pInfo    - contains the scavenger into
   // Outputs: pPayload - populated with command payload, length & checksum 
   // Returns: None
   // Notes:   None
   //
   LIB_API
   void BuildGetScavengerInfoCommand(MsgPayload* pPayload);

   // Builds response payload for teh get scavenger info command.
   // Inputs:  eResponse - the response from the command handler
   //          pInfo     - info to populate response
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetScavengerInfoCommandResponse(EHandlerResponse eResponse, ScavengerInfo* pInfo, MsgPayload* pPayload);

   // Builds get gas volume info command payload.
   // Inputs:  None
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildGetGasVolumeInfoCommand(MsgPayload* pPayload);

   // Builds get gas volume info command response payload.
   // Inputs:  eResponse - response from command handler
   //          pInfo     - gas volume info for response payload
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetGasVolumeInfoCommandResponse(EHandlerResponse eResponse, GasVolumeInfo* pInfo, MsgPayload* pPayload);

   // Build reset gas volume info command payload
   // Inputs:  eId - gas selection id
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildResetGasVolumeInfoCommand(EGasId eId, MsgPayload* pPayload);

   // Builds payload for the flush O2 command
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildFlushO2Command(bool bEnabled, MsgPayload* pPayload);

   // Build payload to retrieve the number of procedure logs
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   
   
   LIB_API
   void SetLogDateTime(uint8_t year, 
                       uint8_t month, 
                       uint8_t day, 
                       uint8_t hour, 
                       uint8_t min, 
                       uint8_t second, 
                       char* pBuffer);
   LIB_API
   void BuildGetProcedureLogCount(MsgPayload* pPayload);

   // Builds payload response for the get procedure log count
   // Inputs:  None.
   // Outputs: pPayload - populated with command payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetProcedureLogCountResponse(EHandlerResponse eResponse, int nCount, MsgPayload* pPayload);

   // Build payload for get procedure list command. Requests nCount number of procedure logs
   // Inputs:  nOffset - first log to load. 
   //          nCount  - number of requested logs. 
   // Outputs: pPayload - populated with message payload
   // Returns: None
   // Notes:   nOffset + nCount must be less than response from GetProcedureLogCount value.
   LIB_API
   void BuildGetProcedureLogList(int nOffset, int nCount, MsgPayload* pPayload);

   // Builds payload for the ger procedure log list response. Returns the requested number
   // of procedure logs and a count for the number of logs actually returned.
   // Inputs:  eResponse   - command handler response code
   //          nCount      - the number of records in the response
   //          pEntries    - pointer to array of nCount number of records.
   LIB_API
   void BuildGetProcedureLogListResponse(EHandlerResponse eResponse, int nCount, ProcedureLog* pEntries, MsgPayload* pPayload);

   LIB_API
   void BuildGetProcedureLogCommand(size_t nIndex, MsgPayload* pPayload);

   // Builds get procedure entry list command payload
   // Inputs: None
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Notes: None
   LIB_API
   void BuildGetProcedureLogEntryListCommand(int nIndex, int nOffset, int nCount, MsgPayload* pPayload);

   // Builds get procedure entry command payload
   // Inputs:  None
   // Outputs: pPayload - populated the message buffer, length & checksum
   // Returns: None
   // Notes:

   LIB_API
   void BuildGetProcedureLogEntryListCommandResponse(EHandlerResponse eResponse, int nCount, LogEntry* pEntries, MsgPayload* pPayload);
   
 
   // Builds payload for get alarm log list command
   // Inputs:  None
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildGetAlarmLogListCommand(MsgPayload* pPayload);

   // Builds payload for get alarm log entry command
   // Inputs:  nIndex - index of log entry to load
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes:   None.
   LIB_API
   void BuildGetAlarmLogEntryCommand(size_t nIndex, MsgPayload* pPayload);

   // Builds payload for rest logs command
   // Inputs:  eLogId - id for log selection
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildResetLogsCommand(ELogId eLogId, MsgPayload* pPayload);

   // Builds payload for the set max N2O command
   // Inputs:  nMaxN2O - the max value for N2O gas dispensing
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildSetMaxN20Command(int nMaxN2O, MsgPayload* pPayload);

   // Builds payload for the get max N2O command
   // Inputs:  nMaxN2O - the max value for N2O gas dispensing
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetMaxN2OCommand(MsgPayload* pPayload);

   // Builds payload for the get max N2O command
   // Inputs:  nMaxN2O - the max value for N2O gas dispensing
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetMaxN2OCommandResponse(EHandlerResponse eResponse, int nMaxN2O, MsgPayload* pPayload);

   // Builds payload for the set mix step size command
   // Inputs:  eStepSize - mix step size selection
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildSetMixStepSizeCommand(EMixStepSize eStepSize, MsgPayload* pPayload);

   // Builds payload for the get mix step size command
   // Inputs:  eStepSize - mix step size selection
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetMixStepSizeCommand(MsgPayload* pPayload);

   // Builds payload for the get mix step size command
   // Inputs:  eResponse - response from command handler
   //          eStepSize - mix step size selection
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None.
   // Notes:   None.
   LIB_API
   void BuildGetMixStepSizeCommandResponse(EHandlerResponse eResponse,
                                           EMixStepSize eStepSize, 
                                           MsgPayload* pResponse);

   // Builds payload for the set flow rate step size command
   // Inputs:  eStepSize - selected step size
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildSetFlowRateStepSizeCommand(EFlowRateStepSize eStepSize, MsgPayload* pPayload);

   // Builds payload for the set flow rate step size command
   // Inputs:  None
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildGetFlowRateStepSizeCommand(MsgPayload* pPayload);

   // Builds payload for the set flow rate step size command
   // Inputs:  eStepSize - selected step size
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildGetFlowRateStepSizeCommandResponse(EHandlerResponse eResponse, EFlowRateStepSize eStepSize, MsgPayload* pPayload);

   // Builds payload for the set clock format command
   // Inputs:  eStepSize - selected step size
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildSetClockFormatCommand(EClockFormat eFormat, MsgPayload* pPayload);

   // Builds payload for the get clock format command
   // Inputs:  eStepSize - selected step size
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildGetClockFormatCommand(MsgPayload* pPayload);

   // Builds payload for the get clock format command response
   // Inputs:  eStepSize - selected step size
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildGetClockFormatCommandResponse(EHandlerResponse eResponse, EClockFormat eFormat, MsgPayload* pPayload);

   // Build the payload for the get Bluetooth status command
   // Inputs:  None
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildGetBtStatusCommand(MsgPayload* pPayload);

   // Builds the payload for get Bluetooth status command response
   // Inputs:  None
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes: None
   LIB_API
   void BuildGetBtStatusCommandResponse(EHandlerResponse eResponse, EBtStatus eStatus, MsgPayload* pPayload);

   // Builds the payload for the reset main micro command
   // Inputs:  None
   // Outputs: pPayload - populated with payload, length & checksum
   // Returns: None
   // Notes:   None
   LIB_API
   void BuildResetMainMicroCommand(MsgPayload* pPayload);

   LIB_API
   void BuildEnableDisableBtCommand(bool bEnable, MsgPayload* pPayload);


// end C++ guard
#ifdef __cplusplus
} // closing brace for extern "C"

// end macro guard
#endif

#endif
