#include <string.h>
#include "commandHandlers.h"

CommandHandlers m_pHandlers = { 0 };

/********************************************************************************
 *
 * Name:    SetCommandHandlers
 *
 * Purpose: Sets reference to command handler structure.
 *
 * Inputs:  pHandlers - reference to command handler structure
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes:   None
 *
 *******************************************************************************/
void SetCommandHandlers(CommandHandlers* pHandlers)
{
   m_pHandlers = *pHandlers; 
}


