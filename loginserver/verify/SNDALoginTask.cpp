#include "pch.h"
#include "md5.h"
#include <time.h>
#include <rapidjson/document.h>
#include "SNDALoginTask.h"
#include "crpc.h"
#include "tokenverifymgr.h"
#include "util.h"
#include "gatemgr/gatemgr.h"
#include "DBGetSelfServerTask.h"

using namespace rapidjson;

SNDALoginTask::SNDALoginTask()
{
	m_pf = "sdo";
	m_dwRpcID = 0;
}

SNDALoginTask::~SNDALoginTask()
{

}

const std::string SNDALoginTask::GetUrl()
{
	static std::string SNDAURL = "http://m.dn.api.123u.com/auth.php?";
	return SNDAURL + GetHttpParam();
}

static void CalcMD5(const std::string &str, unsigned char hexdigit[33])
{
	const char HexDigit[] = "0123456789abcdef";

	unsigned char md5[16];
	li_MD5_CTX ctx;
	li_MD5_Init(&ctx);
	li_MD5_Update(&ctx, str.data(), str.size());
	li_MD5_Final(md5, &ctx);
	for (int i = 0; i < 16; ++i)
	{
		hexdigit[i*2] = HexDigit[(md5[i] >> 4) & 0xF];
		hexdigit[i*2+1] = HexDigit[md5[i] & 0xF];
	}

	hexdigit[32] = 0;
}

static const char *PlatFormSecrtKey = "joyauth!@#$%^123456";

const std::string SNDALoginTask::GetHttpParam()
{
	char buf[32] = {0};
	UINT32 t = (UINT32)time(NULL);
	sprintf(buf, "%u", t);

	std::string temp;
	temp = m_pf;
	temp += m_token;
	temp += buf;
	temp += PlatFormSecrtKey;
	
	unsigned char hex[33];
	CalcMD5(temp, hex);

	std::stringstream ss;
	ss << "pf=" << m_pf << "&"
	   << "ticket=" << m_token << "&"
	   << "time=" << buf << "&"
	   << "sign=" << hex;
	std::string postStr = ss.str();
	LogInfo("post string = [%s]", postStr.c_str());
	return postStr;
}

void SNDALoginTask::OnResult(const std::string &response)
{
	LogDebug("get snda response result [%s]", response.c_str());

	Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		OnFailed();
	}
	else
	{
		if (!doc.HasMember("status") || doc["status"].GetInt() != 1)
		{
			OnFailed();
			return;
		}

		if (!doc.HasMember("u_info") || !doc["u_info"].HasMember("uid"))
		{
			OnFailed();
			return;
		}

		std::string phone;
		if (doc["u_info"].HasMember("phone"))
		{
			phone = doc["u_info"]["phone"].GetString(); 
		}

		char buf[64];
		UINT32 uid = doc["u_info"]["uid"].GetUint();
		DBGetSelfServerTask* pTask = new DBGetSelfServerTask(m_dwRpcID, ToString(uid, buf, 64), phone, m_ip);
		TokenVerifyMgr::Instance()->AddDBTask(pTask);
	}
}

void SNDALoginTask::OnFailed()
{
	CUserData oUserData;
	oUserData.m_dwUserData = 0;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
}

void SNDALoginTask::SetPF(const std::string &pf)
{
	if (!pf.empty())
	{
		m_pf = pf;
	}
}
