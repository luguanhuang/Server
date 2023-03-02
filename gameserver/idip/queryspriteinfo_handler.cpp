#include "pch.h"
#include "commondef_handler.h"
#include "idip/idiperrdesc.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/spriterecord.h"
#include "gamelogic/sprite.h"
#include "gamelogic/spriteconfig.h"

bool CIdipQuerySpriteInfoHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	INT32 PageNo = JsonUtil::GetJsonInt(roBodyJson, "PageNo");

	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(uRoleId);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	JsonBuilder oBuilder;
	rapidjson::Value oList(rapidjson::kArrayType);
	std::vector<Sprite*> vList;
	pRole->Get<CSpriteRecord>()->GetAllList(vList);
	UINT32 Count = 0;
	INT32 iStart = 0;
	INT32 iEnd = 0;
	if (!GetPageNo(vList.size(), PageNo, MAX_FAIRYLIST_NUM, iStart, iEnd))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	for (INT32 i = iStart; i < iEnd; ++ i)
	{
		if (NULL == vList[i])
		{
			continue;
		}
		UINT32 Quality = 0;
		std::string strName = "null";
		auto pConf = SpriteConfig::Instance()->GetSpriteConf(vList[i]->GetSpriteID());
		if (pConf)
		{
			Quality = pConf->SpriteQuality;
			strName = pConf->SpriteName;
		}
		rapidjson::Value stprite(rapidjson::kObjectType);
		stprite.AddMember("OpenId", strOpenId, oBuilder.Allocator());
		stprite.AddMember("UId", pRole->GetNickId(), oBuilder.Allocator());
		stprite.AddMember("RoleId", ToString(pRole->GetID()), oBuilder.Allocator());
		stprite.AddMember("NickName", UrlEncode(pRole->GetName()), oBuilder.Allocator());
		stprite.AddMember("FairyName", UrlEncode(strName), oBuilder.Allocator());
		stprite.AddMember("FairyLevel", vList[i]->GetLevel(), oBuilder.Allocator());
		stprite.AddMember("FairyQual", ToString(Quality), oBuilder.Allocator());
		stprite.AddMember("UpDown", pRole->Get<CSpriteRecord>()->IsInfight(vList[i]->Getuid())?0:1, oBuilder.Allocator());
		stprite.AddMember("IsCaptain", pRole->Get<CSpriteRecord>()->IsLeader(vList[i]->Getuid())?0:1, oBuilder.Allocator());
		stprite.AddMember("ExpNum", vList[i]->GetPowerPoint(), oBuilder.Allocator());
		stprite.AddMember("FairyUid", ToString(vList[i]->Getuid()), oBuilder.Allocator());
		stprite.AddMember("FairyId", vList[i]->GetSpriteID(), oBuilder.Allocator());
		oList.PushBack(stprite, oBuilder.Allocator());
		Count ++;
	}
	oBuilder.AddMember_Int("Result", KKSG::ERR_SUCCESS);
	oBuilder.AddMember_Uint("FairyList_count", Count);
	oBuilder.AddMember_Value("FairyList", oList);
	INT32 TotalPageNo = (vList.size() + MAX_FAIRYLIST_NUM -1) / MAX_FAIRYLIST_NUM;
	oBuilder.AddMember_Int("TotalPageNo", TotalPageNo);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return true;
}
