#ifndef __POWERPOINTMGR_H__
#define __POWERPOINTMGR_H__

#include "DefModTable.h"
#include "PowerPointAdjustTable.h"
#include "PowerPointCoeffTable.h"

class Role;
class CombatAttribute;

class PowerPointMgr
{
	PowerPointMgr();
	~PowerPointMgr();
	DECLARE_SINGLETON(PowerPointMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	float  GetAtkCoeffcient(int Level, double CurrentPPT, double NeedPPT);
	float  GetDefCoeffcient(int Level, double CurrentPPT, double NeedPPT);

	double CalcPowerPoint(CombatAttribute *combatAttr, int prof);

	void AdjustAttributeByCoeffcient(Role *pRole, float atkcoeff, float defcoeff);
	void AdjustOneAttributeByCoeffcient(Role *pRole, int nAttr, float coeff);

	double _GetPowerPointAttributeWeight(int attribute, UINT32 prof);

private:
	DefModTable m_deftable;
	PowerPointAdjustTable m_oTable;
	PowerPointCoeffTable  m_oCoeffTable;

	struct PowerPointCoeffConf
	{
		PowerPointCoeffConf(int _attributeID, UINT32 _prof, double _weight):attributeID(_attributeID), prof(_prof), weight(_weight){}
		int attributeID;
		UINT32 prof;	
		double weight;
		bool operator < (const PowerPointCoeffConf& conf) const
		{
			return prof < conf.prof;
		}
	};

public:
	std::map<UINT32, std::vector<PowerPointCoeffConf>> m_oCoeff;
};

#endif // __POWERPOINTMGR_H__