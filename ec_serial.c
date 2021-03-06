/*
  Copyright (C) 2020 Embed Creativity LLC
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/************************************************************************/
/*                                                                      */
/*  ec_serial.c --  EmbedCreativity's serial routines                   */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*  Author:     Mark Taylor                                             */
/*                                                                      */
/************************************************************************/
/*  Module Description:                                                 */
/*                                                                      */
/*  This module was written to facilitate the use of the PC's serial    */
/*  comm port for interfacing with embedded devices.                    */
/*                                                                      */
/*  This file was written specifically for Ubuntu 8.10 Linux            */
/*  installations and as such, may not work on other distrubutions of   */
/*  Linux.                                                              */
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*  02/02/2009 (MarkT): created                                         */
/*  10/11/2016 (MarkT): updated to remove dependence on ascii strings   */
/*                      removed xuart stuff as I'm not likely going to  */
/*                      to use anything from Technologic Systems ever   */
/*                      again.                                          */
/*  02/24/2017 (MarkT): Adding timeout capability to SerialRead()       */
/*  07/16/2017 (MarkT): Packaged into a library                         */
/*                                                                      */
/************************************************************************/

/* ------------------------------------------------------------ */
/*              Include File Definitions                        */
/* ------------------------------------------------------------ */

#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#include "ec_serial.h"

/* ------------------------------------------------------------ */
/*              Local Type Definitions                          */
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*              Global Variables                                */
/* ------------------------------------------------------------ */

static int fd;

/* ------------------------------------------------------------ */
/*              Local Variables                                 */
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*              Forward Declarations                            */
/* ------------------------------------------------------------ */

static bool    GetTimeDiff ( struct timeval *result,
            struct timeval *t2, struct timeval *t1 );

/* ------------------------------------------------------------ */
/*              Procedure Definitions                           */
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***    SerialWriteNBytes
**
**  Synopsis:
**      int SerialWriteNBytes(uint8_t *rgbChars, int n)
**
**  Parameters:
**      *rgbChars       pointer to string
**      n               number of characters to be written to serial port
**
**  Return Values:
**      0               failure
**      1               success
**
**  Errors:
**      none
**
**  Description:
**      Uses the a globally defined integer "file descriptor" fd to write
**      to the serial port.  The serial port must be previously opened and
**      the fd must be the assigned file descriptor for the serial port.
**      Please use the SerialInit(char *szDevice) prior to using this function.
*/
bool SerialWriteNBytes(uint8_t *rgbChars, int n) {

    int i;

    for(i = 0; i < n; i++) {
        //printf("SerialWriteNBytes(n=%d)\n", n);
        int n = write(fd, rgbChars, 1);
        if (n < 0) {
            //printf("Write Error : %d,  %s\n", errno, strerror(errno));
            //printf("0x%x ", *rgbChars);
            return false;
        }
        //printf("0x%x ", *rgbChars);
        rgbChars++; //increment pointer
    }
    return true;
}

/* ------------------------------------------------------------ */
/***    SerialWriteByte
**
**  Synopsis:
**      int SerialWriteByte(uint8_t *pByte)
**
**  Parameters:
**      *pByte          pointer to a char
**
**  Return Values:
**      0               failure
**      1               success
**
**  Errors:
**      none
**
**  Description:
**      Uses the a globally defined integer "file descriptor" fd to write
**      to the serial port.  The serial port must be previously opened and
**      the fd must be the assigned file descriptor for the serial port.
**      Please use the SerialInit(char *szDevice) prior to using this function.
*/
bool SerialWriteByte(uint8_t *pByte) {

    int n = write(fd, pByte, 1);
    if (n < 0) {
        //printf("Write Error : %d,  %s\n", errno, strerror(errno));
        //printf("0x%x ", *pByte);
        return false;
    }

    return true;
}

/* ------------------------------------------------------------ */
/***    SerialRead
**
**  Synopsis:
**      int SerialRead(uint8_t *result)
**
**  Parameters:
**      *result         pointer to string
**
**  Return Values:
**      number of characters read on success
**      -1 on error
**
**  Errors:
**      none
**
**  Description:
**      Uses the a globally defined integer "file descriptor" fd to write
**      to the serial port.  The serial port must be previously opened and
**      the fd must be the assigned file descriptor for the serial port.
**      Please use the SerialInit(char *szDevice) prior to using this function.
**      Places the read NULL terminated string into *result.
*/
int SerialRead(uint8_t *result, uint32_t len, uint32_t timeOutMs) {
    int bytesRead;
    int totalBytesRead;
    struct timeval tvBegin, tvEnd, tvDiff;
    long int elapsedMs;
    uint8_t *ptr;

    // init locals
    ptr = result;
    totalBytesRead = 0;

    // Get timestamp starting now
    gettimeofday(&tvBegin, NULL);

    do {
        // call serial read function
        bytesRead = read(fd, ptr, len);

        totalBytesRead += bytesRead;
        if (bytesRead < 0) {
            if (errno == EAGAIN) {
                printf("SERIAL EAGAIN ERROR\n");
                return SERIAL_ERROR_CODE;
            } else {
                printf("SERIAL read error %d %s\n", errno, strerror(errno));
                return SERIAL_ERROR_CODE;
            }
        } else {
            // caller does not have a timeout defined. Return
            // what we've got now
            if ( timeOutMs == 0 ) {
                return bytesRead;
            }
            if ( bytesRead == 0 ) {
                usleep(1000); // sleep 1ms before calling read again
            }
            if ( len < bytesRead ) {
                printf("ERROR: Read too much data!\n");
                return SERIAL_ERROR_CODE;
            } else {
                ptr += bytesRead; // advance pointer
                len -= bytesRead; // decrement desired count of future bytes
            }
        }
        // grab timestamp
        gettimeofday(&tvEnd, NULL);
        // calculate elapsed time
        if ( GetTimeDiff(&tvDiff, &tvEnd, &tvBegin) ) {
            elapsedMs = (1000*tvDiff.tv_sec + (tvDiff.tv_usec/1000));
        } else {
            printf("ERROR: elapsed time error\n");
            return SERIAL_ERROR_CODE;
        }
    } while ( (elapsedMs < timeOutMs) && (len > 0) );

    if ( elapsedMs >= timeOutMs ) {
        return SERIAL_TIMEOUT_CODE;
    }
    return totalBytesRead;
}

/* ------------------------------------------------------------ */
/***    SerialGetBaud
**
**  Synopsis:
**      int SerialGetBaud( void )
**
**  Parameters:
**      none
**
**  Return Values:
**      inputSpeed          integer value relating baud rate
**
**  Errors:
**      none
**
**  Description:
**      Uses the a globally defined integer "file descriptor" fd to write
**      to the serial port.  The serial port must be previously opened and
**      the fd must be the assigned file descriptor for the serial port.
**      Please use the SerialInit(char *szDevice) prior to using this function.
**      Places the defined baud rate value into inputSpeed and returns.
*/
int SerialGetBaud( void ) {
    struct termios termAttr;
    int inputSpeed = -1;
    speed_t baudRate;
    tcgetattr(fd, &termAttr);
    /* Get the input speed. */
    baudRate = cfgetispeed(&termAttr);
    switch (baudRate) {
        case B0:      inputSpeed = 0; break;
        case B50:     inputSpeed = 50; break;
        case B110:    inputSpeed = 110; break;
        case B134:    inputSpeed = 134; break;
        case B150:    inputSpeed = 150; break;
        case B200:    inputSpeed = 200; break;
        case B300:    inputSpeed = 300; break;
        case B600:    inputSpeed = 600; break;
        case B1200:   inputSpeed = 1200; break;
        case B1800:   inputSpeed = 1800; break;
        case B2400:   inputSpeed = 2400; break;
        case B4800:   inputSpeed = 4800; break;
        case B9600:   inputSpeed = 9600; break;
        case B19200:  inputSpeed = 19200; break;
        case B38400:  inputSpeed = 38400; break;
        case B57600:  inputSpeed = 57600; break;
        case B115200:  inputSpeed = 115200; break;
        case B230400:  inputSpeed = 230400; break;
    }
    return inputSpeed;
}


/* ------------------------------------------------------------ */
/***    IntToBaud
**
**  Synopsis:
**      int IntToBaud( void )
**
**  Parameters:
**      int baud
**
**  Return Values:
**      OS Baud Rate type
**
**  Errors:
**      none
**
**  Description:
*/
speed_t IntToBaud( int baudRate ) {
    speed_t osBaudRate;

    switch (baudRate) {
        case 0:         osBaudRate = B0; break;
        case 50:        osBaudRate = B50; break;
        case 110:       osBaudRate = B110; break;
        case 134:       osBaudRate = B134; break;
        case 150:       osBaudRate = B150; break;
        case 200:       osBaudRate = B200; break;
        case 300:       osBaudRate = B300; break;
        case 600:       osBaudRate = B600; break;
        case 1200:      osBaudRate = B1200; break;
        case 1800:      osBaudRate = B1800; break;
        case 2400:      osBaudRate = B2400; break;
        case 4800:      osBaudRate = B4800; break;
        case 9600:      osBaudRate = B9600; break;
        case 19200:     osBaudRate = B19200; break;
        case 38400:     osBaudRate = B38400; break;
        case 57600:     osBaudRate = B57600; break;
        case 115200:    osBaudRate = B115200; break;
        case 230400:    osBaudRate = B230400; break;
        default: osBaudRate = B0; break;
    }
    return osBaudRate;
}

/* ------------------------------------------------------------ */
/***    SerialInit
**
**  Synopsis:
**      int SerialInit( char *szDevice )
**
**  Parameters:
**      char *szDevice           string indicating location of serial port "file"
**      int baudRate
**
**  Return Values:
**      1                   success
**      0                   failure
**
**  Errors:
**      none
**
**  Description:
**      Uses the a globally defined integer "file descriptor" fd to write
**      to the serial port.  The serial port must be previously opened and
**      the fd must be the assigned file descriptor for the serial port.
**      Please use the SerialInit(char *szDevice) prior to using this function.
**      Opens the serial port with a baud rate of 9600.  No parity or flow control.
**      One stop bit.
*/
bool SerialInit(char *szDevice, int baudRate) {

    struct termios options;
    memset (&options, 0, sizeof(options)); // clear whatever was in there before

    fd = open(szDevice, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        printf("Unable to open %s!", szDevice);
        return false;
    } else {
        fcntl(fd, F_SETFL, 0);
    }

    // Get the current options for the port...
    if ( 0 != tcgetattr(fd, &options) ) {
        printf("SerialInit Error - unable to get options!\n");
        return false;
    }

    // Set the baud rates
    cfsetispeed(&options, IntToBaud(baudRate));
    cfsetospeed(&options, IntToBaud(baudRate));
    // Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_cflag &= ~PARENB; //no parity
    options.c_cflag &= ~CSTOPB; //1 stop bit
    options.c_cflag &= ~CSIZE; //ready value for character size
    options.c_cflag |= CS8; //8-bit character size
    options.c_iflag &= ~IGNBRK; // disable break processing
    options.c_lflag = 0; // no signalling chars, no echo, no canonical processing
    options.c_oflag = 0; // no remapping, no delays
    options.c_cc[VMIN] = 0; // read doesn't block
    options.c_cc[VTIME] = 0; //polling mode. No timeout, return what's there now
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // no xon/xoff ctrl
    options.c_cflag &= ~CRTSCTS;

    // Set the new options for the port...
    if ( 0 != tcsetattr(fd, TCSANOW, &options) ) { //change attributes NOW
        printf("SerialInit Error - unable to set options!\n");
        return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/***    SerialClose
**
**  Synopsis:
**      void SerialClose( void)
**
**  Parameters:
**
**
**  Return Values:
**
**
**  Errors:
**      none
**
**  Description:
**      Uses the a globally defined integer "file descriptor" fd to write
**      to the serial port.  The serial port must be previously opened and
**      the fd must be the assigned file descriptor for the serial port.
**      Please use the SerialInit(char *szDevice) prior to using this function.
**      Closes the fd file descriptor.
*/
void SerialClose(void) {
    if(0 > close(fd)) {
        printf("SerialClose: Error - %d,  %s\n", errno, strerror(errno));
    }
    else {
        printf("Serial connection closed.\n");
    }
}

// Helper functions for enabling a timeout
static bool GetTimeDiff ( struct timeval *result, struct timeval *t2, struct timeval *t1 )
{
    long int diff;
    //long int elapsedMs;

    diff = (t2->tv_usec + (1000000 * t2->tv_sec)) - (t1->tv_usec + (1000000 * t1->tv_sec));
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    //elapsedMs = (1000*result->tv_sec + (result->tv_usec/1000));
    //printf("%ld.%06ld\n", result->tv_sec, result->tv_usec);
    //printf("elapsed milliseconds: %ld\n", elapsedMs);

    if ( diff > 0 ) {
        return true;
    } else {
        return false;
    }
}




/************************************ EOF ********************************/
