
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#ifndef __DES_H__
#define __DES_H__

int encrypt(unsigned char key[8], unsigned char* data, int blocks);
int decrypt(unsigned char key[8], unsigned char* data, int blocks);

#endif
