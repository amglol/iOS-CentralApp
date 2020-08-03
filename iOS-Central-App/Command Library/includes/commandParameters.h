#ifndef COMMAND_PARAMETER_H
#define COMMAND_PARAMETER_H


#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
   #ifdef DLL_EXPORT
      #define LIB_API __declspec(dllexport)
   #else
      #define LIB_API __declspec(dllimport)
   #endif
#else
   #define LIB_API
#endif

#define DATE_TIME_BUFF_SIZE 32
#define LOG_ENTRY_DETAILS_LEN 512

// max nLength of a szPayload frame header
#define TOTAL_FRAMING_BYTES 11

// 4 byte length (minimum), 3 byte checksum, STX, ETX and Comma
#define PREAMBLE_LENGTH 10
#define PAYLOAD_LENGTH (256 - TOTAL_FRAMING_BYTES)

// header file content

/*********************************************************************************
*                          E N U M E R A T I O N S
*********************************************************************************/
// unique ID for for command types
typedef enum _ECommandCode
{
   EAck  = 0x06,
   ENak  = 0x15,
   ECommandCodeMin = 0x20, // add command below
   EVacuumIncrease,
   EVacuumDecrease,
   EStartProcedure,
   EEndProcedure,
   EEnableDisablePin,
   EGetProcedureLogCount,
   EGetProcedureList,
   EGetProcedureLog,
   EGetProcedureEntryCount,
   EGetProcedureEntryList,
   EGetProcedureEntry,
   EGetAlarmLogList,
   EGetAlarmLogEntry,
   EResetLogs,
   ESetLanguage,
   EGetLanguage,
   ESetTotalFlowRate,
   EGetTotalFlowRate,
   EGetFlowRates,
   ESetO2MixPercentage,
   EGetO2MixPercentage,
   EStopGas,
   EEnableDisableBT,
   EEnableDisableVacuum,
   ERestoreDefaults,
   EChangePin,
   EMuteAlarm,
   EEnableDisablePower,
   EGetTimeAndDate,
   ESetTimeAndDate,
   EGetGasVolume,
   EResetGasVolume,
   EGetScavengerInfo,
   EFlushO2,
   EHeartbeat ,
   EScreenReady,
   ESyncData,
   EGetFirmwareVersion,
   EGetFirmwareInfo,
   EGetConfigData,
   ESetValve,
   EGetValve,
   EFirmwareDownload,
   EEnableGasFlow,
   EBtFirmwareDownload,
   EWriteManufacturerField,
   EReadManufacturerField,
   ESetN2OMax,
   EGetN2OMax,
   EGetMixStepSize,
   ESetMixStepSize,
   EGetFlowRateStepSize,
   ESetFlowRateStepSize,
   EGetClockFormat,
   ESetClockFormat,
   EGetBtStatus,
   ECommandCodeMax

} ECommandCode;

typedef enum _EFlowRateStepSize
{
   EFlowRateStepSizeMin,
   EPointOneLPM,
   EPointFiveLPM

}EFlowRateStepSize;

typedef enum _EMixStepSize
{
   EMixStepSizeMin,
   EOnePercent,
   EFivePercent
}EMixStepSize;

typedef enum _EClockFormat
{
   ETwelveHour,
   ETwentyFourHour
}EClockFormat;

typedef enum _ELanguage
{
   ELanguageMin,
   EEnglish,
   ESpanish,
   EGerman,
   Italian,
   French
}ELanguage;

// unique ID for log types
typedef enum _ELogId
{
   EProcedure,
   ESystems,
   EAlarms
}ELogId;

typedef enum _EProcedureLogId
{
   EStarted,
   EStopped,
   EGasMixAdjust,
   EGasFlowAdjust,
   EScavFlowAdjust,
   EFault,
   EO2RecoveryStarted,
   EO2RecoveryExpired,
   EO2FlushStarted,
   EO2FlushEnded,
   EGasFlowRecord
}EProcedureLogId;

typedef enum _EGasId
{
   EO2,
   EN2O,
   EGasIdMax
}EGasId;

typedef enum _EBtStatus
{
   EBtStatusMin,
   EOff,
   EDisconnected,
   EConnected
}EBtStatus;

typedef enum _MessageFrameResult
{
   MFR_OK           = 0,
   MFR_STX_ERR,
   MFR_ETX_ERR,
   MFR_HEADER_ERR,
   MFR_LENGTH_ERR,
   MFR_CHKSUM_ERR,
   MFR_COMMAND_ERR,
   MFR_BUFF_LEN_ERR
} MessageFrameResult;
/*********************************************************************************
*                          S T R U C T U R E S
*********************************************************************************/

typedef struct _MsgPayload
{
   int nChecksum;
   size_t nLength;
   char szPayload[PAYLOAD_LENGTH];
}MsgPayload;

typedef struct _MessageFrame
{
   MsgPayload  payload;
   ECommandCode eCmdType;
   bool bStxFound;
   bool bEtxFound;
   bool bChecksumOk;
} MessageFrame;

// contains configuration data
typedef struct _ConfigData
{
   int nMaxN20;
   EMixStepSize   eMixStepSize;
   EFlowRateStepSize eFlowStepSize;
   EClockFormat eClockFormat;
   ELanguage eLanguage;
   char szTime[DATE_TIME_BUFF_SIZE];
}ConfigData;

// screen ready response data
typedef struct _ScreenReady
{
   ConfigData configData;
   bool bTouchscreenPowerState;

}ScreenReady;

typedef struct _SyncDataInfo
{
   int nO2MixPercentage;
   int nTotalFlowRate;
   int nO2FlowRate;
   int nN2OFlowRate;
   int nScavengerFlowRate;     
   bool bTouchscreenPowerState;  
   bool bInProgress;             // signals a procedure has been started
   bool bEnding;                 // signals a procedure is ending
   bool bStopGas;
   ELanguage eDefaultLanguage;
}SyncDataInfo;

// contains firmware info for a single component
typedef struct _FirmwareVersion
{
   int nMajor;
   int nMinor;
   int nRevision;
}FirmwareVersion;

// contains firmware info for all components
typedef struct _FirmwareInfo
{
   FirmwareVersion mainController;
   FirmwareVersion blueTooth;
   FirmwareVersion gui;
   FirmwareVersion scavenger;
}FirmwareInfo;

// stores values from gas dispensed screen
typedef struct _GasVolumeInfo
{
   int nO2VolumeDispensed;
   int nN2OVolumeDispensed;
   char szO2LastReset[DATE_TIME_BUFF_SIZE];
   char szN2OLastReset[DATE_TIME_BUFF_SIZE];
}GasVolumeInfo;

// contains info for a log entry (summary)

#define LOG_ENTRY_CNT 8

typedef struct _LogEntry
{
   EProcedureLogId eId;
   uint16_t nValues[LOG_ENTRY_CNT];
   
}LogEntry;

#define PROCEDURE_LOG_ENTRY_CNT 10
#define DATE_TIME_BYTES 6
typedef struct _ProcedureLog
{
   int nDuration;
   int nEntryCount;
   LogEntry* pEntries;
   uint8_t szDate[DATE_TIME_BYTES];
   char szName[DATE_TIME_BUFF_SIZE];
}ProcedureLog;

// contains info for a log entry (detailed)
typedef struct _LogDetails
{
   LogEntry entry;
   char szDetails[LOG_ENTRY_DETAILS_LEN];
}LogDetails;

// response data for get scavenger info command
typedef struct _ScavengerInfo
{
   bool bValveOpen;     // open or closed
   bool bSensorStatus;  // good or bad
   uint8_t nFlowRate;   // 0 -100
}ScavengerInfo;


// C++ guard
#ifdef __cplusplus
extern "C" {
#endif

   // library functions go here
   void RestoreDelimiterAndMovePast(char** pCmdBuffer, char delimiter);

   // strtok replacement function for STM8 micro
   // Tokenizes a string by replacing delimiter with null
   // subsequent calls with s = NULL return the next delimiter
   // Inputs:  s - string to tokenize
   //          delim - array of delimiters
   //          savePtr - the last token position
   char* StrTokenize(char* s, const char* delim);

// end C++ guard
#ifdef __cplusplus
} // closing brace for extern "C"

// end macro guard
#endif

#endif
