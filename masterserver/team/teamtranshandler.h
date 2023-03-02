#ifndef __TEAMTRANSHANDLER_H__
#define __TEAMTRANSHANDLER_H__

#include "define/teamdef.h"

class CRole;
class TeamInviteRec;
struct PlatUserInfo;

class TeamTransHandler
{
public:
	TeamTransHandler();
	~TeamTransHandler();

	static bool NeedInTs(int expID);

	/*组队操作请求*/
	KKSG::ErrorCode HandleRoleOpCond(const KKSG::TeamOPArg& roArg, CRole* pRole);
	bool NeedHandleToTs(const KKSG::TeamOPArg& roArg, CRole* pRole);
	void HandleRoleOpReqFromC(UINT32 delayID, CRole* pRole, const KKSG::TeamOPArg& roArg, int specialType = THSpeType_None);
	void HandleRoleOpResFromTs(UINT32 delayID, UINT64 roleID, const KKSG::TeamOPArg& roArg, const KKSG::TeamOPRes& roRes);

	void HandleRoleOpReqFromTs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg);
	void HandleRoleOpResFromGs(UINT32 delayID, UINT64 roleID, const KKSG::TeamTransData& roArg, const KKSG::TeamTransRes& roRes);

	/*组队列表请求*/
	void HandleFetchTeamReqFromC(UINT32 delayID, UINT64 roleID, const KKSG::FetchTeamListArg& roArg);
	void HandleFetchTeamResFromTs(UINT32 delayID, UINT64 roleID, const KKSG::FetchTeamListRes& roRes);

	/*组队信息同步*/
	void HandleTeamChangeFromTs(UINT64 roleID, const KKSG::TeamChanged& changeData);
	void HandleLeaveTeamFromTs(UINT64 roleID, KKSG::LeaveTeamType reason);
	void HandleRole2TeamFromTs(UINT64 roleID, int teamID);

	/*组队邀请相关*/
	void HandleInviteDataFromTs(const KKSG::TeamTransData& data);
	void HandleRoleAckToTs(CRole* pRole, TeamInviteRec& invdata, bool accept);

	void HandleInvRefuseToTs(CRole* pRole, UINT64 roleID);
	void HandleInvRefuseFromTs(const std::string& name, UINT64 roleID);

	/*匹配*/
	void HandleMatchReqFromC(UINT32 delayID, CRole* pRole, const KKSG::KMatchCommonArg& roArg);
	void HandleMatchResFromTs(UINT32 delayID, const KKSG::KMatchCommonRes& roRes);

	void HandleMatchReqFromTs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg);
	void HandleMatchResFromGs(UINT32 delayID, UINT64 roleID, const KKSG::TeamTransData& roArg, const KKSG::TeamTransRes& roRes);

	bool HandleRoleMatchToTs(CRole* pRole, int expID, bool start);
	void HandleRoleMatchFromTs(UINT32 delayID, CRole* pRole, const KKSG::TeamTransData& roArg);
	void HandleRoleMatchFromGs(UINT32 delayID, UINT64 roleID, const KKSG::TeamTransRes& roRes);

	//通用 无须做其他处理的转发
	void HandleTransArgFromTsToGs(UINT32 delayID, CRole* pRole,  const KKSG::TeamTransData& roArg);
	void HandleTransResFromGsToTs(UINT32 delayID, const KKSG::TeamTransRes& roRes);

	/*断线重连*/
	void HandleTeamFullData(CRole* pRole, const KKSG::TeamFullDataNtf& fullData);

	/*转发聊天*/
	void HandleChatToTs(UINT64 roleID, const KKSG::ChatInfo& chatInfo);
	void HandleChatFromTs(UINT64 roleID, const KKSG::ChatInfo& chatInfo);

	void HandlerGsRegister(UINT32 gsLine);

	void HandleRoleLogout(UINT64 roleID, int teamID);

	void HandleErrorCodeToTs(UINT64 roleID, KKSG::ErrorCode errcode);
	void HandleErrorCodeFromTs(UINT64 roleID, KKSG::ErrorCode errcode);

	void SynTeamExtraInfoToTs(UINT64 roleID);

private:
	void FillRoleDataFromGsBackToTs(UINT64 roleID, const KKSG::TeamTransData& roArg, KKSG::TeamTransRes& tmpRes);
	bool FillRoleMemberData(UINT64 roleID, KKSG::TeamTransRes& tmpRes);
	void HandleCompatibleConf(UINT64 roleID, const KKSG::TeamTransData& roArg, KKSG::TeamTransRes& tmpRes);

	const PlatUserInfo* GetPlatUserInfo(UINT64 roleID, const std::string& account);

};

#endif