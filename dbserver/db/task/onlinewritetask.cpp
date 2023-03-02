#include "pch.h"
#include "onlinewritetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "config.h"
#include "appid.h"

COnlineWriteTask::COnlineWriteTask()
:CMysqlTask("OnlineWriteTask")
,m_uTimeKey(0)
,m_uServerID(0)
,m_uZoneID(0)
,m_uIosNum(0)
,m_uAndroidNum(0)
,m_uAppID(0)
{
}

COnlineWriteTask::~COnlineWriteTask()
{

}

void COnlineWriteTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	const char* pszAppID = (m_uAppID == KKSG::GAME_APP_QQ ? MobileQQ_App_ID : WeChat_App_ID);
	std::stringstream ss;
	ss << "insert into tb_dragonnest_onlinecnt(`gameappid`,`timekey`,`gsid`,`zoneareaid`,`onlinecntios`,`onlinecntandroid`) values('";
	ss << pszAppID << "'," << m_uTimeKey << "," << m_uServerID << "," << m_uZoneID << "," << m_uIosNum << "," << m_uAndroidNum << ")";
	//LogInfo("Online Write %s", ss.str().c_str());//写日志，以防出问题后还可以根据日志手动补回去
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void COnlineWriteTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Online write task failed, errMsg: %s", GetErrStream().str().c_str());
	}
}
