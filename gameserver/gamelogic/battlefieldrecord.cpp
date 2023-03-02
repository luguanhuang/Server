#include "pch.h"
#include "battlefieldrecord.h"
#include "pb/project.pb.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "designationMgr.h"
#include "helldoordropmgr.h"
#include "battlefieldgsmgr.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "define/tlogenum.h"
#include "scene/scene.h"
#include "scene/battlefieldfight.h"


CBattleFieldRecord::CBattleFieldRecord(Role *pRole) : m_pRole(pRole)
{
	m_LastUpdateWeekTime = 0;
	m_FirstRankCount = 0;
	m_TotalKillCount = 0;
	m_LastUpdateDayTime = 0;
	m_mapPoint.clear();
}

CBattleFieldRecord::~CBattleFieldRecord()
{

}

bool CBattleFieldRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_battlefield())
	{
		const KKSG::BattleFieldData& field = poRoleAllInfo->battlefield();
		m_LastUpdateWeekTime	= field.lastupdatetime();
		m_LastUpdateDayTime		= field.dayupdatetime();
		m_FirstRankCount		= field.firstrankcount();
		m_TotalKillCount		= field.totalkillcount();
		
		for (INT32 i = 0; i < field.point_size(); i ++)
		{
			m_mapPoint[field.point(i).id()] = field.point(i).count();
		}

		for (INT32 i = 0; i < field.hell().all_size(); i++)
		{
			const KKSG::HellDropInfo &hellinfo = field.hell().all(i);
			KKSG::HellDropInfo info;
			info.CopyFrom(hellinfo);
			m_mapDropInfo[info.id()] = info;
			LogDebug("roleid=%llu rolename=%s count=%u hellid=%u worse=%u worst=%u ",m_pRole->GetID(),m_pRole->GetName().c_str(),info.count(),info.id(),info.worse(),info.worst());
		}
		OnDayPass();
	}else
	{
		ResetDropInfo();
		time_t curTime			= GameTime::GetTime();
		m_LastUpdateDayTime		= GameTime::GetDayBeginTime(curTime);
		m_LastUpdateWeekTime    = GameTime::GetWeekBeginTime(curTime);
		m_isModified.Set();
	}
	return true;
}



void CBattleFieldRecord::ResetDropInfo()
{
	m_mapDropInfo.clear();
	HellDoorDropTable *pTable =  HellDoorDropMgr::Instance()->GetTable();
	for (size_t i = 0; i < pTable->Table.size(); i++)
	{
		HellDoorDropTable::RowData *pRowData = pTable->Table[i];
		if (pRowData)
		{
			KKSG::HellDropInfo info;
			info.set_id(pRowData->HellDoorDropID);
			info.set_worse(XRandom::randInt(pRowData->EnterWorseDropTimes[0],1+pRowData->EnterWorseDropTimes[1]));
			info.set_worst(XRandom::randInt(pRowData->EnterWorstDropTimes[0],1+pRowData->EnterWorstDropTimes[1]));
			m_mapDropInfo.insert(std::pair<UINT32, KKSG::HellDropInfo>(pRowData->HellDoorDropID,info));
		}
	}
}

void CBattleFieldRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModified.TestAndReset())
	{
		KKSG::BattleFieldData* pField = poRoleAllInfo->mutable_battlefield();
		pField->Clear();
		pField->set_firstrankcount(m_FirstRankCount);
		pField->set_totalkillcount(m_TotalKillCount);
		pField->set_lastupdatetime(m_LastUpdateWeekTime);
		pField->set_dayupdatetime(m_LastUpdateDayTime);
		for (auto it = m_mapPoint.begin(); it != m_mapPoint.end(); it ++)
		{
			KKSG::BattleFieldPoint* pPoint =  pField->add_point();
			pPoint->set_id(it->first);
			pPoint->set_count(it->second);
		}
		KKSG::HellDropInfoAll *all  = pField->mutable_hell();
		for (auto iter = m_mapDropInfo.begin(); iter!=m_mapDropInfo.end(); iter++)
		{
			KKSG::HellDropInfo *pDrop = all->add_all();
			pDrop->CopyFrom(iter->second);	
		}
		roChanged.insert(pField);
	}
}


void CBattleFieldRecord::OnDayPass()
{
	time_t nowtime = GameTime::GetTime();
	LogDebug("OnDayPass ---begintime=%u endtime=%u",(UINT32)GameTime::GetWeekBeginTime(nowtime), m_LastUpdateWeekTime);
	if (GameTime::GetWeekBeginTime(nowtime)!=m_LastUpdateWeekTime)
	{
		m_mapPoint.clear();
		m_LastUpdateWeekTime = GameTime::GetWeekBeginTime(nowtime);
		m_isModified.Set();
	}
	if (GameTime::GetDayBeginTime(nowtime)!=m_LastUpdateDayTime)
	{
		m_mapDropInfo.clear();
		m_LastUpdateDayTime = GameTime::GetDayBeginTime(nowtime);
		ResetDropInfo();
		m_isModified.Set();
	}
}


void CBattleFieldRecord::AddFirstRankCount()
{
	m_FirstRankCount++;
	DesignationMgr::Instance()->OneBigerCondition(m_pRole, DESIGNATION_COM_TYPE_BATTLEFIELD_RANK, m_FirstRankCount,GameTime::GetTime());
	m_isModified.Set();
}

void CBattleFieldRecord::AddTotalKillCount()
{
	m_TotalKillCount++;
	DesignationMgr::Instance()->OneBigerCondition(m_pRole, DESIGNATION_COM_TYPE_BATTLEFIELD_KILL, m_TotalKillCount,GameTime::GetTime());
	m_isModified.Set();
}

UINT32 CBattleFieldRecord::GetPointIdCount(UINT32 id)
{
	auto it = m_mapPoint.find(id);
	if (it == m_mapPoint.end())
	{
		return 0;
	}
	return it->second;
}

void CBattleFieldRecord::AddPointIdCount(UINT32 id)
{
	auto it = m_mapPoint.find(id);
	if (it == m_mapPoint.end())
	{
		m_mapPoint[id] = 1;
	}
	else
	{
		it->second++;
	}
	m_isModified.Set();
}

KKSG::HellDropInfo* CBattleFieldRecord::GetDropInfo(UINT32 nId)
{
	return &m_mapDropInfo[nId];
}

void CBattleFieldRecord::SetDropInfo(UINT32 nId,UINT32 nCount)
{
	m_mapDropInfo[nId].set_count(nCount);
	m_mapDropInfo[nId].set_id(nId);
	LogInfo("set drop count id=%u count=%u",nId,nCount);
	m_isModified.Set();
}

void CBattleFieldRecord::GivePointAward(UINT32 nPoint)
{
	BattleFieldPointReward *pTable =  BattleFieldGSMgr::Instance()->GetTable();
	if (pTable==NULL||pTable->Table.empty())
		return;
	std::vector<ItemDesc> itemList;
	for (size_t i = 0; i < pTable->Table.size(); i++)
	{
		BattleFieldPointReward::RowData*  pRowData = pTable->Table[i];
		if (pRowData)
		{
			if ((m_pRole->GetLevel()>= pRowData->levelrange[0]&& m_pRole->GetLevel()<= pRowData->levelrange[1])&&(nPoint>=pRowData->pointseg[0]&& nPoint<=pRowData->pointseg[1]))
			{
				if (m_mapPoint[pRowData->id] < pRowData->count && m_mapGotAward[pRowData->id] == 0)
				{
					for (size_t j = 0; j < pRowData->reward.size(); j++)
					{
						ItemDesc item(pRowData->reward[j][0],pRowData->reward[j][1]);
						itemList.push_back(item);
						LogDebug("GivePointAward roleid=%llu name=%s id=%u count=%u", m_pRole->GetID(),m_pRole->GetName().c_str(),pRowData->id,m_mapPoint[pRowData->id]);
					}
					m_mapPoint[pRowData->id]++;
					m_mapGotAward[pRowData->id] = 1;
					m_isModified.Set();
				}
			}
		}
	}

	if (!itemList.empty())
	{
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_BattleField);
		if (conf)
		{
			ostringstream stream;
			stream<<nPoint;
			std::string szContent = MailConfig::Instance()->ReplaceString("0", stream.str(), conf->m_content);
			MailSender sender;
			sender.SetContent(Mail_System, conf->m_title,  szContent);
			sender.SetReason(ItemFlow_BattleFieldWeekPoint);
			sender.SetItems(itemList);
			sender.Send(m_pRole->GetID());
		}
	}

}


void CBattleFieldRecord::FillBattleReward(KKSG::BattleFieldAwardNumRes &res)
{
	for (auto iter = m_mapPoint.begin(); iter != m_mapPoint.end(); iter++)
	{
		KKSG::BattleFieldLeftAward *pReward = res.add_award();
		pReward->set_id(iter->first);
		pReward->set_count(iter->second);
	}
}

void CBattleFieldRecord::ClearBattleReward()
{
	m_mapGotAward.clear();
}