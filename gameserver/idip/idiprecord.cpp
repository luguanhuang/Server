#include "pch.h"
#include "idiprecord.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include <time.h>
#include "common/ptcg2c_idippunishinfontf.h"
#include "common/ptcg2c_notifyidipmessagegs.h"
#include "idip/idip.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "gamelogic/antiaddictionmgr.h"

CIdipRecord::CIdipRecord(Role* pRole) : m_pRole(pRole) ,m_XinYueHint(false), m_AdultType(AdultType1)
{
}

CIdipRecord::~CIdipRecord()
{

}

bool CIdipRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_idiprecord())
	{
		const KKSG::IdipData& data = poRoleAllInfo->idiprecord();
		m_szMessage = data.mess().message();

		for (INT32 i = 0; i < data.punishinfo_size(); i ++)
		{
			m_vPunishInfo.insert(std::make_pair(data.punishinfo(i).type(), data.punishinfo(i)));
		}

		for (INT32 i = 0 ; i < data.notice_size(); i ++)
		{
			m_vNoticeInfo.push_back(data.notice(i));
		}
		m_XinYueHint = data.xinyue_hint();

		for (INT32 i = 0; i < data.hintdata_size(); i ++)
		{
			m_HintInfo[data.hintdata(i).systemid()] = data.hintdata(i).ishint();
		}

		m_AdultType = data.has_adulttype()?data.adulttype():AdultType1;
	}
	return true;
}

void CIdipRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModify.TestAndReset())
	{
		KKSG::IdipData* pData = poRoleAllInfo->mutable_idiprecord();
		pData->clear_punishinfo();
		pData->clear_mess();
		pData->clear_notice();
		//注意有2个数据不写mysql，不能用pData->Clear();
		KKSG::IdipMessage* pMess = pData->mutable_mess();
		pMess->set_message(m_szMessage);
	
		for (auto i = m_vPunishInfo.begin(); i != m_vPunishInfo.end(); i ++)
		{
			KKSG::IdipPunishData* pPunishData = pData->add_punishinfo();
			pPunishData->CopyFrom(i->second);
		}

		for (UINT32 i = 0; i < m_vNoticeInfo.size(); i ++)
		{
			KKSG::PlatNotice* pNotice = pData->add_notice();
			pNotice->CopyFrom(m_vNoticeInfo[i]);
		}
		pData->set_xinyue_hint(m_XinYueHint);


		for (auto i = m_HintInfo.begin(); i != m_HintInfo.end(); i ++)
		{
			KKSG::IdipHintData* pHintData = pData->add_hintdata();
			pHintData->set_systemid(i->first);
			pHintData->set_ishint(i->second);
		}

		pData->set_adulttype(m_AdultType);
		roChanged.insert(pData);
	}
}
 
void CIdipRecord::SetMessage(const std::string& strMessage)
{
	m_szMessage = strMessage;
	m_isModify.Set();
}

const KKSG::IdipPunishData* CIdipRecord::GetIdipPunishData(INT32 iType)
{
	auto it = m_vPunishInfo.find(iType);
	if (it != m_vPunishInfo.end())
	{
		if ((it->second.punishtime() + it->second.bantime()) > (INT32)time(NULL))
		{
			return &(it->second);
		}
		m_vPunishInfo.erase(it);
		m_isModify.Set();
	}
	return NULL;
}

void CIdipRecord::SetIdipPunishData(KKSG::IdipPunishData& oData)
{
	auto it = m_vPunishInfo.find(oData.type());
	if (it == m_vPunishInfo.end())
	{
		m_vPunishInfo.insert(std::make_pair(oData.type(), oData));
	}
	else
	{
		it->second.CopyFrom(oData);
	}
	NotifyIdipPunish(oData);
	m_isModify.Set();
}

void CIdipRecord::UnDoIdipPunishData(INT32 iType)
{
	auto it = m_vPunishInfo.find(iType);
	if (it != m_vPunishInfo.end())
	{

		KKSG::IdipPunishData oData;
		oData.CopyFrom(it->second);
		oData.set_bantime(0);
		NotifyIdipPunish(oData);

		m_vPunishInfo.erase(it);
		m_isModify.Set();
	}
}

bool CIdipRecord::CheckZeroProFit()
{
	const KKSG::IdipPunishData* pData = GetIdipPunishData(KKSG::PUNISH_USER_ZERO_PROFIT);
	if (NULL == pData)
	{
		return false;
	}
	NotifyIdipPunish(*pData);
	return true;
}
void CIdipRecord::NotifyIdipPunish(KKSG::IdipPunishData oData)
{
	PtcG2C_IdipPunishInfoNtf ptc;
	ptc.m_Data.set_type(oData.type());
	ptc.m_Data.set_bantime(oData.bantime());
	ptc.m_Data.set_endtime(oData.punishtime() + oData.bantime());
	int now = (int)time(NULL);
	int lefttime = (ptc.m_Data.endtime() > now)? ptc.m_Data.endtime() - now: 0;
	ptc.m_Data.set_lefttime(lefttime);
	m_pRole->Send(ptc);
}
void CIdipRecord::NotifyMessage()
{
	if (m_szMessage.empty())
	{
		return;
	}
	PtcG2C_NotifyIdipMessageGs ptc;
	ptc.m_Data.set_message(m_szMessage);
	m_pRole->Send(ptc);

	m_szMessage = "";
	m_isModify.Set();
}
bool CIdipRecord::CheckIdipAndNotify(INT32 iType, bool notify/* = true*/)
{
	auto pIdip = GetIdipPunishData(iType);
	if(pIdip)
	{
		if(notify)
		{
			NotifyIdipPunish(*pIdip);
		}
		SSDebug << __FUNCTION__ << " rolename = " << m_pRole->GetName() << " type = " << iType << END;
		return true;
	}
	return false;
}
void CIdipRecord::ClickNotice(KKSG::PlatNotice notice)
{
	auto it = std::find_if(m_vNoticeInfo.begin(), m_vNoticeInfo.end(), EqualNotice(notice.type(), notice.noticeid(), notice.areaid(), notice.platid(), notice.updatetime()));
	if (it == m_vNoticeInfo.end())
	{
		m_vNoticeInfo.push_back(notice);
	}
	else
	{
		it->CopyFrom(notice);
	}
	m_isModify.Set();
}
void CIdipRecord::Update()
{
	if (!XCommon::IsDaySame(time_t(m_pRole->GetLogoutTime()), GameTime::GetTime()))
	{
		for (auto j = m_vNoticeInfo.begin(); j != m_vNoticeInfo.end(); )
		{
			if (j->type() == NOTICE_TYPE_ENTER_FACE)
			{
				j = m_vNoticeInfo.erase(j);
				m_isModify.Set();
			}
			else
			{
				j ++;
			}
		}
		m_isModify.Set();
	}
}
void CIdipRecord::DoNotice(const KKSG::CheckNoticeArg &roArg, KKSG::CheckNoticeRes &roRes)
{
	Update();
	//删除不存在的公告;
	for (auto j = m_vNoticeInfo.begin(); j != m_vNoticeInfo.end(); )
	{
		bool isFind = false;
		for (INT32 i = 0; i < roArg.notice_size(); i ++)
		{
			if (roArg.notice(i).type() == j->type() && roArg.notice(i).areaid() == j->areaid() && 
				roArg.notice(i).platid() == j->platid() && roArg.notice(i).noticeid() == j->noticeid() && roArg.notice(i).updatetime() == j->updatetime())
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			j = m_vNoticeInfo.erase(j);
			m_isModify.Set();
		}
		else
		{
			j ++;
		}
	}

	//验证哪些公告读取过;拍脸图的读取由客户端主动发起;
	std::vector<KKSG::PlatNotice> vNotice;
	for (INT32 i = 0; i < roArg.notice_size(); i ++)
	{
		auto it = std::find_if(m_vNoticeInfo.begin(), m_vNoticeInfo.end(), EqualNotice(roArg.notice(i).type(), roArg.notice(i).noticeid(), roArg.notice(i).areaid(), roArg.notice(i).platid(), roArg.notice(i).updatetime()));
		KKSG::PlatNotice roInfo;
		roInfo.CopyFrom(roArg.notice(i));
		if (it == m_vNoticeInfo.end())
		{
			roInfo.set_isnew(true);
		}
		else
		{
			roInfo.set_isnew(false);
		}
		vNotice.push_back(roInfo);
	}
	std::sort(vNotice.begin(), vNotice.end(), CompareRule);
	for (UINT32 i = 0; i < vNotice.size(); i ++)
	{
		KKSG::PlatNotice* pInfo = roRes.add_notice();
		pInfo->CopyFrom(vNotice[i]);
	}
}
bool CIdipRecord::CompareRule(const KKSG::PlatNotice& fir, const KKSG::PlatNotice& sec)
{
	if (fir.isnew() == sec.isnew())
	{
		return fir.updatetime() > sec.updatetime();
	}
	return fir.isnew() > sec.isnew();
}

void CIdipRecord::SetXinYueHint(bool hint)
{
	if (m_XinYueHint == hint)
	{
		return;
	}
	m_XinYueHint = hint;
	m_isModify.Set();
}

void CIdipRecord::SetHint(UINT32 sysytemid, bool hint)
{
	auto it = m_HintInfo.find(sysytemid);
	if (it == m_HintInfo.end())
	{
		m_HintInfo[sysytemid] = hint;
	}
	else if (it->second == hint)
	{
		return;
	}
	else
	{
		it->second = hint;
	}
	CheckHint(sysytemid);
	m_isModify.Set();
}

void CIdipRecord::CheckHint(UINT32 systemid)
{
	auto it = m_HintInfo.find(systemid);
	if (it == m_HintInfo.end())
	{
		return;
	}
	if (it->second)
	{
		m_pRole->HintNotify(systemid, false);
	}
	else
	{
		m_pRole->HintNotify(systemid, true);
	}
}


void CIdipRecord::CheckAllHint()
{
	for (auto it = m_HintInfo.begin(); it != m_HintInfo.end(); it ++)
	{
		CheckHint(it->first);
	}
}

void CIdipRecord::SetAdultType(INT32 adulttype)
{
	m_AdultType = adulttype;
	m_isModify.Set();
}
