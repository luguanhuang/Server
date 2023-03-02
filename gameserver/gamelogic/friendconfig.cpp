#include "pch.h"
//#include "friendconfig.h"
//#include "unit/rolemanager.h"
//#include "unit/role.h"
//#include "rolesummarymgr.h"
//#include "itemconfig.h"
//#include "item.h"
//#include "buff/XBuffTemplateManager.h"
//#include "friend/ptcg2c_frienddegreeupntf.h"
//#include "rewardmgr.h"
//#include "noticemgr.h"
//#include "friendop.h"
//#include "expeditionconfigmgr.h"
//
//INSTANCE_SINGLETON(FriendConfig);
//
//FriendConfig::FriendConfig()
//{
//
//}
//
//FriendConfig::~FriendConfig()
//{
//	
//}
//
//bool FriendConfig::Init()
//{
//	if (!m_friendtable.LoadFile("table/Friend.txt"))
//	{
//		SSWarn<<"load file table/Friend.txt failed"<<END;
//		return false;
//	}
//	if (!m_degreetable.LoadFile("table/FriendDegree.txt"))
//	{
//		SSWarn<<"load file table/FriendDegree.txt failed"<<END;
//		return false;
//	}
//	/*
//	if (!m_systable.LoadFile("table/FriendSystemConfig.txt"))
//	{
//		SSWarn<<"load file table/FriendSystemConfig.txt failed"<<END;
//		return false;
//	}
//	*/
//	return true;
//}
//
//void FriendConfig::Uninit()
//{
//
//}
//
//UINT32 FriendConfig::GetDegreeLevel(UINT32 degree)
//{
//	UINT32 level = 1;
//	for (auto i = m_friendtable.Table.begin(); i != m_friendtable.Table.end(); ++i)
//	{
//		FriendTable::RowData* data = *i;
//		if (degree >= data->totaldegree) 
//		{
//			++level;
//		}
//		else
//		{
//			break;
//		}
//	}
//	return (0 == level) ? 1 : (level - 1);
//}
//
//UINT32 FriendConfig::GetDegreeLeft(UINT32 degree)
//{
//	UINT32 total = 0;
//	for (auto i = m_friendtable.Table.begin(); i != m_friendtable.Table.end(); ++i)
//	{
//		FriendTable::RowData* data = *i;
//		if (degree < data->totaldegree) 
//		{
//			break;
//		}
//		else
//		{
//			total = data->totaldegree;
//		}
//	}
//	return degree - total;
//}
//
//float FriendConfig::GetDegree(UINT32 id)
//{
//	FriendDegreeTable::RowData* rowdata = m_degreetable.GetByid(id);
//	if (NULL == rowdata)
//	{
//		return 0.0f;
//	}
//	return rowdata->degree[0];
//}
//
//UINT32 FriendConfig::GetLimit(UINT32 id)
//{
//	FriendDegreeTable::RowData* rowdata = m_degreetable.GetByid(id);
//	if (NULL == rowdata)
//	{
//		return 0;
//	}
//	return (UINT32)rowdata->degree[1];
//}
//
//UINT32 FriendConfig::OnLevelUp(UINT64 roleid, UINT64 friendid, UINT32 all, UINT32 day)
//{
//	Role* role = RoleManager::Instance()->FindByRoleID(roleid);
//	if (NULL != role)
//	{
//		PtcG2C_FriendDegreeUpNtf ntf;
//		ntf.m_Data.set_roleid(friendid);
//		ntf.m_Data.set_friendname(RoleSummaryMgr::Instance()->FindRoleName(friendid));
//		ntf.m_Data.set_alldegree(all);
//		ntf.m_Data.set_daydegree(day);
//		role->Send(ntf);
//	}
//	return 0;
//}
//
//ItemDesc FriendConfig::GetReward(UINT32 level)
//{
//	ItemDesc item;
//	FriendTable::RowData* row = m_friendtable.GetBylevel(level);
//	if (NULL == row)
//	{
//		return item;
//	}
//	DropConf* conf = ItemConfig::Instance()->GetRandomDrop(row->dropid);
//	if (NULL == conf)
//	{
//		return item;
//	}
//	item.ItemID = conf->ItemID;
//	item.ItemCount = conf->ItemCount;
//	return item;	
//}
//
//BuffDesc FriendConfig::GetBuff(UINT32 degree)
//{
//	BuffDesc buff;
//	UINT32 level = GetDegreeLevel(degree);
//	FriendTable::RowData* rowdata = m_friendtable.GetBylevel(level);
//	if (NULL == rowdata)
//	{
//		return buff;
//	}
//	buff.buffID = rowdata->buf[0];
//	buff.BuffLevel = rowdata->buf[1];
//	return buff;
//}
//
//UINT32 FriendConfig::GetNotice(UINT32 level)
//{
//	FriendTable::RowData* rowdata = m_friendtable.GetBylevel(level);
//	if (NULL == rowdata)
//	{
//		return 0;
//	}
//	return rowdata->noticeid;
//}
//
//
//void FriendConfig::RewardDegree(UINT32 level, UINT64 idfirst, UINT64 idsecond)
//{
//	if (level <= 0)
//	{
//		return;
//	}
//	ItemDesc item = GetReward(level);
//	if (0 != item.ItemID && 0 != item.ItemCount)
//	{
//		std::vector<ItemDesc> vecItem;
//		vecItem.push_back(item);
//
//		std::vector<std::string> vecParam1;
//		vecParam1.push_back(RoleSummaryMgr::Instance()->FindRoleName(idsecond));
//		vecParam1.push_back(ToString(level));
//		CRewardMgr::Instance()->PushOnceReward(idfirst, KKSG::RewardDegree, vecParam1, vecItem);
//
//		std::vector<std::string> vecParam2;
//		vecParam2.push_back(RoleSummaryMgr::Instance()->FindRoleName(idfirst));
//		vecParam2.push_back(ToString(level));
//		CRewardMgr::Instance()->PushOnceReward(idsecond, KKSG::RewardDegree, vecParam2, vecItem);
//	}
//}	
//
//void FriendConfig::NoticeDegree(UINT32 level, UINT64 idfirst, UINT64 idsecond)
//{
//	if (level <= 0)	
//	{
//		return;
//	}
//	UINT32 noticeid = GetNotice(level);
//	if (0 == noticeid)
//	{
//		return;
//	}
//	Role* role1 = RoleManager::Instance()->FindByRoleID(idfirst);
//	RoleSummary* role2 = RoleSummaryMgr::Instance()->FindRoleSummary(idsecond);
//	if (NULL == role1 || NULL == role2)
//	{
//		return;
//	}
//	NoticeMgr::Instance()->FriendNotice(role1, role2, level, noticeid);
//}
//
//UINT32 FriendConfig::SceneToType(UINT32 scenetype, UINT32 sceneid)
//{
//	switch (scenetype)
//	{
//	case (KKSG::SCENE_ABYSSS):
//		{
//			return DegreeElite;
//		}
//	case (KKSG::SCENE_NEST):
//		{
//			return ExpeditionConfigMgr::Instance()->IsDragonScene(sceneid) ? DegreeDragon : DegreeNest;
//		}
//	case (KKSG::SCENE_TOWER):
//		{
//			return DegreeTower;
//		}
//	case (KKSG::SCENE_SMALLMONSTER):
//		{
//			return DegreeSmallMonster;
//		}
//	case (KKSG::SCENE_PVP):
//		{
//			return DegreePVP;
//		}
//	case (KKSG::SCENE_GODDESS):
//		{
//			return DegreeGoddess;
//		}
//	default:
//		break;
//	}
//	return (UINT32)(-1);
//}
