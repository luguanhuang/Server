#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/MultiActivityList.h"
#include "table/MultActivityMgr.h"
#include "util/XCommon.h"

static std::string GetGuildTaskTime(UINT32 uStartTime, UINT32 uEndTime)
{
	char pBuf[16] = {0};
	UINT32 todaybegin = (UINT32)XCommon::GetTodayBeginTime(false);
	if (uStartTime >= todaybegin && uStartTime < uEndTime)
	{
		uStartTime -= todaybegin;
		uEndTime -= todaybegin;
		sprintf(pBuf, "%02d:%02d-%02d:%02d", uStartTime/3600, uStartTime%3600/60, uEndTime/3600, uEndTime%3600/60);
	}
	return pBuf;
}
INT32 CMSIdipQueryGuildTaskHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 GuildId = JsonUtil::GetJsonUint64(roBodyJson, "GuildId");
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(GuildId);
	if (pGuild == NULL)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	std::vector<MultiActivityList::RowData*> vData;
	MultActivityMgr::Instance()->GetGuildTask(vData);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode).c_str());
	rapidjson::Value oTaskList(rapidjson::kArrayType);
	INT32 Count = 0;
	for (UINT32 i = 0; i < vData.size(); i ++)
	{
		if (vData[i] != NULL)
		{
			std::string strPrefix;
			if (vData[i]->ID == MULT_ACTIVITY_1)
			{
				std::vector<std::string> vstr;
				vstr = Split(vData[i]->OpenDayTips, "{n}");
				if (vstr.size())
				{
					strPrefix = vstr[0];
				}
			}
			else
			{
				strPrefix = vData[i]->OpenDayTips;
			}
			std::string strSuffix = GetGuildTaskTime(MultActivityMgr::Instance()->GetOpenTime(vData[i]->ID), MultActivityMgr::Instance()->GetEndTime(vData[i]->ID));
			rapidjson::Value oTask(rapidjson::kObjectType);
			std::ostringstream ss;
			ss << vData[i]->Name << ": " << strPrefix << strSuffix;
			oTask.AddMember("Task", UrlEncode(ss.str()), oBuilder.Allocator());
			oTaskList.PushBack(oTask, oBuilder.Allocator());
			Count ++;
		}
	}
	std::string strGuildCampTaskDesc = GetGlobalConfig().GuildCampTaskDesc;//公会大冒险比较特殊不属于多人活动，但是公会任务要求返回,直接配置在globalconfig中;
	if (!strGuildCampTaskDesc.empty())
	{
		rapidjson::Value oTask(rapidjson::kObjectType);
		oTask.AddMember("Task", UrlEncode(strGuildCampTaskDesc), oBuilder.Allocator());
		oTaskList.PushBack(oTask, oBuilder.Allocator());
		Count ++;
	}
	oBuilder.AddMember_Int("TaskList_count", Count);
	oBuilder.AddMember_Value("TaskList", oTaskList);
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}
