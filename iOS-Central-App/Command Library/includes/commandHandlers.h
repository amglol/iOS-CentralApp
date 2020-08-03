#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H
#include <stdbool.h>
#include <stdio.h>
#include "commandParameters.h"

typedef enum EHandlerResponse
{
   EInputBufferError,
   EOutputBufferError,
   EBuffSizeError,
   EOpNotAllowed,
   EInvalidParameters,
   EOutOfRangeLow,
   EOutOfRangeHigh,
   EResponseOk,
   EDataCorrupt,
   EMaxEHandlerResponse
}
EHandlerResponse;

typedef EHandlerResponse(*FnHandleNoParameterCommand)(void);

typedef EHandlerResponse(*FnHandleEnableGasFlow)(EGasId eId, bool bEnabled);
typedef EHandlerResponse(*FnHandleSetBoolCommand)(bool bEnabled);

//typedef EHandlerResponse(*FnHandleSetLanguage)(ELanguage* pLanguage);
//typedef EHandlerResponse(*FnHandleGetLanguage)(ELanguage* pLanguage);
typedef EHandlerResponse(*FnHandleSetIntValueCommand)(int nValue);

typedef EHandlerResponse(*FnHandleGetIntValueCommand)(int* pPosition);
typedef EHandlerResponse(*FnHandleGetMixStepSizeCommand)(EMixStepSize* pStepSize);
typedef EHandlerResponse(*FnHandleGetLanguageCommand)(ELanguage* pLang);
typedef EHandlerResponse(*FnHandleGetFlowRateStepSizeCommand)(EFlowRateStepSize* pStepSize);
typedef EHandlerResponse(*FnHandleGetClockFormatCommand)(EClockFormat* pClockFormat);
typedef EHandlerResponse(*FnHandleGetBtStatusCommand)(EBtStatus* pStatus);
typedef EHandlerResponse(*FnHandleSetStringParameterCommand)(const char* pValue);
typedef EHandlerResponse(*FnHandleGetStringParameterCommand)(char* pValue);

typedef EHandlerResponse(*FnHandleGetLogList)(int nOffset, int nCount, ProcedureLog* pEntries, int* pCount);
typedef EHandlerResponse(*FnHandleGetLogEntry)(int nIndex, int nOffset, int nCount, LogEntry* pEntries, int* pCount);
typedef EHandlerResponse(*FnHandleGetFlowRates)(int* pO2, int* pN2O, int* pScavenger);

typedef EHandlerResponse(*FnHandleGetScreenReady)(ScreenReady* pScreenReady);
typedef EHandlerResponse(*FnHandleSyncData)(SyncDataInfo* pSyncData);
typedef EHandlerResponse(*FnHandleGetFirmwareInfo)(FirmwareInfo* pInfo);
typedef EHandlerResponse(*FnHandleGetFirmwareVersion)(FirmwareVersion* pVersion);
typedef EHandlerResponse(*FnHandleGetConfigurationData)(ConfigData* pData);
typedef EHandlerResponse(*FnHandleSetConfigurationData)(ConfigData* pData);
typedef EHandlerResponse(*FnHandleSetValve)(EGasId eId, int nPosition);
typedef EHandlerResponse(*FnHandleGetValve)(EGasId* pId, int* pPosition);

typedef EHandlerResponse(*FnHandleWriteManufacturerField)(const char* pFieldName, const char* pFieldValue);
typedef EHandlerResponse(*FnHandleReadManufacturerField)(const char* pFieldName, char* pFieldValue);

typedef EHandlerResponse(*FnHandleGetScavengerInfo)(ScavengerInfo* pInfo);
typedef EHandlerResponse(*FnHandleGetGasVolumeInfo)(GasVolumeInfo* pInfo);


typedef struct __CommandHandlers
{
   FnHandleNoParameterCommand			   fpHandleIncreaseVacuumFlow;
   FnHandleNoParameterCommand			   fpHandleDecreaseVacuumFlow;
   FnHandleNoParameterCommand			   fpHandleStartProcedure;
   FnHandleNoParameterCommand			   fpHandleEndProcedure;
   FnHandleNoParameterCommand          fpHandleMuteAlarm;
   FnHandleNoParameterCommand			   fpHandleHeartbeat;
   FnHandleNoParameterCommand          fpHandleFirmwareDownload;
   FnHandleSetBoolCommand              fpHandleO2Flush;
   FnHandleNoParameterCommand          fpHandleBtFirmwareDownload;
   FnHandleNoParameterCommand	         fpHandleRestoreDefaultSettings;
   FnHandleNoParameterCommand			   fpHandleSendPinChallenge;
   FnHandleNoParameterCommand			   fpHandleCancelPinChallenge;
   FnHandleSetBoolCommand			      fpHandleEnableDisablePin;
   FnHandleSetBoolCommand              fpHandleEnableDisableVacuum;
   FnHandleSetBoolCommand              fpHandleEnableDisablePower;
   FnHandleSetBoolCommand              fpHandleEnableDisableBt;
   FnHandleGetIntValueCommand          fpHandleGetProcedureLogCount;
   FnHandleGetLogList			         fpHandleGetProcedureList;
   FnHandleGetLogEntry			         fpHandleGetProcedureEntryList;
   FnHandleGetLogList                  fpHandleGetAlarmLogList;
   FnHandleGetLogEntry                 fpHandleGetAlarmLogEntry;
   FnHandleGetLanguageCommand		      fpHandleGetLanguage;
   FnHandleSetIntValueCommand		      fpHandleSetLanguage;
   FnHandleSetIntValueCommand          fpHandleSetMaxN2OMixPercent;
   FnHandleGetIntValueCommand          fpHandleGetMaxN2OMixPercent;
   FnHandleSetIntValueCommand	         fpHandleSetO2MixPercent;
   FnHandleGetIntValueCommand          fpHandleGetO2MixPercent;
   FnHandleSetIntValueCommand			   fpHandleSetTotalFlowRate;
   FnHandleGetIntValueCommand          fpHandleGetTotalFlowRate;
   FnHandleGetFlowRates                fpHandleGetFlowRates;
   FnHandleNoParameterCommand		      fpHandleStopGasFlow;
   FnHandleSetBoolCommand			      fpHandleBtToggle;
   FnHandleSetStringParameterCommand   fpHandleSendPinResponse;
   FnHandleSetStringParameterCommand   fpHandleChangePin;
   FnHandleGetStringParameterCommand	fpHandleGetTimeAndDate;
   FnHandleSetStringParameterCommand	fpHandleSetTimeAndDate;
   FnHandleSetStringParameterCommand   fpHandleSendMessage;
   FnHandleGetScreenReady				   fpHandleScreenReady;
   FnHandleSyncData						   fpHandleSyncData;
   FnHandleGetFirmwareVersion          fpHandleGetFirmwareVersion;
   FnHandleGetFirmwareInfo		         fpHandleGetFirmwareInfo;
   FnHandleGetConfigurationData		   fpHandleGetConfigurationData;
   FnHandleSetConfigurationData		   fpHandleSetConfigurationData;
   FnHandleEnableGasFlow               fpHandleEnableGasFlow;
   FnHandleWriteManufacturerField	   fpHandleWriteManufacturerField;
   FnHandleReadManufacturerField		   fpHandleReadManufacturerField;
   FnHandleSetValve                    fpHandleSetValve;
   FnHandleGetValve                    fpHandleGetValve;
   FnHandleGetScavengerInfo            fpHandleGetScavengerInfo;
   FnHandleGetGasVolumeInfo            fpHandleGetGasVolumeInfo;
   FnHandleSetIntValueCommand          fpHandleResetGasVolumeInfo;
   FnHandleGetMixStepSizeCommand       fpHandleGetMixStepSize;
   FnHandleSetIntValueCommand          fpHandleSetMixStepSize;
   FnHandleGetFlowRateStepSizeCommand  fpHandleGetFlowRateStepSize;
   FnHandleSetIntValueCommand          fpHandleSetFlowRateStepSize;
   FnHandleGetClockFormatCommand       fpHandleGetClockFormat;
   FnHandleSetIntValueCommand          fpHandleSetClockFormat;
   FnHandleGetBtStatusCommand          fpHandleGetBtStatus;
   FnHandleSetIntValueCommand          fpHandleSetBtStatus;
   
}CommandHandlers;

extern CommandHandlers m_pHandlers;

#ifdef __cplusplus
extern "C" {
#endif

   LIB_API
   void SetCommandHandlers(CommandHandlers* pHandlers);
   // Sets function pointers to command handlers
   // Inputs:  pHandlers - pointer to command handler structure. 
   // Outputs: None.
   // Returns: None.
   // Notes:   None.

#ifdef __cplusplus
}
#endif

#endif
