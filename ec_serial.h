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
/*  ec_serial.h --  EmbedCreativity's Serial Comm header file           */
/*                                                                      */
/************************************************************************/
/*  Author:     Mark Taylor                                             */
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*  This header file contains declarations the functions contained in   */
/*  ec_serial.c                                                         */
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*  02/02/2009(MarkT): created                                          */
/*  02/24/2017(MarkT): added support for timeout in SerialRead()        */
/*                                                                      */
/************************************************************************/

#if !defined(_SERIALCONTROL_H)
#define _SERIALCONTOL_H

#include <stdint.h>  /* for unsigned values*/


/* ------------------------------------------------------------ */
/*                  Definitions                                 */
/* ------------------------------------------------------------ */
#define SERIAL_ERROR_CODE   -1
#define SERIAL_TIMEOUT_CODE -2

/* ------------------------------------------------------------ */
/*                  General Type Declarations                   */
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*                  Object Class Declarations                   */
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*                  Variable Declarations                       */
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*                  Function Prototypes                         */
/* ------------------------------------------------------------ */

bool    SerialWriteNBytes(uint8_t *rgbChars, int n);
bool    SerialWriteByte(uint8_t *pByte);
int     SerialRead(uint8_t *result, uint32_t len, uint32_t timeOutMs);
int     SerialGetBaud(void);
bool    SerialInit(char *szDevice, int baudRate);
void    SerialClose(void);
/* ------------------------------------------------------------ */

#endif

/**********************************  EOF  **************************************/





