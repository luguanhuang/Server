#ifndef __XBUFFREDUCESKILLCD_H__
#define __XBUFFREDUCESKILLCD_H__

#include "XBuff.h"

class XSkillMgr;

class XBuffReduceSkillCD : public BuffEffect
{
public:
	static bool TryCreate(CombatEffectHelper* pEffectHeler, XBuff *pBuff);

	XBuffReduceSkillCD(XBuff *pBuff);
	~XBuffReduceSkillCD();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);
private:
	void _AddCD(XSkillMgr* pSkillMgr, UINT32 skillHash, UINT32 value, UINT32 type);
	void _RemoveCD(XSkillMgr* pSkillMgr, UINT32 skillHash, UINT32 value, UINT32 type);

	XBuff *m_pBuff;
	XEntity *m_pEntity;

	std::vector<Sequence<UINT32, 3>> m_Datas;
};

#endif // __XBUFFREDUCESKILLCD_H__