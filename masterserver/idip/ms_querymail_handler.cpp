#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "mail/mailmgr.h"
#include "pb/project.pb.h"

INT32 CMSIdipQueryMailHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	INT32 PageNo = JsonUtil::GetJsonUint(roBodyJson, "PageNo");
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (PageNo <= 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	UINT32 TotalPageNo = 0;
	UINT32 dwPageNo = PageNo-1;
	std::vector<KKSG::SMail*> roMailList;
	CMailMgr::Instance()->Fetch(roleid, dwPageNo, TotalPageNo, MAX_MAILLIST_NUM, roMailList);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode).c_str());
	oBuilder.AddMember_Uint("MailList_count", roMailList.size());
	rapidjson::Value oMailList(rapidjson::kArrayType);
	for(UINT32 i = 0; i < roMailList.size(); i ++)
	{
		if (roMailList[i] == NULL)
		{
			continue;
		}
		rapidjson::Value mail(rapidjson::kObjectType);
		mail.AddMember("MailId", ToString(roMailList[i]->uid()), oBuilder.Allocator());
		mail.AddMember("MailTitle", UrlEncode(roMailList[i]->title()), oBuilder.Allocator());
		mail.AddMember("MailContent", UrlEncode(roMailList[i]->content()), oBuilder.Allocator());
		INT32 num = roMailList[i]->items_size() + roMailList[i]->xitems_size();
		std::ostringstream ss;
		ss << num;
		for (UINT32 j = 0;j < roMailList[i]->items_size(); j ++)
		{
			ss << "#";
			ss << roMailList[i]->items(j).itemid() << "-" << roMailList[i]->items(j).itemcount();
		}
		for (UINT32 j = 0; j < roMailList[i]->xitems_size(); j ++)
		{
			ss << "#";
			ss << roMailList[i]->xitems(j).itemid() << "-" << roMailList[i]->xitems(j).itemcount();
		}
		ss << "#";
		mail.AddMember("ItemList", ss.str(), oBuilder.Allocator());
		mail.AddMember("HasGet", roMailList[i]->state(), oBuilder.Allocator());
		oMailList.PushBack(mail, oBuilder.Allocator());
	}
	oBuilder.AddMember_Value("MailList", oMailList);
	oBuilder.AddMember_Int("TotalPageNo", TotalPageNo);
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}
