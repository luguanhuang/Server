#ifndef __ARENARECORD_H__
#define __ARENARECORD_H__

#include "unit/roleserilizeinterface.h"

class CArenaRecord : public RoleSerializeInterface
{
public:
	CArenaRecord ();
	void Reset();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	UINT32 GetOptimalRank();
	void SetOptimalRank(UINT32 dwOptimalRank);

	void Update();

	UINT32 GetPoint();
	void AddPoint(UINT32 point);
	void SetPoint(UINT32 point);

	bool IsGetPointReward(UINT32 index);
	void SetPointReward(UINT32 index);
	const std::set<UINT32>& GetPointReward();

	bool IsGetRankReward(UINT32 index);
	void SetRankReward(UINT32 index);
	const std::set<UINT32>& GetRankReward();

private:
	Switch m_ismodify;
	UINT32 m_dayupdate;
	UINT32 m_optimalrank;
	UINT32 m_point;
	std::set<UINT32> m_pointreward;
	std::set<UINT32> m_rankreward;
};

#endif