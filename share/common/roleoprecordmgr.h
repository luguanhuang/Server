#ifndef __ROLEOPRECORDMGR_H__
#define __ROLEOPRECORDMGR_H__

enum RolePairRecType
{
	RPRT_MATCH_DAY,
	RPRT_MATCH_WEEK,
};

struct RolePairRecK
{
	RolePairRecK(UINT64 roleID1_, UINT64 roleID2_, UINT32 tstab_, int recType_)
		:roleID1(roleID1_), roleID2(roleID2_), tstab(tstab_), recType(recType_)
	{

	}

	bool operator < (const RolePairRecK& rec) const
	{
		if(recType != rec.recType)
		{
			return recType < rec.recType;
		}
		if(roleID1 != rec.roleID1)
		{
			return roleID1 < rec.roleID1;
		}
		if(roleID2 != rec.roleID2)
		{
			return roleID2 < rec.roleID2;
		}
		return tstab < rec.tstab;
	}

	UINT64 roleID1;
	UINT64 roleID2;
	UINT32 tstab;
	int recType;

	void InfoPrint() const;
};

class RoleOpRecMgr
{
public:
	RoleOpRecMgr();

	void Update(time_t now);

	void AddPairRec(UINT64 roleID1, UINT64 roleID2, time_t now);
	bool IsPairLimit(UINT64 roleID1, UINT64 roleID2, time_t now, UINT32 dayLimit, UINT32 weekLimit);

	void InfoPrint();
	void Clear();

private:
	void AddPairRec(const RolePairRecK& reck);
	bool IsPairLimit(const RolePairRecK& reck, UINT32 limit);

	UINT32 GetPairRecCount(const RolePairRecK& reck);

private:

	std::map<RolePairRecK, UINT32> m_rolePair2Count;

	UINT32 m_lastUpDayStab;
};

#endif



