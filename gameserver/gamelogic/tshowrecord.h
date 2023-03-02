#ifndef __TSHOWRECORD_H__
#define __TSHOWRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "globalconfig.h"

class Role;

struct VoteData
{
	int freeCount;
	int costCount;
	VoteData()
	{
		freeCount = 0;
		costCount = 0;
	}
};
class CTShowRecord : public RoleSerializeInterface
{
public:
	CTShowRecord(Role* role);

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

public:
	void Update();
	void DebugReset();
	
	bool IsCountLimit(UINT64 roleID, int type);
	UINT32 Vote(UINT64 roleid, int type);
	UINT32 GetVotedCount(UINT64 roleID, int type);
	void GiveFirstVoteRewardToday();
	UINT32 GetLeftFreeVoteCount();
	bool HaveSendRank(){return m_haveSendRank;}
	void SetSendRank(bool send){m_haveSendRank = send;m_ismodify.Set();}
private:
	Switch m_ismodify;
	UINT32 m_updateday;
	std::map<UINT64, VoteData> m_rolessend;
	Role* m_pRole;
	bool m_haveSendRank;
};

#endif