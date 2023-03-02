#ifndef __PROFESSIONMGR_H__
#define __PROFESSIONMGR_H__

#include "ProfessionTable.h"
#include "ProfessionConvertTable.h"
#include "ProfessionGrowthPercentTable.h"

typedef std::unordered_map<int, double> ConvertCoefficientList;

class ProfessionMgr
{
	ProfessionMgr();
	~ProfessionMgr();
	DECLARE_SINGLETON(ProfessionMgr)

public:

	bool Init();
	void Uninit();

	ProfessionConvertTable::RowData *GetProfessionAttrConvertParams(int profID, int attrID);
	const ConvertCoefficientList *GetProfessionAttrConvertCoefficients(int profID, int attrID);
	ProfessionGrowthPercentTable::RowData *GetProfessionGrowthPercentConf(int profID);
	const std::string &GetPrefabName(int profID);

	static int  GetBasicProfession(int Prof);
	static bool IsBasicProfession(int Prof);
	static int  CheckProtmote(int FromProf, int ToProf, int Level);
	static int  PromoteLevel(int Prof);

	void GetAllProfession(std::vector<UINT32>& profs);

private:
	static bool CanPromote(int FromProf, int ToProf);

	ProfessionConvertTable m_oConvertTable;
	ProfessionGrowthPercentTable m_oGrowthTable;
	std::unordered_map<int, ProfessionConvertTable::RowData*> m_oConvertIndex;
	std::unordered_map<int, ConvertCoefficientList*> m_oConvertCoefficients;
};

#endif // __PROFESSIONMGR_H__