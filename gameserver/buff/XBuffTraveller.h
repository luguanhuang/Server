#ifndef __XBUFFTRAVELLER_H__
#define __XBUFFTRAVELLER_H__

#include "pb/project.pb.h"

template <typename T>
void BuffFetcher(void *arg, int buffID, int buffLevel)
{
	T *pApp = (T *)arg;
	KKSG::Buff *pBuff = pApp->add_buffs();
	pBuff->set_buffid(buffID);
	pBuff->set_bufflevel(buffLevel);
}

#endif // __XBUFFTRAVELLER_H__