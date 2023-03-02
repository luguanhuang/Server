#ifndef __XBUFFREDUCEDAMAGE_H__
#define __XBUFFREDUCEDAMAGE_H__

#include "XBuff.h"

struct HurtInfo;
struct ProjectDamageResult;

typedef std::pair<double, double> PieceWiseData;
class XPieceWiseDataMgr
{
	std::vector<PieceWiseData> m_Datas;

	bool bRangeIsSet;
	bool bDataIsSet;
	PieceWiseData m_Min;
	PieceWiseData m_Max;

public:
	XPieceWiseDataMgr();
	~XPieceWiseDataMgr();
	void SetRange(double xmin, double ymin, double xmax, double ymax);
	void Init(const vector<Sequence<float, 2>>& datas);

	double GetData(double x);
};

class XBuffReduceDamage : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffReduceDamage(const vector<Sequence<float, 2>>& reduceDamages, int buffid);
	~XBuffReduceDamage();
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual bool OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result);

	virtual void OnAttributeChanged(CombatAttrDef attr);
	virtual XBuffEffectPrioriy GetPriority();
private:
	
	XPieceWiseDataMgr m_Data;

	int m_buffID;
	XEntity *m_Entity;
};

#endif // __XBUFFREDUCEDAMAGE_H__