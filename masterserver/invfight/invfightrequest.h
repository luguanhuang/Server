#ifndef __INVFIGHTREQUEST_H__
#define __INVFIGHTREQUEST_H__

namespace KKSG
{
	class InvFightRes;
	class InvFightM2GRes;
	class InvFightArg;
}

struct InvFightProcessDelayInfo
{
	InvFightProcessDelayInfo()
	{
		fromID = 0;
	}

	UINT64 fromID;
};

struct CUserData;
class CRole;
class InvFightRequest
{
public:
	InvFightRequest(CRole* role);
	~InvFightRequest();

	int InvOne(UINT64 roleID, KKSG::InvFightM2GRes& m2gRes);
	int RefushOne(UINT64 invID);
	int IgnoreAll();
	int ReqList(KKSG::InvFightRes& roRes);
	int AcceptPre(UINT64 invID, UINT64& fromRoleID);
	int AcceptOne(UINT64 invID);
	
	bool ProcessCall(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, InvFightProcessDelayInfo& delayInfo);
	void ProcessDelay(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, const CUserData &roUserData);

private:
	
	//bool LevelCondition(CRole* pRole);
	bool SceneCondition(CRole* pRole);

	CRole* m_pRole;
};

#endif