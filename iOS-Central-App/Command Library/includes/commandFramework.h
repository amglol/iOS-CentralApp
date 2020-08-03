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
* DESCRIPTION: Contains definitions common to command library headers
*
* NOTES:       None
*
********************************************************************************/
#ifndef COMMAND_COMMON_H
#define COMMAND_COMMON_H
/********************************************************************************
*                              I N C L U D E S
********************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "commandParameters.h"

#define TMP_BUFF_SIZE 256
#define TMP_STR_SIZE 256

// szPayload framing character definitions
#define SOH '\x01'
#define STX '\x02'
#define ETX '\x03'
#define ACK '\x06'
#define NAK '\x15'

/********************************************************************************
*                          D E F I N I T I O N S
********************************************************************************/
// defines szPayload frame headers
#define HEADER_ASCII       "PH+A"
#define HEADER_BIN         "PH+B"
#define HEADER_TRANSPORT   "PH+T"
/*********************************************************************************
*                            E N U M S
*********************************************************************************/

/*********************************************************************************
*                            S T R U C T U R E S
*********************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

   LIB_API
   MessageFrameResult ParseMessageFrames(char* cmdBuffer, size_t cmdBufferSize, MessageFrame* msgFrame);
   // Parses message framing into MessageFrame structure
   // Inputs:  None.
   // Outputs: None.
   // Returns: None.
   // Notes:   The command buffer should contain a full message containing all framing

   LIB_API
   void AddMessageFraming(MsgPayload* pPayload, size_t cmdBufferSize, char* pCmdBuffer);
   // Adds framing
   // Inputs:  None.
   // Outputs: None.
   // Returns: None.
   // Notes:   None.

   LIB_API
   uint8_t CalculateChecksum(MsgPayload* pPayload);
   // Calculates the checksum for a given buffer
   // Inputs:  pBuffer - pointer to data buffer
   //          nLength - length of data in buffer
   // Outputs: None.
   // Returns: None.
   // Notes:   None.
   
#ifdef __cplusplus
}
#endif

#endif
