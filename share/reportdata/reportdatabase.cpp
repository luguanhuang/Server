#include "pch.h"
#include "reportdatabase.h"
#include "HashUtil.h"
#include "util.h"
#include "util/jsonutil.h"
#include "util/gametime.h"

ReportDataBaseTask::ReportDataBaseTask()
{
	SetType(true);
}

ReportDataBaseTask::~ReportDataBaseTask()
{

}

const std::string ReportDataBaseTask::GetUrl()
{
	std::ostringstream ss;
	UINT32 ts = UINT32(time(NULL));
	std::string sig = MakeSign(ts, strAppKey);
	ss << m_strUrl << "/profile/qqscore_batch/?timestamp=" << ts << 
		"&appid=" << strAppID << "&sig=" << sig << "&openid=" << strOpenId << "&encode=1";
	//SSDebug << ss.str() << END;
	strUrl = ss.str();
	return ss.str();
}

std::string ReportDataBaseTask::MakeSign(UINT32 ts, std::string key)
{
	std::ostringstream ss;
	ss << key << ts;
	return Md5Hash(ss.str());
}

void ReportDataBaseTask::BuildPostData()
{
	JsonBuilder oBuilder;
	rapidjson::Value oBodyList(rapidjson::kArrayType);
	oBuilder.AddMember_String("appid", strAppID);
	oBuilder.AddMember_String("accessToken", strToken);
	oBuilder.AddMember_String("openid", strOpenId);
	for (UINT32 i = 0; i < m_oData.size(); i ++)
	{
		rapidjson::Value oData(rapidjson::kObjectType);
		oData.AddMember("type", m_oData[i].type, oBuilder.Allocator());
		oData.AddMember("bcover", m_oData[i].bcover, oBuilder.Allocator());
		oData.AddMember("data", m_oData[i].data, oBuilder.Allocator());
		oData.AddMember("expires", m_oData[i].expires, oBuilder.Allocator());
		oBodyList.PushBack(oData, oBuilder.Allocator());
		//SSDebug << "type=" << m_oData[i].type << ",data=" << m_oData[i].data << ",bcover=" << m_oData[i].bcover << ",expires=" << m_oData[i].expires << END;
	}
	oBuilder.AddMember_Value("param", oBodyList);
	std::string str = oBuilder.GetString();
	//SSDebug << str << END;
	SetHttpPostString(str);
}

void ReportDataBaseTask::OnResult(const std::string &response)
{
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		return;
	}
	if (!doc.HasMember("ret"))
	{
		SSWarn << "has no ret in res=" << response << END;
		return;
	}
	INT32 ret = doc["ret"].GetInt();
	if (ret != 0)
	{
		SSWarn << "return failed, res=" << response << END;
		SSWarn << "reportdata url=" << strUrl << END;
		SSWarn << "post data=" << m_strPostString << END;
	}
	//SSDebug << response << END;
}

void ReportDataBaseTask::OnFailed()
{

}

void ReportDataBaseTask::AddData(INT32 iType, std::string strData, INT32 iBcover)
{
	stReportParam oData;
	oData.type = iType;
	oData.bcover = iBcover;
	oData.data = strData;
	oData.expires = "0";//ToString(time(NULL));
	m_oData.push_back(oData);
}
