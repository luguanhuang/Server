/*
**  @file $RCSfile: tconvert.h,v $
**  general description of this module
**  $Id: tconvert.h,v 1.5 2009-09-25 06:34:02 hardway Exp $
**  @author $Author: hardway $
**  @date $Date: 2009-09-25 06:34:02 $
**  @version $Revision: 1.5 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCONVERT_H

#define TCONVERT_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



long tconvert_get_bytesl(const char *a_szStr, char** a_ppszEnd, int a_iRadix);
long tconvert_get_daysl(const char *a_szStr, char** a_ppszEnd, int a_iRadix);

char* Bin2Hex(unsigned char * pbin,int binlen, char* phex,int *hexlen);


unsigned char* Hex2Bin(char * phex,int hexlen, unsigned char* pbin,int* binlen);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TCONVERT_H */
