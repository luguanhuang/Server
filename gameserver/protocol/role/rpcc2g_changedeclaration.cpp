#include "pch.h"
#include "role/rpcc2g_changedeclaration.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"
#include "util/XCommon.h"
#include "tsssdk/tss_sdk_uic.h"
#include "tsssdk/TssHandler.h"

// generate by ProtoGen at date: 2017/2/7 14:11:19

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeDeclaration, ChangeDeclarationArg, ChangeDeclarationRes)

	void RpcC2G_ChangeDeclaration::OnCall(const ChangeDeclarationArg &roArg, ChangeDeclarationRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT32 nameLen = (UINT32)XCommon::CountUTF8Char(roArg.declaration());
	if(nameLen > GetGlobalConfig().RoleDeclarationLenMax)
	{
		roRes.set_result(KKSG::ERR_DECLARATION_TOO_LONG);
		return;
	}

	RoleMisc* record = role->Get<RoleMisc>();
	if (!record)
	{
		return;
	}

	TssSdkUicNameUserInputInfoV2 info;
	memset(&info, 0, sizeof(info));
	info.msg_ = (unsigned char*)roArg.declaration().c_str();
	info.msg_len_ = roArg.declaration().length();
	info.door_level_ = 1;
	info.if_replace_ = 1;
	if (0 == TssHandler::Instance()->judge_user_input_name(&info))
	{
		roRes.set_declaration((char *)info.msg_);
	}
	else
	{
		roRes.set_declaration(roArg.declaration());
	}

	record->SetDeclaration(roArg.declaration());
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_ChangeDeclaration::OnDelayReplyRpc(const ChangeDeclarationArg &roArg, ChangeDeclarationRes &roRes, const CUserData &roUserData)
{
}
