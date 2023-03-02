#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "notice/noticemgr.h"

INT32 CMSIdipQueryLampNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	std::vector<std::pair<NoticeTime, stNoticeContent>> list;
	NoticeMgr::Instance()->GetIdipNotice(list);
	JsonBuilder oBuilder;
	rapidjson::Value oList(rapidjson::kArrayType);
	for (UINT32 i = 0; i < list.size(); i ++)
	{
		rapidjson::Value item(rapidjson::kObjectType);
		item.AddMember("RollingId", list[i].second.NoticeId, oBuilder.Allocator());
		item.AddMember("RollingContent", UrlEncode(list[i].second.szContent), oBuilder.Allocator());
		item.AddMember("RollingBeginTime", list[i].second.uStartTime, oBuilder.Allocator());
		item.AddMember("RollingEndTime", list[i].second.uEndTime, oBuilder.Allocator());
		item.AddMember("Interval", list[i].first.timespan, oBuilder.Allocator());
		oList.PushBack(item, oBuilder.Allocator());
	}
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode).c_str());
	oBuilder.AddMember_Uint("Rolling_count", list.size());
	oBuilder.AddMember_Value("Rolling", oList);
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}
