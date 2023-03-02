#ifndef __INVFIGHTCROSS__H_
#define __INVFIGHTCROSS__H_

#include "timer.h"
#include "invfight/invfightmgr.h"

namespace KKSG
{
	class InvFightRes;
	class InvFightRoleBrief;
	class InvFightCrossMsg;
}

class InvCrossRec
{
public:
	InvCrossRec(UINT64 invID, UINT32 fromServer, UINT32 toServer, const std::string& fromAccount, const std::string& toAccount, UINT64 fromID, UINT64 toID);
	~InvCrossRec();

	inline UINT64 InvID() { return m_invID; }
	inline UINT64 InvFromID() { return m_invFromID; }
	inline UINT64 InvToID() { return m_invToID; }
	inline UINT32& InvTime() { return m_invTime; }
	inline std::string& InvFromAccount(){return m_fromAccount;}
	inline std::string& InvToAccount(){return m_toAccount;}
	inline UINT32 InvFromServer(){return m_fromServer;}
	inline UINT32 InvToServer(){return m_toServer;}
	inline KKSG::InvFightBefESpara& InvSmallInfo(){return smallInfo;}

	UINT32 GetTimeLeft(UINT32 now);

	void FillInvFRoleBrief(KKSG::InvFightRoleBrief& data);

	void TestPrint();

private:
	//bool m_isCross;
	UINT32 m_fromServer;
	UINT32 m_toServer;
	UINT64 m_invID;
	UINT64 m_invFromID;
	UINT64 m_invToID;
	std::string m_toAccount;
	std::string m_fromAccount;

	UINT32 m_invTime;

	KKSG::InvFightBefESpara smallInfo;
};


class InvFightCrossMgr : public ITimer
{
	InvFightCrossMgr();
	~InvFightCrossMgr();
	DECLARE_SINGLETON(InvFightCrossMgr);
public:
	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddFromCrossFight(const KKSG::InvFightCrossMsg& msg);
	void AddToCrossFight(const KKSG::InvFightCrossMsg& msg);
	void Del(UINT64 invID);

	void DelFromIDInv(UINT64 fromID);
	void DelToIDInv(const std::string& toAccount);

	bool HaveInvTo(UINT64 fromID, const std::string& toAccount);
	UINT32 InvFromCount(UINT64 fromID);
	UINT32 InvToCount(const std::string& toAccount);

	InvCrossRec* GetInvUniRec(UINT64 invID);
	RoleRec* GetInvFromRec(UINT64 fromID);
	RoleRec* GetInvToRec(const std::string& toAccount);

	void FillInvToList(const std::string& toAccount, KKSG::InvFightRes& roRes);

	void EnterBattle(UINT32 sceneID, UINT64 roleID, UINT32 gsline, UINT32 mapID);
	//for debug
	void TestClearData() { ClearData(); }
	void TestPrint();

private:
	void CheckTimeOver();

	void GetInvFromList(UINT64 fromID, InvFListType& invList);
	void GetInvToList(/*UINT64 toID*/const std::string& toAccount, InvFListType& invList);
	void DelInvList(InvFListType& invList);

	void _AddFrom(InvCrossRec& rec);
	void _AddTo(InvCrossRec& rec);
	void ClearData();

	std::unordered_map<UINT64, InvCrossRec> m_invRecMap;
	std::unordered_map<UINT64, RoleRec> m_roleFromRecMap;
	std::unordered_map<std::string, RoleRec> m_accountToRecMap;

	HTIMER m_handler;

};

#endif
