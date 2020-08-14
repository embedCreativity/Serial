# Serial
Linux Serial Port

## Overview
This library is meant to greatly simplify bringing up a serial port in a Unix/Linux environment with easily configurable properties.

The API listed in the header file makes is so simple, that using the library becomes intuitive.

  // Write a defined number of bytes to the serial port
  bool    SerialWriteNBytes(uint8_t *rgbChars, int n);
  
  // Write a single byte to the serial port
  bool    SerialWriteByte(uint8_t *pByte);
  
  // Read up to (len) number of bytes with a configurable timeout
  int     SerialRead(uint8_t *result, uint32_t len, uint32_t timeOutMs);
  
  // Return an integer representation of the currently configured baud rate
  int     SerialGetBaud(void);
  
  // Initialize and open the serial port
  bool    SerialInit(char *szDevice, int baudRate);
  
  // Close the serial port
  void    SerialClose(void);
