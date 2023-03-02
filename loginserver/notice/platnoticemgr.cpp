#include "platnoticemgr.h"
#include "util.h"
#include "sql/mysqlmgr.h"
#include "idip/idip.h"

INSTANCE_SINGLETON(CPlatNoticeMgr)

CPlatNoticeMgr::CPlatNoticeMgr()
{

}
CPlatNoticeMgr::~CPlatNoticeMgr()
{

}
bool CPlatNoticeMgr::Init()
{
	std::vector<KKSG::PlatNotice> list;
	if (!MysqlMgr::Instance()->QueryNoticeList(list))
	{
		return false;
	}
	for (UINT32 i = 0; i < list.size(); i ++)
	{
		m_oNoticeList.insert(std::make_pair(GetKey(list[i].type(), list[i].areaid(), list[i].platid()), list[i]));
	}
	return true;
}

std::string CPlatNoticeMgr::GetKey(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID)
{
	std::stringstream ss;
	ss << uType << "#" << uAreaID << "#" << uPlatID;
	return ss.str();
}

void CPlatNoticeMgr::Uninit()
{

}

void CPlatNoticeMgr::UpdateNotice(KKSG::PlatNotice oData)
{
	std::string key = GetKey(oData.type(), oData.areaid(), oData.platid());
	auto it = m_oNoticeList.find(key);
	if (it == m_oNoticeList.end())
	{
		m_oNoticeList.insert(std::make_pair(key, oData));
	}
	else
	{
		it->second.CopyFrom(oData);
	}
	MysqlMgr::Instance()->UpdateNoticeInfo(oData);
}

KKSG::PlatNotice* CPlatNoticeMgr::GetNotice(KKSG::LoginType loginType, KKSG::PlatType platId)
{
	KKSG::GameAppType areaID;
	if (loginType == KKSG::LOGIN_QQ_PF)
	{
		areaID = KKSG::GAME_APP_QQ;	
	}
	else if (loginType == KKSG::LGOIN_WECHAT_PF)
	{
		areaID = KKSG::GAME_APP_WECHAT;
	}
	else 
	{
		areaID = KKSG::GAME_APP_WECHAT;
	}
	return GetNotice(NOTICE_TYPE_AUTHORIZE, areaID, platId);
}

KKSG::PlatNotice* CPlatNoticeMgr::GetNotice(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID)
{
	std::string key = GetKey(uType, uAreaID, uPlatID);
	auto it = m_oNoticeList.find(key);
	if (it != m_oNoticeList.end())
	{
		return &it->second;
	}
	return NULL;
}
