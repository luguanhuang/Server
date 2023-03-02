#ifndef __XBUFFTARGETLIFEADDATTACK_H__
#define __XBUFFTARGETLIFEADDATTACK_H__

#include "XBuff.h"
#include "XBuffReduceDamage.h"

struct HurtInfo;
struct ProjectDamageResult;

class XBuffTargetLifeAddAttack : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffTargetLifeAddAttack(const vector<Sequence<float, 2>>& datas);
	~XBuffTargetLifeAddAttack();
	virtual bool OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result);

	virtual XBuffEffectPrioriy GetPriority();
private:
	
	XPieceWiseDataMgr m_Data;
};

#endif // __XBUFFTARGETLIFEADDATTACK_H__