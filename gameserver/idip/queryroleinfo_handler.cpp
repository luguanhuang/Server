#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/rolesummarymgr.h"
#include "dblink.h"
#include "guild/guildmgr.h"
#include "gamelogic/pkrecord.h"


bool CIdipQueryRoleInfoHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

	Role* poRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == poRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_String("RoleName", UrlEncode(poRole->GetName()).c_str());
	oBuilder.AddMember_Int("Level", poRole->GetLevel());
	oBuilder.AddMember_Uint("Exp", poRole->GetExp());
	oBuilder.AddMember_Uint64("Physical", poRole->Get<Bag>()->CountVirtualItem(FATIGUE));
	oBuilder.AddMember_Uint("RegisterTime", (UINT32)poRole->GetRoleAllInfo().brief().inittime());
	oBuilder.AddMember_Uint("LastLogoutTime", poRole->GetRoleAllInfo().extrainfo().lastlogouttime());
	oBuilder.AddMember_Uint("OnlineTime", poRole->GetOnlineTimeFromLogin() + (UINT32)poRole->GetRoleAllInfo().brief().onlimetime());
	oBuilder.AddMember_Uint("IsOnline", 0);
	oBuilder.AddMember_Int64("DiamondNum", poRole->Get<Bag>()->CountVirtualItem(DIAMOND));
	oBuilder.AddMember_Int64("GoldNum", poRole->Get<Bag>()->CountVirtualItem(GOLD));
	oBuilder.AddMember_Uint("HeartNum", 0);
	oBuilder.AddMember_String("GuildId", ToString(poRole->getGuildId()));
	oBuilder.AddMember_String("GuildName", UrlEncode(GuildSimpleMgr::Instance()->GetGuildName(poRole->GetID()).c_str()));
	UINT32 wincount = poRole->Get<CPkRecord>()->GetWin();
	UINT32 totalcount = poRole->Get<CPkRecord>()->GetCont();
	UINT32 winrate = totalcount?UINT32(wincount*100/totalcount):0;
	oBuilder.AddMember_Int("LadderCount", totalcount);
	oBuilder.AddMember_Int("LadderWin", winrate);
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
