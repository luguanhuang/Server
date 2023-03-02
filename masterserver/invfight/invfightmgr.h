#ifndef __INVFIGHT__H_
#define __INVFIGHT__H_

#include "timer.h"

const UINT32 InvFightToCountMax = 100; 

namespace KKSG
{
	class InvFightRes;
	class InvFightRoleBrief;
}

class CRole;

class InvRec
{
public:
	InvRec(UINT64 invFromID, UINT64 invToID);
	InvRec(UINT64 invID,const std::string& toAccount, UINT64 fromID);
	~InvRec();

	inline UINT64 InvID() { return m_invID; }
	inline UINT64 InvFromID() { return m_invFromID; }
	inline UINT64 InvToID() { return m_invToID; }
	inline UINT64 InvTime() { return m_invTime; }
	//inline std::string& InvFromAccount(){return m_fromAccount;}
	inline std::string& InvToAccount(){return m_toAccount;}

	//void SetCross(bool cross){m_isCross = cross;}

	UINT32 GetTimeLeft(UINT32 now);

	void FillInvFRoleBrief(KKSG::InvFightRoleBrief& data);

	void TestPrint();

private:
	bool m_isCross;
	UINT64 m_invID;
	UINT64 m_invFromID;
	UINT64 m_invToID;
	time_t m_invTime;
	//std::string m_fromAccount;
	std::string m_toAccount;
};

typedef std::vector<UINT64> InvFListType;

class RoleRec
{
public:
	RoleRec();	
	~RoleRec();

	void Add(UINT64 invID);
	void Del(UINT64 invID);
	inline UINT32 Size() { return m_invList.size(); }

	const InvFListType& List() { return m_invList;}

	void TestPrint();

private:
	InvFListType m_invList;
};

class InvFightMgr : public ITimer
{
	InvFightMgr();
	~InvFightMgr();
	DECLARE_SINGLETON(InvFightMgr);
public:
	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void MakeInvFight(UINT64 fromID, UINT64 toID);
	void ReadyFight(UINT64 fromID, UINT64 toID);
	void Del(UINT64 invID);

	void CreateSceneBegin(UINT64 fromID, UINT64 toID);
	void CreateSceneEnd(UINT32 sceneID, UINT64 fromID, UINT64 toID);

	void DelFromIDInv(UINT64 fromID);
	void DelToIDInv(UINT64 toID);

	bool IsInDelay(UINT64 roleID);
	bool HaveInvTo(UINT64 fromID, UINT64 toID);
	UINT32 InvFromCount(UINT64 fromID);
	UINT32 InvToCount(UINT64 toID);

	InvRec* GetInvUniRec(UINT64 invID);
	RoleRec* GetInvFromRec(UINT64 fromID);
	RoleRec* GetInvToRec(UINT64 toID);

	void FillInvToList(UINT64 toID, KKSG::InvFightRes& roRes);

	void OnLogin(CRole* role);

	//for debug
	void TestClearData() { ClearData(); }
	void TestPrint();

private:
	void CheckTimeOver();
	void CheckDelayTime();

	void GetInvFromList(UINT64 fromID, InvFListType& invList);
	void GetInvToList(UINT64 toID, InvFListType& invList);
	void DelInvList(InvFListType& invList);

	void Add(InvRec& rec);
	void ClearData();

	std::unordered_map<UINT64, InvRec> m_invRecMap;
	std::unordered_map<UINT64, RoleRec> m_roleFromRecMap;
	std::unordered_map<UINT64, RoleRec> m_roleToRecMap;
	std::unordered_map<UINT64, UINT32> m_delayRoles;//暂时忽略这些玩家的请求

	HTIMER m_handler;

};

#endif