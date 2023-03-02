#ifndef __CUSTOMBATTLEHANDLE_H__
#define __CUSTOMBATTLEHANDLE_H__

#include "crpc.h"
#include "custombattle/custombattle.h"

//CustomBattle_Query = 1;
//CustomBattle_QueryRandom = 7;
//CustomBattle_QueryOne = 8;
//CustomBattle_QuerySelf = 14;
//CustomBattle_Search = 18;

//CustomBattle_Create = 2;
//CustomBattle_DoCreate = 9;
//CustomBattle_Modify = 13;
//CustomBattle_StartNow = 15;
//CustomBattle_Drop = 17;

//CustomBattle_Join = 3;
//CustomBattle_DoJoin = 10;
//CustomBattle_UnJoin = 11;

//CustomBattle_Match = 4;
//CustomBattle_UnMatch = 12;

//CustomBattle_Reward = 5;
//CustomBattle_ClearCD = 6;
//CustomBattle_DoClearCD = 16;

struct CustomBattleReq
{
	CustomBattleReq()
	{
		op = KKSG::CustomBattle_Query;
		uid = 0;
		roleid = 0;
		delayid = 0;
		iscross = false;
        matchinfo = NULL;
	}
	~CustomBattleReq()
	{
		if (NULL != matchinfo)
		{
			delete matchinfo;
			matchinfo = NULL;
		}
	}

	KKSG::CustomBattleOp op;
	UINT64 uid;
	bool iscross;
	KKSG::CustomBattleConfig conf;
    KKSG::CustomBattleMatchInfo* matchinfo; 

	UINT64 roleid;
	UINT32 delayid;

	std::string password;
	std::string battlename;
	std::string token;
};

class CRole;
class CustomBattleMgr;
class CustomBattleHandle
{
public:
	virtual ~CustomBattleHandle() { };

	void Init(CustomBattleMgr* manager) { m_manager = manager; }

	virtual void HandleClientReq(const CustomBattleReq& req) = 0;

	void ErrorNotify(KKSG::ErrorCode errorcode, UINT32 delayid)
	{
		CUserData data;
		data.m_dwUserData = errorcode;
		data.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(delayid, data);
	}
	void DelayReply(KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo* info, UINT32 delayid)
	{
		CUserData data;
		data.m_dwUserData = errorcode;
		data.m_pUserPtr = (void*)info;
		CRpc::ReplyDelayRpc(delayid, data);
	}
	CustomBattleMgr* m_manager;	
};

//CustomBattle_Query = 1;
//CustomBattle_QueryRandom = 7;
//CustomBattle_QueryOne = 8;
class CustomBattleHandleQuery : public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};

//CustomBattle_Modify = 13;
class CustomBattleHandleModify: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};

//CustomBattle_Create = 2;
//CustomBattle_DoCreate = 9;
class CustomBattleHandleCreateBase: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);
};
class CustomBattleHandleCreate: public CustomBattleHandleCreateBase
{
public:
	void HandleGsReply(const CustomBattleReq& req);
};
class CustomBattleHandleCreateWorld: public CustomBattleHandleCreateBase
{
public:
	void HandleGsReply(const CustomBattleReq& req);
	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};


//CustomBattle_Join = 3;
//CustomBattle_DoJoin = 10;
class CustomBattleHandleJoin: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

private:
	KKSG::ErrorCode HandleJoin(const CustomBattleReq& req, CustomBattle* battle);
};
class CustomBattleHandleJoinSystem: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleGsReply(const CustomBattleReq& req);
	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};
class CustomBattleHandleJoinWorld: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};

//CustomBattle_UnJoin = 11;
class CustomBattleHandleUnJoin: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

private:
	KKSG::ErrorCode HandleUnJoin(const CustomBattleReq& req, CustomBattle* battle);
};
class CustomBattleHandleUnJoinSystem: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);

private:
	KKSG::ErrorCode HandleUnJoin(const CustomBattleReq& req);
};
class CustomBattleHandleUnJoinWorld: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};

//CustomBattle_Match = 4;
class CustomBattleHandleMatch: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};
//CustomBattle_UnMatch = 12;
class CustomBattleHandleUnMatch: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);
};

//CustomBattle_Reward = 5;
//CustomBattle_ClearCD = 6;
class CustomBattleHandleReward: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleGsReply(const CustomBattleReq& req);
};
class CustomBattleHandleRewardSystem: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleGsReply(const CustomBattleReq& req);
};
class CustomBattleHandleRewardWorld: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleGsReply(const CustomBattleReq& req);
	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};

//CustomBattle_Drop = 17;
class CustomBattleHandleDrop: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);
};

//CustomBattle_Search = 18;
class CustomBattleHandleSearch: public CustomBattleHandle
{
public:
	virtual void HandleClientReq(const CustomBattleReq& req);

	void HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info);
};

#endif
