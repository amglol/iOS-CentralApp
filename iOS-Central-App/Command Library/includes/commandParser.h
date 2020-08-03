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
* PROJECT:     MIDAS
*
* COMPILER:    COMPILER
*
* TOOLS:       Microsoft Visual Studio 2019
*
* DESCRIPTION: Command library for serial communications.
*
* NOTES:       This module defines the commands and command parsing structure
*					for MIDAS serial communications.
*
********************************************************************************/
#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "commandHandlers.h"

   /************************************************************************
   * Purpose: 	Increases vacuum flow rate
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse IncreaseVacuumFlow(void);

   /************************************************************************
   * Purpose: 	Decreases vacuum flow rate
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse DecreaseVacuumFlow(void);


   // Get the number of procedure log entries.
   // Inputs:  None.
   // Outputs: pCount - retrieve the number
   // Returns: None.
   // Notes:   None.
   LIB_API
   EHandlerResponse GetProcedureLogCount(int* pCount);

   /************************************************************************
   * Purpose: 	Returns a list of all procedure logs available on the TSF
   *			   controller
   * Inputs:  	char* m_cmdBuffer,
   *			   size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:    None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetProcedureLogList(MsgPayload* pPayload, ProcedureLog* pEntries, int* pCount);

   // Returns list of requested log entries.
   // Inputs:  pPayload - message payload with command parameters
   // Outputs: pEntries - populated with list of requested procedure log entries.
   //          pCount   - populated with number of returned entries. May be less than requested.
   // Returns: None
   // Notes:   None
   LIB_API
   EHandlerResponse GetProcedureLogEntryList(MsgPayload* pPayload, LogEntry* pEntries, int* pCount);

   /************************************************************************
   * Purpose: 	Returns a complete procedure log entry
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs:  None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:    None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetProcedureLogEntry(MsgPayload* pPayload , LogDetails* pDetails);

   // Returns a complete list of all alarm log entries
   // Inputs:  m_cmdBuffer - command payload buffer
   //          cmdBufferSize - command payload buffer size
   // Outputs: pDetails - populated with system log entry details.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse GetAlarmLogList(int nIndex, int nOffset, LogEntry* pEntries, size_t nCount);

   // Returns the details of a single alarm log entry
   // Inputs:  m_cmdBuffer - command payload buffer
   //          cmdBufferSize - command payload buffer size
   // Outputs: pDetails - populated with system log entry details.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse GetAlarmLogEntry(MsgPayload* pPayload, LogDetails* pDetails);

   /************************************************************************
   * Purpose: 	Writes a configuration value into flash memory into  a
   *			specific location addressed by field name
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse WriteManufacturerField(MsgPayload* pPayload);

   /************************************************************************
   * Purpose: 	Reads a configuration value from  flash memory
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse ReadManufacturerField(MsgPayload* pPayload, char* pField);

   /************************************************************************
   * Purpose: 	Restores the system back to default settings
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse RestoreDefaultSettings(void);

   /************************************************************************
   * Purpose: 	Returns a szPayload that gathers all parameters displayed on the
   *			main screen and functions as a kind of initializtion szPayload
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	No parameters needed 
   ************************************************************************/
   LIB_API
   EHandlerResponse GetScreenReadyData(ScreenReady* pScreenReady);

   /************************************************************************
   * Purpose: 	Keeps the touchscreen in sync with the mobile application.
   *			Touchscreen values are updated in response to systme changes
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetSyncData(SyncDataInfo* pSyncData);

   /************************************************************************
   * Purpose: 	Firmware data exchange szPayload provides the firmware versions
   *			of all configured components
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: None
   * Returns: EResponseOk if successful, error code otherwise
   * Notes:   None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetFirmwareVersion(FirmwareVersion* pVersion);

   /************************************************************************
   * Purpose: 	Firmware data exchange szPayload provides the firmware versions
   *			of all configured components
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: None
   * Returns: EResponseOk if successful, error code otherwise
   * Notes:   None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetFirmwareInfo(FirmwareInfo* pInfo);

   /************************************************************************
   * Purpose: 	Returns the time and date
   * Inputs:  	char* m_cmdBuffer,
   *			   size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetTimeAndDate(char* pTime);

   /************************************************************************
   * Purpose: 	Sets the time and date for the system
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse SetTimeAndDate(MsgPayload* pPayload);

   /************************************************************************
   * Purpose: 	Stops the gas flow
   * Inputs:   char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns:  EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse StopGasFlow(void);

   /************************************************************************
   * Purpose: 	Sets the oxygen mix percentage
   * Inputs:  	char *pBuffer,
   *			size_t bufferSize,
   *			uint8_t newMixPercValue
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse SetO2MixPercentage(MsgPayload* pPayload);

   // Gets the O2 mix percentage
   // Inputs: None
   // Outputs: pPercentage
   // Returns: EResponseOk if successful, error code otherwise
   LIB_API
   EHandlerResponse GetO2MixPercentage(int* pPercentage);

   /************************************************************************
   * Purpose: 	Sets the value for the total flow rate
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	None
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse SetTotalFlowRate(MsgPayload* pPayload);

   // Returns value for total flow rate
   // Inputs:  None
   // Outputs: pTotalFlowRate - populated with total flow rate
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse GetTotalFlowRate(int* pTotalFlowRate);

   // Gets values for individual flow rates
   // Inputs:  None
   // Outputs: pO2FlowRate - populated with O2 flow rate
   //          pN2OFlowRate - populated with N2O flow rate
   //          pScavFlowRate - populated with scavenger flow rate
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse GetFlowRates(int* pO2FlowRate, int* pN2OFlowRate, int* pScavFlowRate);

   // Prepares Sets the language for the user interface
   // Inputs:  m_cmdBuffer
   // Outputs: pEnabled - security PIN status. enabled/disabled.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   This command instructs the TSF controller that the procedure
   //		      is coming to an end
   LIB_API
   EHandlerResponse SetLanguage(MsgPayload* pPayload);

   // Prepares Sets the language for the user interface
   // Inputs:  m_cmdBuffer
   // Outputs: pEnabled - security PIN status. enabled/disabled.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   This command instructs the TSF controller that the procedure
   //		      is coming to an end
   LIB_API
   EHandlerResponse GetLanguage(ELanguage* pLanguage);

   // Prepares the system to start the gas procedure
   // Inputs:  m_cmdBuffer
   // Outputs: pEnabled - security PIN status. enabled/disabled.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   This command instructs the TSF controller that the procedure
   //		      is coming to an end
   LIB_API
   EHandlerResponse StartProcedure(void);


   // This command enables or disables the PIN security for dispensing N2O gas
   // Inputs:  m_cmdBuffer
   // Outputs: pEnabled - security PIN status. enabled/disabled.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   This command instructs the TSF controller that the procedure
   //		      is coming to an end
   LIB_API
   EHandlerResponse EndProcedure(void);

   // This command enables or disables the PIN security for dispensing N2O gas
   // Inputs:  m_cmdBuffer
   // Outputs: pEnabled - security PIN status. enabled/disabled.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:
   LIB_API
   EHandlerResponse EnableDisablePin(MsgPayload* pPayload);

   // Command to update the security PIN on the flow meter
   // Inputs:  m_cmdBuffer - contains the command payload
   //          cmdBufferSize - command payload length
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse ChangePin(MsgPayload* pPayload);

   // Heartbeat command parser. Forwards request to the handler
   // Inputs:  None
   // Outputs: None
   // Returns: None
   // Notes:   None
   LIB_API
   EHandlerResponse Heartbeat(void);

   // Sets the gas valve to a known position
   // Inputs:
   // Outputs:
   // Returns:
   // Notes:
   LIB_API
   EHandlerResponse SetValvePosition(MsgPayload* pPayload);

   // Sets the gas valve to a known position
   // Inputs:
   // Outputs:
   // Returns:
   // Notes:
   LIB_API
   EHandlerResponse GetValvePosition(EGasId* pId, int* pPosition);

   /************************************************************************
   * Purpose: 	Returns the configuration data when it is changed
   * Inputs:  	char* m_cmdBuffer,
   *			size_t cmdBufferSize
   * Outputs: 	None
   * Returns: 	EResponseOk if successful, error code otherwise
   * Notes:   	None
   ************************************************************************/
   LIB_API
   EHandlerResponse GetConfigurationData(ConfigData* pData);

   // Loads configuration data from TSF controller
   // Inputs:  m_cmdBuffer - contains the command payload
   //          cmdBufferSize - command payload length
   // Outputs: pConfig - populated with configuration data
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse SetConfigurationData(MsgPayload* pPayload);

   // Turns scavenger vacuum on/off
   // Inputs:  m_cmdBuffer - contains the command payload
   //          cmdBufferSize - command payload length
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse EnableDisableVacuum(MsgPayload* pPayload);

   // Mutes audible alarm.
   // Inputs:  m_cmdBuffer - command payload buffer
   //          cmdBufferSize - payload buffer length   
   // Outputs: None
   // Returns: None
   // Notes:   None
   LIB_API
   EHandlerResponse MuteAlarm(void);

   // Turns power on/off for the touchscreen
   // Inputs: m_cmdBuffer - command payload buffer
   //         cmdBufferSize - payload buffer length
   // Outputs:
   // Returns:
   // Notes:
   LIB_API
   EHandlerResponse EnableDisableTouchscreenPower(MsgPayload* pPayload);

   // Places micro in boot loader mode for firmware download.
   // Inputs:  None
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:
   LIB_API
   EHandlerResponse FirmwareDownload(void);

   // Enables gas flow
   // Inputs:  m_cmdBuffer - command payload buffer
   //          cmdBufferSize - payload buffer length
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse EnableGasFlow(MsgPayload* pPayload);

   // Places the BT chip into firmware download mode
   // Inputs:  None.
   // Outputs: None.
   // Returns: None.
   // Notes:   None.
   LIB_API
   EHandlerResponse BTFirmwareDownload(void);

   // Returns information about the scavenger
   // Inputs:  None.
   // Outputs: pInfo - populated with scavenger info data
   // Returns: None.
   // Notes:   None.
   LIB_API
   EHandlerResponse GetScavengerInfo(ScavengerInfo* pInfo);

   // Retrieves info about the total volume of gas dispensed.
   // Inputs:  None.
   // Outputs: pInfo - populated with gas volume info
   // Returns: None.
   // Notes:   None.
   LIB_API
   EHandlerResponse GetGasVolumeInfo(GasVolumeInfo* pInfo);

   // Set O2 gas mixture to 100%
   // Inputs:  None
   // Outputs: None  
   // Returns: EResponseOk if successful, error code otherwise.
   // Notes:   None.
   LIB_API
   EHandlerResponse FlushO2(MsgPayload* pPayload);

   // Places BT chip in firmware download mode.
   // Inputs:  None
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise.
   // Notes:   None.
   LIB_API
   EHandlerResponse BtFirmwareDownload(void);

   // Writes a manufacturer field to flash
   // Inputs:  m_cmdBuffer      - command payload buffer
   //          cmdBufferSize  - length of payload buffer
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes: data is formatted as Field=Value
   LIB_API
   EHandlerResponse WriteManufacturersField(MsgPayload* pPayload);

   // Read a manufacturers field from flash
   // Inputs:  m_cmdBuffer      - command payload buffer
   //          cmdBufferSize  - length of payload buffer
   // Outputs: pszValue - populated with value read from flash memory
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse ReadManufacturersField(MsgPayload* pPayload, char* pszValue);

   // Set the max values for N2O gas
   // Inputs:  m_cmdBuffer      - command payload buffer
   //          cmdBufferSize  - length of payload buffer
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse SetMaxN2O(MsgPayload* pPayload);

   // Set the max values for N2O gas
   // Inputs:  None
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse GetMaxN2O(int* pMaxValue);

   // Gets the mix step size value
   // Inputs:  None
   // Outputs: pStepSize - populated with step size
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse GetMixStepSize(EMixStepSize* pStepSize);

   // Set the mix step size value
   // Inputs:  None
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse SetMixStepSize(MsgPayload* pPayload);

   // Set the flow rate step size value
   // Inputs:  None
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse SetFlowRateStepSize(MsgPayload* pPayload);

   // Gets the flow rate step size value
   // Inputs:  None.
   // Outputs: pStepSize - populated with flow rate step size;
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None.
   LIB_API
   EHandlerResponse GetFlowRateStepSize(EFlowRateStepSize* pStepSize);

   // Get the clock format
   // Inputs:  None
   // Outputs: pFormat - populated with the clock format.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None.
   LIB_API
   EHandlerResponse GetClockFormat(EClockFormat* pFormat);

   // Get the clock format
   // Inputs:  None
   // Outputs: pFormat - populated with the clock format.
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None.
   LIB_API
   EHandlerResponse SetClockFormat(MsgPayload* pPayload);

   LIB_API
   EHandlerResponse GetBtStatus(EBtStatus* pStatus);

   // Enables/Disables Bluetooth for the device
   // Inputs:  cmdBuffer - buffer containing the command code & parameters
   //          cmdBufferSize - the length of the command buffer
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse EnableDisableBt(MsgPayload* pPayload);

   // Resets values for gas volume dispensed
   // Inputs:  cmdBuffer - buffer containing the command code & parameters
   //          cmdBufferSize - the length of the command buffer
   // Outputs: None
   // Returns: EResponseOk if successful, error code otherwise
   // Notes:   None
   LIB_API
   EHandlerResponse ResetGasVolume(MsgPayload* pPayload);

#ifdef __cplusplus
}
#endif

#endif


