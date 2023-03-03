/*
**  @file $RCSfile: tlogio.h,v $
**  general description of this module
**  $Id: tlogio.h,v 1.1 2008-08-05 07:15:26 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008-08-05 07:15:26 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOGIO_H
#define TLOGIO_H

#ifdef __cplusplus
extern "C"
{
#endif


struct tagTLOGIOVec
{
	void *iov_base;   /* Starting address */
	size_t iov_len;   /* Number of bytes */
};

typedef struct tagTLOGIOVec	TLOGIOVEC;
typedef struct tagTLOGIOVec	*LPTLOGIOVEC;

#ifdef __cplusplus
}
#endif

#endif /* TLOGIO_H */


