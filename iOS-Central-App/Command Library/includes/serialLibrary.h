#ifndef SERIAL_LIBRARY_H
#define SERIAL_LIBRARY_H
#include <stdint.h>
#include <stdbool.h>


bool OpenSerialPort();
// Opens and configures the serial port
// Inputs: None
// Outputs: None
// Returns: true if port was opened, false otherwise
// Notes: None

void CloseSerialPort();
// Closes the serial port.
// Inputs: None
// Outputs: None
// Returns: None
// Notes: None

uint64_t Write(void* pMsg, uint64_t nLength);
// Writes data to the serial port.
// Inputs:  pMsg - pointer to data to send across serial line
//          nLength - the nLength of the data to send.
// Outputs: None
// Returns: The number of bytes sent
// Notes:   None


uint64_t Read(void * pMsg, uint64_t nLength);
// Reads data from the serial port.
// Inputs: pMsg - pointer to buffer to store data.
// Outputs: None
// Returns: the number of bytes read
// Notes: None


#endif // end macro guard

