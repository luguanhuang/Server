#ifndef __INVFIGHTRCROSSEQUEST_H__
#define __INVFIGHTRCROSSEQUEST_H__

namespace KKSG
{
	class InvFightRes;
	class InvFightM2GRes;
	class InvFightBefESpara;
	class InvFightCrossMsg;
}

class InvCrossRec;
struct InvFightCrossProcessDelayInfo
{
	InvFightCrossProcessDelayInfo()
	{
		fromServer = 0;
		toServer = 0;
		fromID = 0;
		toID = 0;
		invID = 0;
	}

	void FillFromInvFightCrossRequest(InvCrossRec& rec);

	UINT32 fromServer;
	UINT32 toServer;
	UINT64 fromID;
	UINT64 toID;
	UINT64 invID;
	std::string fromAccount;
	std::string toAccount;
	KKSG::InvFightBefESpara smallInfo;
};

class CRole;
class InvFightCrossRequest
{
public:
	InvFightCrossRequest(CRole* role);
	~InvFightCrossRequest();

	//int InvOne(UINT64 roleID, KKSG::InvFightM2GRes& m2gRes);
	int CheckPreInv(const std::string& toAccount);
	int CheckPreBeInv(UINT64 fromID);
	int InvFromOne(UINT64 invID, const std::string& toAccount);
	int InvToOne(UINT64 invID, UINT64 fromID);

	int AcceptPre(UINT64 invID);
	int AcceptOneFrom(UINT64 invID);
	int AcceptOneTo(UINT64 invID);

	int RefushOne(UINT64 invID);
	int RefushOneFrom(UINT64 invID);
	int IgnoreAll();

	int ReqList(KKSG::InvFightRes& roRes);

	bool ProcessClientCall(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, InvFightCrossProcessDelayInfo& delayInfo);
	void ProcessClientCallDelay(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, const CUserData &roUserData);

	bool ProcessCrossCall(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, InvFightCrossProcessDelayInfo& delayInfo);

private:
	
	//bool LevelCondition(CRole* pRole);
	bool SceneCondition(CRole* pRole);

	bool PlatFriendCondition(const std::string& account);

	void ReqEnterBattle(const KKSG::InvFightCrossMsg& msg);

	CRole* m_pRole;
};

#endif