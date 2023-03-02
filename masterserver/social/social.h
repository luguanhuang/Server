#ifndef __SOCIAL_H__
#define __SOCIAL_H__

#include "tshow.h"
#include "flower.h"

namespace KKSG
{
	class SocialInfo2Db;
}
struct SocialInfo
{
	SocialInfo(UINT64 roleID);
	Flower flower;
	TShow tshow;

	void SocialInfo2Db(KKSG::SocialInfo2Db* socialinfo2db);
	void Db2SocialInfo(const KKSG::SocialInfo2Db* socialinfo2db);
};

#endif