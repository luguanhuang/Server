#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "notice/noticemgr.h"

static bool GetTime(std::string strTime, time_t& tTime)
{
	if (strTime.length() != 12)
	{
		return false;
	}
	struct tm stTime;
	memset(&stTime, 0, sizeof(stTime));
	stTime.tm_isdst = -1;
	stTime.tm_year = convert<INT32>(strTime.substr(0,4)) - 1900;
	stTime.tm_mon = convert<INT32>(strTime.substr(4,2)) - 1;
	stTime.tm_mday = convert<INT32>(strTime.substr(6,2));
	stTime.tm_hour = convert<INT32>(strTime.substr(8,2));
	stTime.tm_min = convert<INT32>(strTime.substr(10,2));
	tTime = mktime(&stTime);
	if (tTime <= 0)
	{
		return false;
	}
	return true;
}

INT32 CMSIdipSendLampNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT32 uID = JsonUtil::GetJsonInt(roBodyJson, "RollingId");
	std::string strContent = UrlDecode(JsonUtil::GetJsonString(roBodyJson, "RollingContent"));
	std::string strBeginTime = JsonUtil::GetJsonString(roBodyJson, "RollingBeginTime");
	std::string strEndtime = JsonUtil::GetJsonString(roBodyJson, "RollingEndTime");
	INT32 nSpanTime = JsonUtil::GetJsonInt(roBodyJson, "Interval");
	time_t begin = 0,end = 0;
	if (!GetTime(strBeginTime, begin) || !GetTime(strEndtime, end))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_TIME;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	tm* begintime = localtime(&begin);
	tm b = *begintime;
	tm* endtime = localtime(&end);
	stNoticeContent notice(uID, strContent, KKSG::LampShortChannel, 0, begin, end);
	NoticeTime noticetime(b, *endtime, nSpanTime);
	if (!NoticeMgr::Instance()->AddIdipNotice(uID, notice, noticetime))
	{
		roReply.m_nErrCode = IDIP_ERR_SAME_LAMPID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode).c_str());
	oBuilder.AddMember_Int("RollingId", uID);
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}
