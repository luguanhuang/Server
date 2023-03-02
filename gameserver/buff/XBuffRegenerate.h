#ifndef __XBUFFREGENERATE_H__
#define __XBUFFREGENERATE_H__

class XBuff;
class XEntity;

#include "XBuff.h"

struct DotTimeInfo
{
	int    attrID;
	double attrValue;
	int    interval;
	UINT64 nextTime;
	int    totalCount;
	int    tickCount;
};

class XBuffRegenerate : public BuffEffect
{
public:
	static bool TryCreate(CombatEffectHelper* pEffectHelper, XBuff *pBuff);

	XBuffRegenerate(XBuff *pBuff);
	~XBuffRegenerate();

//	void OnUpdate();
	bool OnTimer(IArgs*, void*);

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);
	virtual void OnResetTime();

private:
	void ConvertSpecialBuff(DotTimeInfo &info);
	void _KillTimer();
	XBuff  *m_pBuff;
	std::vector<DotTimeInfo> m_dots;
	UINT64 start_time;

	HTIMER m_TimerToken;
};

#endif // __XBUFFREGENERATE_H__