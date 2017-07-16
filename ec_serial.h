/************************************************************************/
/*                                                                      */
/*  ec_serial.h --  EmbedCreativity's Serial Comm header file           */
/*                                                                      */
/************************************************************************/
/*  Author:     Mark Taylor                                             */
/*  Copyright 2017, EmbedCreativity                                     */
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





