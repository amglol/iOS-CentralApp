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
* PROJECT:     
*
* COMPILER:    
*
* TOOLS:       
*
* DESCRIPTION: 
*
* NOTES:       
*
********************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "commandParameters.h"



/********************************************************************************
 *
 * Name:    RestoreDelimiterAndMovePast
 *
 * Purpose: Used with strtok function. Restore the delimiter from destructive call
 *          to strtok and advances pointer 1 byte after delimiter.
 *
 * Inputs:  pCmdBuffer - reference to tokenized string
 *          delimiter  - delimiter to restore in pCmdBuffer string
 *          
 * Outputs: None
 *
 * Returns: None.
 *
 * Notes:   None.
 *
 *******************************************************************************/
void RestoreDelimiterAndMovePast(char** pCmdBuffer, char delimiter)
{
   // skip the command code
   // advance pointer to the inserted NULL
   while(**pCmdBuffer != '\0')
   {
      (*pCmdBuffer)++;
   }

   // restore delimiter from destructive call to strtok
   **pCmdBuffer = delimiter;
   (*pCmdBuffer)++;
}

/********************************************************************************
 *
 * Name:    StrTokenize
 *
 * Purpose: strtok replacement function for STM8 micro. their version of string.h
 *          doesn't seem to have this function.
 *
 * Inputs:  s        - the string to tokenize
 *          delim    - array of delimiters
 *          savePtr  - position of last token.
 *
 * Outputs: None
 *
 * Returns: Pointer to last token
 *
 * Notes:   None
 *
 *******************************************************************************/
char* m_pStart = NULL;
char* m_pLast = NULL;
int m_nTokenLen = 0;

char* StrTokenize(char* s, const char* delim)
{
   char* pStart = NULL;
   char* pNext  = NULL;
   char* pReader = NULL;

   // determine if string is new or continuation from previous
   if (s == NULL)
   {
      pStart = m_pLast;
   }
   else
   {
      pStart = s;
      m_pStart = pStart;
      m_nTokenLen = strlen(pStart);
   }

   pReader = pStart;

   if(pStart != NULL)
   {
      while (pReader - m_pStart < m_nTokenLen)
      {
         if (*pReader == *delim)
         {
            *pReader = '\0';
            pNext = pReader;

            if (pNext - m_pStart < m_nTokenLen)
            {
               pNext++;
               m_pLast = pNext;
            }
            else
            {
               pNext = NULL;
               m_pLast = NULL;
            }

            break;
         }

         pReader++;
      }
   }

   if(pStart == m_pLast)
   {
      pStart = NULL;
      m_pStart = NULL;
      m_pLast = NULL;
   }
   
   return pStart;
}

//char* StrTokenize(char* s, const char* delim)
//{
//   char* pReader = s;
//   char* pFound = NULL;
//   unsigned int nLen = 0;
//   int nCnt = 0;
//   bool bFound = false;
//   
//   if (s == NULL)
//   {
//      pReader = m_pLast;
//
//      // if end of string hasn't been reached
//      if(pReader - m_pStart < m_nTokenLen)
//      {
//         // search for next NULL character
//         while(*pReader != '\0')
//         {
//            pReader++;
//         }
//
//         // still not at end of string?
//         if(pReader - m_pStart < m_nTokenLen)
//         {
//            // move past inserted null to next token.
//            pReader++;
//            pFound = pReader;
//            m_pLast = pReader;
//
//            while (pReader - m_pStart < m_nTokenLen)
//            {
//               if(*pReader == *delim)
//               {
//                  *pReader = '\0';
//                  pReader = pFound;
//                  break;
//               }
//
//               pReader++;
//            }
//
//            if(pFound)
//            {
//               pReader = pFound;
//            }
//           
//         }
//         else
//         {
//            pReader = NULL;
//         }
//      }
//   }
//   else
//   {
//      m_pStart = s;
//      m_pLast = s;
//      m_nTokenLen = strlen(s);
//
//      while(pReader - m_pStart < m_nTokenLen)
//      {
//         if (*pReader == *delim)
//         {
//            *pReader = '\0';
//            pFound = pReader;
//            pReader++;
//            break;
//         }
//
//         pReader++;
//      }
//
////      // replace delimiter with NULL
////      for(nCnt = 0 ; nCnt < m_nTokenLen ; nCnt++)
////      {
////         if (*pReader == *delim)
////         {
////            *pReader = '\0';
////            pReader++;
////            break;
////         }
////
////         pReader++;
////      }
//
//      pReader = s;
//   }
//
//   return pReader;
//}