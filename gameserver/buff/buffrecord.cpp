#include "pch.h"
#include "buffrecord.h"
#include "time.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "entity/XRole.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/MultActivityMgr.h"
#include "battle/ptcg2c_doodaditemaddntf.h"
#include "battle/ptcg2c_doodaditemusentf.h"
#include "horse/rpcc2g_horsereconnect.h"
#include "unit/roleoutlook.h"
#include "transform/ptcg2c_transnotify.h"
#include "chat/ptcg2m_privatechat.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "buff/XBuffTemplateManager.h"
#include "network/mslink.h"
#include "gamelogic/notice.h"
#include "table/doodaditemskillconfig.h"
#include "battle/ptcg2c_doodaditemskillsntf.h"
#include "util/gametime.h"

#define DOODADITEM_MAX 2

BuffRecord::BuffRecord(Role* role)
:m_role(role)
{

}

BuffRecord::~BuffRecord()
{

}

bool BuffRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_buffrecord())
	{
		const KKSG::SBuffRecord& record = poRoleAllInfo->buffrecord();
		for (int i = 0; i < record.buffs_size(); ++i)
		{
			BuffDesc buff;
			buff.buffID = record.buffs(i).buffid();
			buff.BuffLevel = record.buffs(i).bufflevel();
			buff.effecttime = record.buffs(i).effecttime();
			
			UINT32 type = ItemConfig::Instance()->GetBuffType(buff.buffID);
			m_buffs[type][buff.buffID] = buff;
		}
		for (int j = 0; j < record.items_size(); ++j)
		{
			SBuffItem item;
			item.itemid = record.items(j).itemid();
			item.itemcount = record.items(j).itemcount();
			item.timeexpire = record.items(j).expiretime();

			m_buffitems[item.itemid] = item;
		}

		// trans buff
		m_transformbuff.ToThis(&record.transbuff());
		// small buff
		m_smallbuff.ToThis(&record.smallbuff());
	}
	return true;
}

void BuffRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ischanged.TestAndReset())
	{
		KKSG::SBuffRecord* record = poRoleAllInfo->mutable_buffrecord();
		record->Clear();
		for (auto i = m_buffs.begin(); i != m_buffs.end(); ++i)
		{
			for (auto j = i->second.begin(); j != i->second.end(); ++j)
			{
				KKSG::Buff* buff = record->add_buffs();
				buff->set_buffid(j->second.buffID);
				buff->set_bufflevel(j->second.BuffLevel);
				buff->set_effecttime(j->second.effecttime);
			}
		}
		for (auto j = m_buffitems.begin(); j != m_buffitems.end(); ++j)
		{
			KKSG::BuffItem* item = record->add_items();
			item->set_itemid(j->second.itemid);
			item->set_itemcount(j->second.itemcount);
			item->set_expiretime(j->second.timeexpire);
		}

		// trans buff
		KKSG::STransformBuff* transbuff = record->mutable_transbuff();
		m_transformbuff.ToKKSG(transbuff);
		// small buff
		KKSG::STransformBuff* smallbuff = record->mutable_smallbuff();
		m_smallbuff.ToKKSG(smallbuff);

		roChanged.insert(record);
	}
}

void BuffRecord::Update()
{
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	for (auto i = m_buffs.begin(); i != m_buffs.end(); ++i)
	{
		for (auto j = i->second.begin(); j != i->second.end();)
		{
			if (nowtime > j->second.effecttime)
			{
				i->second.erase(j++);
				m_ischanged.Set();
			}
			else
			{
				++j;
			}
		}
	}

	for (auto i = m_buffitems.begin(); i != m_buffitems.end();)
	{
		if (nowtime > i->second.timeexpire)
		{
			m_buffitems.erase(i++);
			m_ischanged.Set();
		}
		else
		{
			++i;
		}
	}
}

void BuffRecord::AddBuff(UINT32 type, UINT32 id, UINT32 level)
{
	BuffTable::RowData* data = XBuffTemplateManager::Instance()->GetBuffInfo(id, level, NULL);
	if (NULL == data)
	{
		SSError<<"no find buff, id:"<<id<<" level:"<<level<<END;
		return;
	}
	BuffDesc buff;
	buff.buffID = id;
	buff.BuffLevel = level;
	if (data->BuffDuration != -1.0f)
	{
		UINT32 nowtime = (UINT32)GameTime::GetTime();
		buff.effecttime = nowtime + (int)(data->BuffDuration);
	}
	m_buffs[type][id] = buff;
	m_ischanged.Set();
}

void BuffRecord::DoAddBuffs(Scene* scene)
{
	if (SceneConfig::Instance()->IsBattleSceneExcept(scene->GetSceneType()))
	{
		return;
	}
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	XAddBuffActionArgs args;
	for(auto i = m_buffs.begin(); i != m_buffs.end(); ++i)
	{
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			BuffDesc buff = j->second;
			if (buff.effecttime > nowtime)
			{
				buff.effecttime = buff.effecttime - nowtime;
				args.vecBuffs.push_back(buff);
				m_role->AddReviveBuff(j->second);
			}
		}
	}
	args.Caster = m_role->GetID();
	args.bClientOnly = !scene->IsSyncScene();
	m_role->GetXObject()->DispatchAction(&args);
}

void BuffRecord::OnLeaveScene(Scene* scene)
{
	if (NULL != scene && NULL != scene->GetSceneTrans())
	{
		if (scene->GetSceneTrans()->OnLeave(m_role))
		{
			m_transformbuff.Reset();
		}
	}
	if (NULL != scene && KKSG::SCENE_LEISURE == scene->GetSceneType())
	{
		m_transformbuff.Reset();
		m_smallbuff.Reset();
	}
	if (KKSG::OutLook_Trans == m_role->GetState().statetype())
	{
		m_role->SetState(KKSG::OutLook_Normal);
	}
}

void BuffRecord::OnEnterScene(Scene* scene)
{
	Update();
//	UpdateTransBuff();

	m_doodads.clear();
	UINT32 size = CDoodadItemSkillConfig::Instance()->GetAllSkillCount(scene->GetSceneType());
	if(size > 0)
	{
		m_doodads.resize(size, DoodadItem());
	}

	UINT32 ret = CanTrans();
	if (KKSG::ERR_SUCCESS == ret)
	{
//		DoAddTransBuff(false);
		RecoverTransBuff(true);
	}
	if (NULL != scene && NULL != scene->GetSceneTrans())
	{
		scene->GetSceneTrans()->OnEnter(m_role);
	}
	DoAddBuffs(scene);
}

void BuffRecord::AddBuffItem(UINT32 itemid)
{
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(itemid);
	if (NULL == conf)
	{
		SSWarn<<"invalid itemid:"<<itemid<<END;
		return;
	}
	// special check
	if (m_buffitems.size() > 100)
	{
		SSError<<"buff item is full, itemid:"<<itemid<<END;
		return;
	}
	auto i = m_buffitems.find(itemid);
	if (i != m_buffitems.end())
	{
		++i->second.itemcount;
	}
	else
	{
		SBuffItem item;
		item.itemid = itemid;
		item.itemcount = 1;
		item.timeexpire = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDRESWAR);
		m_buffitems[itemid] = item;
	}
	m_ischanged.Set();
}

bool BuffRecord::UseBuffItem(UINT32 itemid)
{
	auto i = m_buffitems.find(itemid);
	if (i == m_buffitems.end())
	{
		return false;
	}
	else
	{
		if (i->second.itemcount <= 1)
		{
			m_buffitems.erase(i);
		}
		else
		{
			--i->second.itemcount;
		}
		m_ischanged.Set();

		ItemBuffConf* conf = ItemConfig::Instance()->GetBuffConf(itemid);
		if (NULL == conf)
		{
			return false;
		}
		for (auto i = conf->Buffs.begin(); i != conf->Buffs.end(); ++i)
		{
			AddBuff(conf->Type, i->seq[0], i->seq[1]);
		}
		return true;
	}
}

const std::map<UINT32, BuffDesc>* BuffRecord::GetBuffs(UINT32 type)
{
	auto i = m_buffs.find(type);
	if (i == m_buffs.end())
	{
		return NULL;	
	}
	return &i->second;
}

std::vector<DoodadItem>* BuffRecord::GetDoodItem()
{
	return  &m_doodads;
}

bool BuffRecord::AddDoodItem(UINT32 itemid, UINT32 doodadid)
{
	Scene* pScene = m_role->GetCurrScene();
	if(NULL == pScene)
	{
		return false;
	}
	//优先叠加
	bool overlapflag = false;
	bool havesameflag = false;
	for(size_t i = 0; i < m_doodads.size(); ++i)
	{
		auto& tmp = m_doodads[i];
		if(!tmp.IsEmpty() && tmp.GetItemID() == itemid)
		{
			havesameflag = true;
			if(tmp.AddOverlap())	
			{
				overlapflag = true;
				break;
			}
		}
	}
	if(overlapflag)
	{
		NotifyItemAllSkils();
		return true;
	}

	if(havesameflag && !CDoodadItemSkillConfig::Instance()->CanHaveSameItem(pScene->GetSceneType()))
	{//不能有重复道具
		SSDebug << " cannot have same item itemid = " << itemid << END;
		return false;
	}

	//添空位
	bool addflag = false;
	for(size_t i = 0; i < m_doodads.size(); ++i)
	{
		auto& tmp = m_doodads[i];
		if(tmp.IsEmpty())
		{
			UINT32 overlapLimit = CDoodadItemSkillConfig::Instance()->GetItemOverlapCount(itemid, pScene->GetSceneType());
			tmp.Init(itemid, overlapLimit);
			addflag = true;
			break;;
		}
	}
	if(addflag)
	{
		NotifyItemAllSkils();
		return true;
	}
	SSDebug << " have not empty slot " << END;
	return false;
}

void BuffRecord::GetReHorseInfo(KKSG::HorseReConnectRes *roRes)
{
	//UINT32 nIndex = 0;
	//for (auto iter = m_doodads.begin(); iter!=m_doodads.end(); iter++)
	//{
	//	if (nIndex>= DOODADITEM_MAX)
	//	{
	//		return;
	//	}
	//	if ((*iter).itemid!=0)
	//	{
	//		KKSG::DoodadItemAddNtf*pItem = roRes->add_item();
	//		pItem->set_itemid((*iter).itemid);
	//		pItem->set_index(nIndex);
	//	}
	//	nIndex++;
	//}
}

bool BuffRecord::UseDoodItem(UINT32 index)
{
	if (index >= m_doodads.size())
	{
		return false;	
	}
	DoodadItem& doodad = m_doodads[index];
	if (doodad.IsEmpty())
	{
		return false;
	}

	Scene* scene = m_role->GetCurrScene();
	if (NULL == scene)
	{
		return false;
	}

	// effect
	BuffTable::RowData* pData = XBuffTemplateManager::Instance()->GetBuffInfo(doodad.GetItemID(), 1, scene); 
	if (NULL != pData)
	{
		XAddBuffActionArgs e;
		e.singleBuff.buffID = pData->BuffID;
		e.singleBuff.BuffLevel = pData->BuffLevel;
		e.Caster = m_role->GetID();
		m_role->GetXObject()->DispatchAction(&e);
	}else
	{
		return false;
	}

	if (CDoodadItemSkillConfig::Instance()->IsUseNotify(scene->GetSceneType()))
	{
		PtcG2C_DoodadItemUseNtf ntf;
		ntf.m_Data.set_buffid(pData->BuffID);
		ntf.m_Data.set_roleid(m_role->GetID());
		scene->Broadcast(ntf);
	}

	doodad.DueOverlap();
	NotifyItemAllSkils();

	return true;
}

void BuffRecord::FillItemAllSkills(KKSG::DoodadItemAllSkill& data)
{
	for(size_t i = 0; i < m_doodads.size(); ++i)
	{
		if(!m_doodads[i].IsEmpty())
		{
			m_doodads[i].FillData(i, *data.add_skills());
		}
	}
}

void BuffRecord::NotifyItemAllSkils()
{
	PtcG2C_DoodadItemSkillsNtf ptc;
	FillItemAllSkills(ptc.m_Data);
	m_role->Send(ptc);
	DoodItemInfoPrint();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool BuffRecord::IsTransforming()
{
	return KKSG::OutLook_Trans == m_role->GetState().statetype();
}

bool BuffRecord::IsSmalling()
{
	return m_smallbuff.Valid() && m_smallbuff.effectingoption;
}

UINT32 BuffRecord::GetTransTime()
{
	UINT32 transleft = m_transformbuff.GetTimeLeft();
	UINT32 smallleft = m_smallbuff.GetTimeLeft();
	return transleft > smallleft ? transleft : smallleft;
}

UINT32 BuffRecord::CanTrans(int mask)
{
	// system open
	if (!GetGlobalConfig().IsTransOpen)
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	// scene valid
	Scene* scene = m_role->GetCurrScene();
	if (NULL == scene)
	{
		return KKSG::ERR_ARENA_ADDCOUNTLACKCOIN;
	}
	if (!SceneConfig::Instance()->IsTransScene(scene->GetSceneType()))
	{
		return KKSG::ERR_ARENA_ADDCOUNTLACKCOIN;
	}
	// state valid
	if (mask & 1)
	{
		if (KKSG::OutLook_Normal != m_role->GetState().statetype() && KKSG::OutLook_Trans != m_role->GetState().statetype())
		{
			return KKSG::ERR_SHOP_ITEMNOTEXIST;
		}
	}
	// big or small
	if (mask & 2)
	{
		if (KKSG::OutLook_RidePetCopilot == m_role->GetState().statetype() || KKSG::OutLook_RidePet == m_role->GetState().statetype())
		{
			return KKSG::ERR_SHOP_ITEMNOTEXIST;
		}
	}
	return KKSG::ERR_SUCCESS;
}

void BuffRecord::DelTransBuff(int reason, int mask)
{
	if ((mask & 1) && HasTransBuff())
	{
		SSInfo<<"roleid:"<<m_role->GetID()<<" try delete trans buff:"<<m_transformbuff.buff.buffID<<" reason:"<<reason<<END;

		m_transformbuff.Del(m_role, reason);
	}
	if ((mask & 2) && HasSmallBuff())
	{
		SSInfo<<"roleid:"<<m_role->GetID()<<" try delete small buff:"<<m_smallbuff.buff.buffID<<" reason:"<<reason<<END;

		m_smallbuff.Del(m_role, reason);
	}
}

void BuffRecord::AddTransBuff(UINT32 buffid, UINT32 bufflevel, UINT64 casterid)
{
	if (NULL == m_role || NULL == m_role->GetCurrScene())
	{
		return;
	}

	SSInfo<<"roleid:"<<m_role->GetID()<<" try add trans buff:"<<buffid<<" casterid:"<<casterid<<END;

	XAddBuffActionArgs args;

	BuffDesc buff;
	buff.buffID = buffid;
	buff.BuffLevel = bufflevel;
	buff.casterID = casterid;
	args.vecBuffs.push_back(buff);
	args.Caster = casterid;
	args.bClientOnly = !m_role->GetCurrScene()->IsSyncScene();

	m_role->GetXObject()->DispatchAction(&args);
}

void BuffRecord::RecoverTransBuff(bool isenterscene, int mask)
{
	if (NULL == m_role || NULL == m_role->GetCurrScene())
	{
		return;
	}
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	if ((mask & 1) && HasTransBuff())
	{
		bool caneffect = isenterscene ? m_transformbuff.effectingoption : true;
		if (caneffect)
		{
			if (nowtime > m_transformbuff.buff.effecttime)
			{
				m_transformbuff.Reset();
				m_ischanged.Set();
			}
			else
			{
				m_transformbuff.Add(m_role, nowtime, m_transformbuff.ispassive ? (UINT64)(-1) : m_role->GetID());

				SSInfo<<"roleid:"<<m_role->GetID()<<" try recover trans buff:"<<m_transformbuff.buff.buffID<<END;
			}
		}
	}
	if ((mask & 2) && HasSmallBuff())
	{
		bool caneffect = isenterscene ? m_smallbuff.effectingoption : true;
		if (caneffect)
		{
			if (nowtime > m_smallbuff.buff.effecttime)
			{
				m_smallbuff.Reset();
				m_ischanged.Set();
			}
			else
			{
				m_smallbuff.Add(m_role, nowtime, m_smallbuff.ispassive ? (UINT64)(-1) : m_role->GetID());

				SSInfo<<"roleid:"<<m_role->GetID()<<" try recover small buff:"<<m_smallbuff.buff.buffID<<END;
			}
		}
	}
}

void BuffRecord::OnAddTransBuff(UINT32 buffid, UINT32 bufflevel, UINT32 timeleft, UINT64 caseruid)
{
	// buff data
	BuffDesc buff;
	buff.buffID = buffid;
	buff.BuffLevel = bufflevel;
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	if (UINT32(-1) != timeleft)
	{
		buff.effecttime = nowtime + timeleft;
	}
	else
	{
		SSWarn<<"role:"<<m_role->GetID()<<" get buffid:"<<buffid<<" time:"<<timeleft<<END;
		buff.effecttime = nowtime;
	}
	
	bool ispassive = caseruid != m_role->GetID();

	if (TransformBuff::IsSmallBuff(buffid))
	{
		m_smallbuff.buff = buff;
		m_smallbuff.ispassive = ispassive;
		m_smallbuff.effectingoption = true;

		SSInfo<<"role:"<<m_role->GetID()<<" add small buff succeed, timeleft:"<<GetTransTime()<<" buffid:"<<buffid<<END;
	}
	else
	{
		m_transformbuff.buff = buff;
		m_transformbuff.ispassive = ispassive;
		m_transformbuff.effectingoption = true;
	
		// state
		m_role->SetState(KKSG::OutLook_Trans, buffid);

		RoleOutLook outlook(m_role);
		outlook.Fill(SUMMARY_STATE, m_role);
		outlook.BroadCast();

		SSInfo<<"role:"<<m_role->GetID()<<" add trans succeed, timeleft:"<<GetTransTime()<<" buffid:"<<buffid<<END;
	}

	ChatNotifyGS(caseruid, m_role->GetID(), buffid, GetTransTime());

	m_ischanged.Set();
}

void BuffRecord::OnDelTransBuff(UINT32 buffid, bool istruedelete)
{
	if (TransformBuff::IsSmallBuff(buffid))
	{
		if (istruedelete)
		{
			m_smallbuff.Reset();
		}
		SSInfo<<"role:"<<m_role->GetID()<<" delete small succeed, timeleft:"<<GetTransTime()<<" istrue:"<<istruedelete<<END;
	}
	else
	{
		if (istruedelete)
		{
			m_transformbuff.Reset();
		}

		m_role->SetState(KKSG::OutLook_Normal);

		RoleOutLook outlook(m_role);
		outlook.Fill(SUMMARY_STATE, m_role);
		outlook.BroadCast();

		SSInfo<<"role:"<<m_role->GetID()<<" delete trans succeed, timeleft:"<<GetTransTime()<<" istrue:"<<istruedelete<<END;
	}

	m_ischanged.Set();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void BuffRecord::DoodItemInfoPrint()
{
	for(size_t i = 0; i < m_doodads.size(); ++i)
	{
		KKSG::DoodadItemSkill data;
		m_doodads[i].FillData(i, data);
		SSDebug << " index = " << data.index() << " itemid = " << data.itemid() << " overlap = " << data.count() << END;
	}
}

void DoodadItem::Init(UINT32 itemID, UINT32 overlapLimit)
{
	m_itemID = itemID;
	m_overlapLimit = overlapLimit;
	m_curOverlap = 1;
}

bool DoodadItem::IsEmpty()
{
	return (0 == m_itemID || 0 == m_curOverlap);
}

bool DoodadItem::IsFull()
{
	return m_curOverlap >= m_overlapLimit;
}

bool DoodadItem::AddOverlap()
{
	if(IsFull())
	{
		return false;
	}
	m_curOverlap ++;
	return true;
}

void DoodadItem::DueOverlap()
{
	if(0 == m_curOverlap)
	{
		return;
	}
	m_curOverlap --;
	if(0 == m_curOverlap)
	{
		m_itemID = 0;
	}
}

void BuffRecord::ChatNotifyGS(UINT64 casterroleid, UINT64 destroleid, UINT32 buffid, UINT32 time)
{
	if (0 == casterroleid || (UINT64)(-1) == casterroleid || casterroleid == destroleid)
	{
		return;
	}
	// monster
	BuffTable::RowData* buffdata = XBuffTemplateManager::Instance()->GetBuffInfo(buffid, 1, NULL);
	if (NULL == buffdata)
	{
		return;
	}
	UINT32 monsterid = buffdata->StateParam;
	const XEntityStatistics::RowData* monsterdata = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(monsterid);
	if (NULL == monsterdata)
	{
		return;
	}

	Notice notice(TransOtherNotice);
	notice.Replace("$C", monsterdata->Name);
	notice.Replace("$C", ToString(time));
	if (NULL == notice.chatinfo)
	{
		return;
	}

	PtcG2M_PrivateChat chat;
	chat.m_Data.set_sourceroleid(casterroleid);
	chat.m_Data.set_destroleid(destroleid);

	chat.m_Data.set_content(notice.chatinfo->info());

	MSLink::Instance()->SendTo(chat);
}

void DoodadItem::FillData(UINT32 index, KKSG::DoodadItemSkill& data)
{
	data.set_index(index);
	data.set_itemid(m_itemID);
	data.set_count(m_curOverlap);
}
