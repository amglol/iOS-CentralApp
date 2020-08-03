#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commandFramework.h"

/********************************************************************************
 *
 * Name:    CalculateChecksum
 *
 * Purpose: Calculates checksum on a buffer of nLength bytes.
 *
 * Inputs:  pBuffer  - buffer to message data
 *       
 *
 * Outputs: None
 *
 * Returns: Calculated checksum
 *
 * Notes:   None
 *
 *******************************************************************************/
LIB_API
uint8_t CalculateChecksum(MsgPayload* pPayload)
{
   uint8_t  nChecksum   = 0;
   uint32_t nCount      = 0;
   char*    pReader     = pPayload->szPayload;

   for (nCount = 0; nCount < pPayload->nLength; nCount++)
   {
      nChecksum += *pReader++;
   }

   return nChecksum;
}

/********************************************************************************
 *
 * Name:    ParseMessageFrames
 *
 * Purpose: cmbBuffer - reference to command buffer
 *          cmdBufferSize - total length of command buffer
 *          
 *
 * Inputs:  msgFrame - populated with message validation info.
 *
 * Outputs: None.
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
LIB_API
MessageFrameResult ParseMessageFrames(char* cmdBuffer, size_t cmdBufferSize, MessageFrame* msgFrame)
{
   // assume success until we fail
   MessageFrameResult eResult       = MFR_OK;
   char szSpecifier[10] = { 0 };
   char* pReader                    = cmdBuffer;
   size_t nTotalRead                = 0;
   char  szTmp[256] = { 0 };
   char  nEtx                       = 0;
   char  nStx                       = 0;
   uint8_t  nComputedChecksum       = 0;
   char* pCh                        = NULL;
   char* pEnd                       = NULL;
   int nScannedItems                = 0;
   int nCmdCode                     = 0;
   // check pointers
   if(cmdBuffer != NULL
	&& msgFrame  != NULL)
   {

      // read STX character from buffer
      sscanf(pReader, "%c", szTmp);
      // convert ascii to numeric
      nStx = *(char*)szTmp;
      pReader++;
      nTotalRead++;
      if (nStx != STX)
      {
         eResult = MFR_STX_ERR;
      }
      else
      {
         msgFrame->bStxFound = true;

         // tokenize string. read until the comma
         pCh = StrTokenize(pReader, ",");
         msgFrame->payload.nLength = atoi(pCh);
         pReader     += strlen(pCh);
         nTotalRead  += strlen(pCh);

         // restore destructive change from strtok
         *pReader = ',';

         // skip the comma
         if(*pReader == ',')
         {
            pReader++;
            nTotalRead++;

            // buffer length check
            if((nTotalRead + msgFrame->payload.nLength) <= cmdBufferSize)
            {
               // copy payload from message frame
               memcpy(msgFrame->payload.szPayload, pReader, msgFrame->payload.nLength);

               // move pointer past payload
               pReader     += msgFrame->payload.nLength;
               nTotalRead  += msgFrame->payload.nLength;

               // if ETX isn't where expected
               if (*(pReader + 3) != ETX)
               {
                  eResult = MFR_ETX_ERR;
               }
               // if the end of the command isn't where expected
               else if (*(pReader + 4) != 0)
               {
                  eResult = MFR_LENGTH_ERR;
               }
               else
               {
                  // only checksum(3 bytes) & ETX remain in the buffer
                  if((nTotalRead + 4) <= cmdBufferSize)
                  {
                     // reset temp buffer
                     memset(szTmp, 0, sizeof(szTmp));

                     // read checksum from buffer and convert from ascii
                     sscanf(pReader, "%3c", szTmp);
                     msgFrame->payload.nChecksum = atoi(szTmp);
                     pReader += 3;

                     // reset temp buffer. read ETX and convert from ascii
                     memset(szTmp, 0, sizeof(szTmp));
                  
                     sscanf(pReader, "%c", szTmp);
                     nEtx = *(char*)szTmp;

                     // validate the checksum
                     nComputedChecksum = CalculateChecksum(&msgFrame->payload);
                     if (nComputedChecksum == msgFrame->payload.nChecksum)
                     {
                        msgFrame->bChecksumOk = true;

                        // check for ETX
                        if(nEtx == ETX)
                        {
                           msgFrame->bEtxFound = true;

                           // read until comma for command code
                           pCh = StrTokenize(msgFrame->payload.szPayload, ",");

                           // if payload is for a command response
                           if(strcmp(pCh, "RSP") == 0)
                           {
                              // skip the prefix, and restore delimiter
                              RestoreDelimiterAndMovePast(&pCh, ',');
                           }

                           // try to convert string to int
                           nCmdCode = strtol(pCh, &pEnd, 10);

                           // if command code is non printable (ACK or NAK)
                           // conversion fails, just read the character
                           if(nCmdCode == 0)
                           {
                              // just read it's value
                              nCmdCode = (int)*pCh;
                           }
                           // convert command code from hex string to int
                           msgFrame->eCmdType = (ECommandCode)nCmdCode;

                           // skip the prefix, and restore delimiter
                           RestoreDelimiterAndMovePast(&pCh, ',');

                           // if command code is out of range
                           if ((msgFrame->eCmdType != EAck && msgFrame->eCmdType != ENak)
                           && ((msgFrame->eCmdType <= ECommandCodeMin) || msgFrame->eCmdType >= ECommandCodeMax))
                           {
                              eResult = MFR_COMMAND_ERR;
                           }
                        }
                        else
                        {
                           eResult = MFR_ETX_ERR;
                        }
                     }
                     else
                     {
                        eResult = MFR_CHKSUM_ERR;
                     }
                  }
               }
            }
            else
            {
               eResult = MFR_BUFF_LEN_ERR;
            }
         }
      }
   }

   return eResult;
}

/********************************************************************************
 *
 * Name:    AddMessageFraming
 *
 * Purpose: Adds message framing to an outgoing message.
 *
 * Inputs:  pPayload - command payload
 *
 * Outputs: pCmdBuffer - output buffer
 *
 * Returns: TODO:
 *
 * Notes:   TODO:
 *
 *******************************************************************************/
LIB_API
void AddMessageFraming(MsgPayload* pPayload, size_t cmdBufferSize, char* pCmdBuffer)
{
   char szHeader[5] = {0};

   // if buffer is valid and data will fit
   if (pCmdBuffer != NULL
   && (pPayload->nLength + PREAMBLE_LENGTH + 1) <= cmdBufferSize)
   {
      // clear previous data before populating buffer
      memset(pCmdBuffer, 0, cmdBufferSize);

      sprintf(pCmdBuffer,
         "%c%04d,%s%03d%c", 
         (char)STX,
         pPayload->nLength,
         pPayload->szPayload,
         pPayload->nChecksum,
         (char)ETX);
   }
}