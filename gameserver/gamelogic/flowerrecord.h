#ifndef __FLOWERRECORD_H__
#define __FLOWERRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "globalconfig.h"

#define REWARDRANKNUM 10
class Role;
namespace KKSG
{
	class GetFlowerRewardListRes;
}

class CFlowerRecord : public RoleSerializeInterface
{
public:
	CFlowerRecord(Role* role);

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

public:
	void Update();
	
	UINT32 GetCount() { return  GetGlobalConfig().DailyFlowerCount <= (int)m_rolessend.size() ?
		0 :  GetGlobalConfig().DailyFlowerCount - (int)m_rolessend.size(); }
	bool IsCountLimit();
	bool IsRoleLimit(UINT64 roleid);

	void SendFlower(UINT64 roleid, UINT32 count);

	//总共送出的鲜花数量
	UINT32 GetTotalSend();

	UINT32 GetLeftFlowerTime();
	UINT32 GetFlower();
	bool CheckGetRankReward();
	UINT32 GetFlowerSuccessCount(){return m_getNum;}
	void OnFirstEnterScene();
	void OnFirstEnterSceneReply(UINT32 rank, UINT32 totalRank);
	UINT32 GetRankReward(int rank);
	void OnDayPass();
	void GetFlowerRewardList(KKSG::GetFlowerRewardListRes& roRes);
private:
	Role* m_pRole;
	Switch m_ismodify;
	UINT32 m_updateday;
	std::multimap<UINT64, UINT32> m_rolessend;
	bool m_getRankReward;
	UINT32 m_getFlowerTime;
	UINT32 m_getNum;
};

#endif