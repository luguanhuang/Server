#include "pch.h"
#include "npcfeelingmgr.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "npcfeeling/ptcm2g_npcflnotifym2g.h"
#include "scene/gsmanager.h"
#include "network/gslink.h"
#include "global/GlobalConfigDBBuild.h"
#include "network/dblink.h"

CNpcFeelingMgr::CNpcFeelingMgr()
{
	m_handler = 0;
	m_amendFlag = false;
}

CNpcFeelingMgr::~CNpcFeelingMgr()
{

}

bool CNpcFeelingMgr::Init()
{
	if(!ReadData())
	{
		return false;
	}
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	
	return true;
}

void CNpcFeelingMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
}

void CNpcFeelingMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if(!DBLink::Instance()->IsConnected())
	{
		return;
	}

	UINT32 now = UINT32(GameTime::GetTime());

	bool changeFlag = false;
	if(m_amendFlag)
	{
		m_amendFlag = false;
		changeFlag = true;
	}
	if(0 == m_data.lastdayuptime())
	{
		m_data.set_lastdayuptime(now);
		changeFlag = true;
	}
	if(0 == m_data.lastweekuptime())
	{
		m_data.set_lastweekuptime(now);
		changeFlag = true;
	}
	if(!GameTime::IsInSameDay(m_data.lastdayuptime(), now, true))
	{
		DayZero();
		m_data.set_lastdayuptime(now);
		changeFlag = true;
	}
	if(!GameTime::IsInSameWeek(m_data.lastweekuptime(), now, true))
	{
		WeekZero();
		m_data.set_lastweekuptime(now);
		changeFlag = true;
	}

	if(changeFlag)
	{
		WriteData();
		SyncData();
	}
}

bool CNpcFeelingMgr::ReadData()
{
	std::string tmp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eNpcFeelingGlobalData);
	if(!tmp.empty())
	{
		if(!m_data.ParseFromString(tmp))
		{
			SSError << " parse npc feeling global data failed " << END;
			return false;
		}
		AmendData();
	}
	else
	{
		SSInfo << " first time " << END;
	}
	return true;
}

void CNpcFeelingMgr::WriteData()
{
	std::string tmp;
	if(!m_data.SerializeToString(&tmp))
	{
		SSError << " npc feeling global data serialize to string failed " << END;
		return;
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eNpcFeelingGlobalData, tmp);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eNpcFeelingGlobalData);
}

//用来同步last_xxx数据
void CNpcFeelingMgr::SyncData(UINT32 gsLine/* = 0*/)
{
	PtcM2G_NpcFlNotifyM2G ptc;	
	*ptc.m_Data.mutable_globaldata() = m_data;

	if(0 != gsLine)
	{
		GSLink::Instance()->SendToLine(gsLine, ptc);
	}
	else
	{
		auto itBegin = CGsManager::Instance()->Begin();
		auto itEnd = CGsManager::Instance()->End();
		for(auto it = itBegin; it != itEnd; ++it)
		{
			GSLink::Instance()->SendToLine(it->first, ptc);
		}	
	}
}

void CNpcFeelingMgr::UpdateTopAvExp(const KKSG::NpcFlRoleExp& roleExp)
{
	if(0 == roleExp.roleid() || 0 == roleExp.exp())
	{
		return;
	}
	const auto& topCount = GetGlobalConfig().NpcFeelingTopCount;
	int index = m_data.roles_size();
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		if(m_data.roles(i).roleid() == roleExp.roleid())
		{
			index = i;
			break;
		}
	}
	bool updateFlag = true;
	if(index < m_data.roles_size())//已在队列
	{
		if(m_data.roles(index).exp() == roleExp.exp())//没有变化
		{
			updateFlag = false;
		}
		else
		{
			//移到尾部删除
			for(int i = index; i + 1 < m_data.roles_size(); ++i)
			{
				if(m_data.roles(i).roleid() == roleExp.roleid())
				{
					m_data.mutable_roles()->SwapElements(i, i+1);
				}
			}
			m_data.mutable_roles()->RemoveLast();
		}
	}
	else
	{
		if(m_data.roles_size() > 0)	
		{
			if(m_data.roles(m_data.roles_size()-1).exp() >= roleExp.exp() && m_data.roles_size() >= topCount)
			{
				updateFlag = false;
			}
		}
	}

	if(updateFlag)
	{
		*m_data.add_roles() = roleExp;

		//调整位置
		for(int i = m_data.roles_size() - 1; i - 1 >= 0; --i)
		{
			if(m_data.roles(i-1).exp() < m_data.roles(i).exp())
			{
				m_data.mutable_roles()->SwapElements(i-1, i);
			}
		}
	}
	
	bool removeFlag = (m_data.roles_size() > int(topCount));
	if(removeFlag)
	{
		while(m_data.roles_size() > int(topCount))
		{
			m_data.mutable_roles()->RemoveLast();
		}
	}

	if(updateFlag || removeFlag)
	{
		WriteData();
	}
}

void CNpcFeelingMgr::FillNpc2Role(KKSG::NpcFlRes& roRes)
{

}

void CNpcFeelingMgr::UpdateNpc2Role(const KKSG::NpcFlNpc2Role& npc2role)
{
	if(0 == npc2role.npcid() || 0 == npc2role.role().roleid() || 0 == npc2role.role().exp())
	{
		return;
	}
	for(int i = 0; i < m_data.npc2role_size(); ++i)
	{
		if(m_data.npc2role(i).npcid() == npc2role.npcid())
		{
			if(m_data.npc2role(i).role().exp() < npc2role.role().exp())
			{
				*m_data.mutable_npc2role(i) = npc2role;
				WriteData();
			}
			return;
		}
	}

	//add new
	*m_data.add_npc2role() = npc2role;
	WriteData();
}

void CNpcFeelingMgr::InfoPrint()
{
	InfoPrint(m_data);
}

void CNpcFeelingMgr::InfoPrint(const KKSG::NpcFlGlobal& data)
{
	SSInfo << " lastupdaytime = " << data.lastdayuptime() << END;
	SSInfo << " lastupweektime = " << data.lastweekuptime() << END;

	for(int i = 0; i < data.roles_size(); ++i)
	{
		const auto& tmp = data.roles(i);
		SSInfo << " cur: role = " << tmp.roleid() << " exp = " << tmp.exp() << END;
	}
	for(int i = 0; i < data.npc2role_size(); ++i)
	{
		const auto& tmp = data.npc2role(i);
		SSInfo << " cur: npc = " << tmp.npcid() << " roleid = " << tmp.role().roleid() << " rolename = " << tmp.rolename() << " exp = " << tmp.role().exp() << END;
	}

	for(int i = 0; i < data.lastday_roles_size(); ++i)
	{
		const auto& tmp = data.lastday_roles(i);
		SSInfo << " last: role = " << tmp.roleid() << " exp = " << tmp.exp() << END;
	}
	for(int i = 0; i < data.lastweek_npc2role_size(); ++i)
	{
		const auto& tmp = data.lastweek_npc2role(i);
		SSInfo << " last: npc = " << tmp.npcid() << " roleid = " << tmp.role().roleid() << " rolename = " << tmp.rolename() << " exp = " << tmp.role().exp() << END;
	}

}

void CNpcFeelingMgr::DayZero()
{
	*m_data.mutable_lastday_roles() = m_data.roles();
}

void CNpcFeelingMgr::WeekZero()
{
	*m_data.mutable_lastweek_npc2role() = m_data.npc2role();
}

void CNpcFeelingMgr::AmendData()
{
	//auto oldData = m_data;
	//修正数据 主要用于合服后
	if(AmendData(*m_data.mutable_roles())) m_amendFlag = true;
	if(AmendData(*m_data.mutable_lastday_roles())) m_amendFlag = true;
	if(AmendData(*m_data.mutable_npc2role())) m_amendFlag = true; 
	if(AmendData(*m_data.mutable_lastweek_npc2role())) m_amendFlag = true;
	if(m_amendFlag)
	{
		//SSInfo << " amend data old :" << END;
		//InfoPrint(oldData);
		SSInfo << " amend data new:" << END;
		InfoPrint(m_data);
	}
	else
	{
		SSDebug << " no amend data " << END;
	}
}

bool CNpcFeelingMgr::AmendData(::google::protobuf::RepeatedPtrField< ::KKSG::NpcFlRoleExp >& data)
{
	bool cflag = false;
	//冒泡排序
	for(int i = 0; i < data.size(); ++i)
	{
		for(int j = i + 1; j < data.size(); ++j)
		{
			if(data.Get(i).exp() < data.Get(j).exp())
			{
				data.SwapElements(i, j);
				cflag = true;
			}
		}
	}
	while(data.size() > 0 && data.size() > GetGlobalConfig().NpcFeelingTopCount)
	{
		cflag = true;
		data.RemoveLast();
	}

	return cflag;
}

bool CNpcFeelingMgr::AmendData(::google::protobuf::RepeatedPtrField< ::KKSG::NpcFlNpc2Role >& data)
{
	std::map<UINT32, KKSG::NpcFlNpc2Role> mt;
	bool cflag = false;
	for(int i = 0; i < data.size(); ++i)
	{
		const auto& npc2role = data.Get(i);
		auto it = mt.find(npc2role.npcid());
		if(it == mt.end())
		{
			mt[npc2role.npcid()] = npc2role;
		}
		else
		{
			cflag = true;
			if(npc2role.role().exp() > it->second.role().exp())
			{
				mt[npc2role.npcid()] = npc2role;
			}
		}
	}

	if(cflag)
	{
		data.Clear();
		for(auto it = mt.begin(); it != mt.end(); ++it)
		{
			*data.Add() = it->second;
		}
	}
	return cflag;
}

void CNpcFeelingMgr::Clear()
{
	m_data.Clear();
	m_data.set_lastdayuptime(GameTime::GetTime());
	m_data.set_lastweekuptime(GameTime::GetTime());

	WriteData();
	SyncData();
}

void CNpcFeelingMgr::DebugMergeData(KKSG::NpcFlGlobal& data)
{
	if(data.has_lastdayuptime())
	{
		auto& destdata = data;
		auto& srcdata = m_data;
		destdata.mutable_roles()->MergeFrom(srcdata.roles());
		destdata.mutable_lastday_roles()->MergeFrom(srcdata.lastday_roles());
		destdata.mutable_npc2role()->MergeFrom(srcdata.npc2role());
		destdata.mutable_lastweek_npc2role()->MergeFrom(srcdata.lastweek_npc2role());	
	}
	else
	{
		data = m_data;
	}
	InfoPrint(data);
}

void CNpcFeelingMgr::DebugAmendData(KKSG::NpcFlGlobal& data)
{
	m_data = data;
	InfoPrint(m_data);
	WriteData();
}

