#include "pch.h"
#include "platnotice.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "timeutil.h"
#include "gm/ptcm2d_saveplatnoticeptf.h"
#include "network/dblink.h"
#include "idip/idip.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"

INSTANCE_SINGLETON(CPlatNoticeMgr);


CPlatNoticeMgr::CPlatNoticeMgr()
{

}

CPlatNoticeMgr::~CPlatNoticeMgr()
{

}

bool CPlatNoticeMgr::Init()
{
	if (!ReadAllNoticeFromDB())
	{
		return false;
	}
	return true;
}

void CPlatNoticeMgr::Uninit()
{

}

std::string CPlatNoticeMgr::GetKey(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID, UINT32 uID)
{
	std::stringstream ss;
	ss << uType << "#" << uAreaID << "#" << uPlatID << "#" << uID;
	return ss.str();
}

bool CPlatNoticeMgr::ReadAllNoticeFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();


	std::stringstream ss;
	ss << "select `type`,`id`,`isopen`,`areaid`,`platid`,`content`,`updatetime`,`isnew`,`title` from notice";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table notice failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		UINT32 uType = 0;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			uType = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'type' column failed");
			return false;
		}

		UINT32 uID = 0;
		if (poRes->GetFieldValue(1, &pVal, dwLen))
		{
			uID = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'id' column failed");
			return false;
		}

		bool bIsOpen = false;
		if (poRes->GetFieldValue(2, &pVal, dwLen))
		{
			bIsOpen = convert<INT32>(pVal)?false:true;
		}
		else
		{
			LogError("Read 'isopen' column failed");
			return false;
		}

		UINT32 uAreaID = 0;
		if (poRes->GetFieldValue(3, &pVal, dwLen))
		{
			uAreaID = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'areaid' column failed");
			return false;
		}

		UINT32 uPlatID = 0;
		if (poRes->GetFieldValue(4, &pVal, dwLen))
		{
			uPlatID = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'platid' column failed");
			return false;
		}

		std::string strContent;
		if (poRes->GetFieldValue(5, &pVal, dwLen))
		{
			strContent.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'content' column failed");
			return false;
		}

		UINT32 uUpdateTime = 0;
		if (poRes->GetFieldValue(6, &pVal, dwLen))
		{
			uUpdateTime = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'updatetime' column failed");
			return false;
		}

		bool bIsNew = false;
		if (poRes->GetFieldValue(7, &pVal, dwLen))
		{
			bIsNew = convert<INT32>(pVal)?true:false;
		}
		else
		{
			LogError("Read 'isnew' column failed");
			return false;
		}

		std::string strTitle;
		if (poRes->GetFieldValue(8, &pVal, dwLen))
		{
			strTitle.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'title' column failed");
			return false;
		}
		if (bIsOpen)
		{
			KKSG::PlatNotice oData;
			oData.set_type(uType);
			oData.set_noticeid(uID);
			oData.set_isopen(bIsOpen);
			oData.set_areaid(uAreaID);
			oData.set_platid(uPlatID);
			oData.set_content(strContent);
			oData.set_updatetime(uUpdateTime);
			oData.set_isnew(bIsNew);
			oData.set_title(strTitle);
			std::string key = GetKey(uType, uAreaID, uPlatID, uID);
			m_oNoticeList.insert(std::make_pair(key, oData));
		}
	}
	poRes->Release();
	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all notice data from db use time: %llu ms", qwTimeUsed);
	return true;
}
void CPlatNoticeMgr::UpdateNotice(KKSG::PlatNotice& notice)
{
	std::string key = GetKey(notice.type(), notice.areaid(), notice.platid(), notice.noticeid());
	auto it = m_oNoticeList.find(key);
	if (it == m_oNoticeList.end())
	{
		m_oNoticeList.insert(std::make_pair(key, notice));
	}
	else
	{
		it->second.CopyFrom(notice);
	}
	SaveNotice(notice);
	if (notice.isopen() && notice.type() != NOTICE_TYPE_ENTER_FACE)
	{
		HintNotifyClient(notice.platid(), notice.areaid());
	}
}

void CPlatNoticeMgr::HintNotifyClient(INT32 platid, UINT32 areaid)
{
	for (auto i = CRoleManager::Instance()->Begin(); i != CRoleManager::Instance()->End(); i ++)
	{
		CRole* role = *i;
		if (role && role->GetLoginPlatID() == platid && GetAreaID(role->GetLoginType()) == areaid)
		{
			role->HintNotify(SYS_IDIPNOTICE, false); 
		}
	}
}

KKSG::PlatNotice* CPlatNoticeMgr::FindNotice(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID, UINT32 uID)
{
	std::string key = GetKey(uType, uAreaID, uPlatID, uID);
	auto it = m_oNoticeList.find(key);
	if (it != m_oNoticeList.end())
	{
		return &it->second;
	}
	return NULL;
}

bool CPlatNoticeMgr::ClickNoticeCheck(const KKSG::PlatNotice& notice)
{
	auto it = m_oNoticeList.find(GetKey(notice.type(), notice.areaid(), notice.platid(), notice.noticeid()));
	return it != m_oNoticeList.end()?true:false;
}
void CPlatNoticeMgr::SaveNotice(const KKSG::PlatNotice& notice)
{
	PtcM2D_SavePlatNoticePtf ptc;
	ptc.m_Data.CopyFrom(notice);
	DBLink::Instance()->SendTo(ptc);
}
UINT32 CPlatNoticeMgr::GetAreaID(KKSG::LoginType loginType)
{
	if (loginType == KKSG::LOGIN_QQ_PF)
	{
		return KKSG::GAME_APP_QQ;	
	}
	if (loginType == KKSG::LGOIN_WECHAT_PF)
	{
		return  KKSG::GAME_APP_WECHAT;
	}
	if (loginType == KKSG::LOGIN_IOS_GUEST)
	{
		return GAME_GUEST;
	}
	return KKSG::GAME_APP_WECHAT;
}
void CPlatNoticeMgr::GetNotice(KKSG::LoginType loginType, KKSG::PlatType platId, std::vector<KKSG::PlatNotice>& list)
{
	UINT32 areaID = GetAreaID(loginType);
	GetNotice(areaID, platId, list);
}
void CPlatNoticeMgr::GetNotice(UINT32 uAreaID, UINT32 uPlatID, std::vector<KKSG::PlatNotice>& list)
{
	for (auto i = m_oNoticeList.begin(); i != m_oNoticeList.end(); i ++)
	{
		if (i->second.isopen() && i->second.areaid() == uAreaID && i->second.platid() == uPlatID)
		{
			list.push_back(i->second);
		}
	}
}

