#ifndef __TRANSBUFF_H__
#define __TRANSBUFF_H__

#include "buff/XBuffTemplateManager.h"

namespace KKSG
{
	class STransformBuff;
}
class Role;;
class Scene;
class TransformBuff
{
public:
	TransformBuff();
	~TransformBuff();

	void ToKKSG(KKSG::STransformBuff* sbuff);
	void ToThis(const KKSG::STransformBuff* sbuff);

	void Reset();
	bool Valid() { return buff.buffID != 0; }

	void Del(Role* role, int reason);
	void Add(Role* role, UINT32 nowtime, UINT64 casterid);

	UINT32 GetTimeLeft();

	static bool IsSmallBuff(UINT32 buffid);
	static bool CanTransOther(UINT32 itemid);

public:
	bool effectingoption; 
	bool ispassive;
	BuffDesc buff;
};


#endif