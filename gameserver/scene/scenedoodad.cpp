#include "pch.h"
#include <time.h>
#include "scenedoodad.h"
#include "scene.h"
#include "pb/project.pb.h"
#include "gamelogic/XLevelSpawnMgr.h"
#include "gamelogic/itemconfig.h"
#include "foreach.h"
#include "unit/role.h"
#include "table/BuffTable.h"
#include "buff/XBuffTemplateManager.h"
#include "unit/rolemanager.h"
#include "math/XVector3.h"
#include "unit/enemy.h"
#include "entity/XObject.h"
#include "define/itemdef.h"
#include "gamelogic/bagtransition.h"
#include "util/XRandom.h"
#include "battle/ptcg2c_enemydorpdoodadntf.h"
#include "table/globalconfig.h"
#include "battle/ptcg2c_fetchenemydoodadntf.h"
#include "scene/grid.h"
#include "battle/ptcg2c_sceneleftdoodad.h"
#include "scene/scenetower.h"
#include "sceneteam.h"
#include "battle/ptcg2c_startrollntf.h"
#include "gamelogic/noticemgr.h"
#include "buff/buffrecord.h"
#include "gamelogic/lotteryrecord.h"
#include "config/itemdropconfig.h"
#include "gamelogic/skilllevel.h"
#include "bigmeleefight.h"
#include "battlefieldfight.h"
#include "gamelogic/rolemisc.h"
#include "scenewedding.h"
#include "gamelogic/helldoordropmgr.h"


class CheckDoodadTimer : public ITimer
{
public:
	CheckDoodadTimer(SceneDoodad* doodad)
	{
		m_sceneDoodad = doodad;
	}
	void Start()
	{
		m_TimerHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}

	void Stop()
	{
		CTimerMgr::Instance()->KillTimer(m_TimerHandler);
	}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		m_sceneDoodad->Update();
	}

public:
	SceneDoodad* m_sceneDoodad;
	HTIMER m_TimerHandler;
};


SceneDoodad::SceneDoodad(Scene* scene)
{
	m_curDoodadID = 0;
	m_pScene = scene;
	m_Doodads.clear();
	m_ServerModeDoodads.clear();
	m_doodadTimer = NULL;
	m_hasrandleft = false;
	m_droppableInfo = NULL;

	m_staticDropDoodad.clear();

	if (!SceneConfig::Instance()->IsSpawnExcept(m_pScene->GetSceneType()))
	{
		m_doodadTimer = new CheckDoodadTimer(this);
		m_doodadTimer->Start();
		if (!XLevelSpawnMgr::Instance()->GetLevelWaveMapDoodad(m_pScene->GetSceneTemplateID(), m_staticDropDoodad))
		{
			LogInfo("scene :%u, doodadinfo not exist.", m_pScene->GetSceneTemplateID());
		}
	}
}

SceneDoodad::~SceneDoodad()
{
	if (m_doodadTimer)
	{
		m_doodadTimer->Stop();
		delete m_doodadTimer;
		m_doodadTimer = NULL;
	}
	m_curDoodadID = 0;
	m_pScene = NULL;

	if (m_droppableInfo)
	{
		delete m_droppableInfo;
		m_droppableInfo = NULL;
	}
}

void SceneDoodad::Update()
{
	auto iter = m_RollList.begin();
	auto end = m_RollList.end();
	if(iter != end)
	{
		int now = time(0);
		std::vector<Role*> roles;
		m_pScene->GetPlayersNotHelper(roles);

		for (auto i = (*iter)->m_rollsRes.begin(); i != (*iter)->m_rollsRes.end();)
		{
			if (i->second)
			{
				++i;
				continue;
			}
			if(now - (*iter)->beginRollTime >=  GetGlobalConfig().ManualRollTime)
			{
				bool inScene = false;
				Role* tempRole = NULL;
				foreach (j in roles)
				{
					if (i->first == (*j)->GetID())
					{
						inScene = true;
						tempRole = (*j);
						break;
					}
				}
				if(inScene)
				{
					KKSG::EnemyDoodadInfo tempInfo;
					_ConvertToKKSG(*(*iter), tempInfo);
					ChooseRoll(tempRole, 1, tempInfo);
				}
				else
				{
					(*iter)->m_rollsRes.erase(i++);
					continue;
				}
			}
			++i;
		}

		if (_IsRollEnd(**iter))
		{
			iter = m_RollList.erase(iter);
			if (iter != end)
			{
				_NotifyBeginRoll(*iter);
			}
		}
	}

}

void SceneDoodad::Clear()
{
	m_ServerModeDoodads.clear();
}

void SceneDoodad::SetPickedDoodad(UINT32 waveID)
{
	foreach (i in m_Doodads)
	{
		if (i->waveID == waveID)
		{
			i->IsPicked = true;
			return;
		}
	}
}

void SceneDoodad::InitDroppableInfo(DroppableInfo& info)
{
	if (!info.nonHelperCount)
	{
		LogError("scene:%llu, error nonhelper count.", m_pScene->GetSceneID());
		return;
	}
	m_droppableInfo = new DroppableInfo();
	m_droppableInfo->totalCount = info.totalCount;
	m_droppableInfo->nonHelperCount = info.nonHelperCount;
}

const std::list<PickDoodadInfo> & SceneDoodad::GetDoodadsInfo()
{
	return m_Doodads;
}

std::list<PickServerModeDoodadInfo> & SceneDoodad::GetServerDoodadsInfo()
{
	return m_ServerModeDoodads;
}

void SceneDoodad::GetServerDoodadsInfo(int type, std::list<PickServerModeDoodadInfo>& listDoodad)
{
	for (auto iter = m_ServerModeDoodads.begin(); iter != m_ServerModeDoodads.end(); ++iter)
	{
		if (!iter->roleID && iter->Type == (UINT32)type)
		{
			listDoodad.push_back(*iter);
		}
	}
}

void SceneDoodad::FillDoodadInfo(KKSG::SceneCfg* pData)
{
	std::vector<XLevelDoodad> vec;
	if (XLevelSpawnMgr::Instance()->GetLevelDoodad(m_pScene->GetSceneTemplateID(), vec))
	{
		for (UINT32 i = 0; i < vec.size(); ++i)
		{
			DoodadConf *pConf = ItemConfig::Instance()->GetRandomDoodadDrop(vec[i].m_DoodadID);
			if (pConf)
			{
				if (pConf->ItemID != 0)
				{
					KKSG::DoodadInfo *pInfo = pData->add_doodads();
					pInfo->set_waveid(vec[i].m_WaveID);
					pInfo->set_type(pConf->DoodadType);
					pInfo->set_id(pConf->ItemID);
					pInfo->set_count(pConf->ItemCount);

					PickDoodadInfo pick;
					pick.waveID = vec[i].m_WaveID;
					pick.Type = pConf->DoodadType;
					pick.ItemID = pConf->ItemID;
					pick.Count = pConf->ItemCount;
					pick.IsPicked = false;
					m_Doodads.push_back(pick);
				}
			}
		}
	}
}


bool isSameDoodad(std::list<PickServerModeDoodadInfo>::const_iterator iter, const KKSG::EnemyDoodadInfo& arg)
{
	if (iter->id == arg.index() && (int)iter->waveID == arg.waveid() && iter->ItemID == arg.id())
	{
		return true;
	}
	return false;
}

bool isSameDoodad(const PickServerModeDoodadInfo* pInfo, const KKSG::EnemyDoodadInfo& arg)
{
	if (pInfo->id == arg.index() && (int)pInfo->waveID == arg.waveid() && pInfo->ItemID == arg.id())
	{
		return true;
	}
	return false;
}

void SceneDoodad::RemoveBuffDoodadById(int doodadID)
{
	auto iter = m_ServerModeDoodads.begin();

	while( iter != m_ServerModeDoodads.end())
	{
		if (iter->Type == 1 && iter->doodadID == doodadID) 
		{
			BuffTable::RowData* pData = XBuffTemplateManager::Instance()->GetBuffInfo(iter->ItemID, 1, m_pScene);
			if (!pData)
				iter ++;
			else
			{
				KKSG::EnemyDoodadInfo info;
				_ConvertToKKSG(*iter, info);
				LogInfo("Remove buff by doodad id: %d, itemid: %d", doodadID, iter->ItemID);
				_NotifyDoodadDisappear(info);

				iter = m_ServerModeDoodads.erase(iter);
			}
		}
		else
			iter ++;
	}
}

void SceneDoodad::FetchDoodad(Role* role, const KKSG::EnemyDoodadInfo& arg, KKSG::RollInfoRes& roRes, bool bRand)
{
	roRes.set_errcode(KKSG::ERR_FAILED);

	if (!role || role->IsDead())
	{
		return;
	}
	float distance = Vector3::Distance(role->GetXObject()->GetPosition(), Vector3(arg.pos()));
	if (!bRand && distance > 1.5f)
	{
		return;
	}

	for (auto iter = m_ServerModeDoodads.begin(); iter != m_ServerModeDoodads.end(); ++iter)
	{
		if(!isSameDoodad(iter, arg))
		{
			continue;
		}
		if (iter->roleID)
		{
			return;
		}
		if (iter->canPickRole && role->GetID() != iter->canPickRole)
		{
			roRes.set_errcode(KKSG::ERR_CANNOT_PICK_THIS_ITEM);
			return;
		}
		bool isget = false;
		if (iter->Type == 1)
		{
			BuffTable::RowData* pData = XBuffTemplateManager::Instance()->GetBuffInfo(iter->ItemID, 1, role->GetCurrScene());
			if (!pData)
				return;
			XAddBuffActionArgs e;
			e.singleBuff.buffID = pData->BuffID;
			e.singleBuff.BuffLevel = pData->BuffLevel;
			e.singleBuff.type = 1;
			e.Caster = role->GetID();
			role->GetXObject()->DispatchAction(&e);

			isget = true;
			roRes.set_errcode(KKSG::ERR_SUCCESS);
		}
		else if (iter->Type == 2)
		{
			if (role->Get<BuffRecord>()->AddDoodItem(iter->ItemID, iter->doodadID))
			{
				roRes.set_errcode(KKSG::ERR_SUCCESS);
			}
			else
			{
				roRes.set_errcode(KKSG::ERR_DOODAD_FULL);
			}
			isget = true;
		}
		else if (iter->Type == 3)
		{
			if (KKSG::ERR_SUCCESS == role->Get<RoleSkillSet>()->LevelTransformSkill(1, 0, 0, 0))
			{
				roRes.set_errcode(KKSG::ERR_SUCCESS);
			}
			else
			{
				roRes.set_errcode(KKSG::ERR_DOODAD_FULL);
			}
			isget = true;
		}else if(iter->Type == 4)
		{
			SceneBMFight*  pSceneBmFight = m_pScene->GetBMFightHandler();
			if (pSceneBmFight)
			{
				m_pScene->GetBMFightHandler()->PickUpPoint(role->GetID(),role->GetProfession(),role->GetLevel(),iter->Count);
				roRes.set_errcode(KKSG::ERR_SUCCESS);
			}

			SceneBFFight*  pSceneBfFight = m_pScene->GetBFFightHandler();
			if (pSceneBfFight)
			{
				pSceneBfFight->AddPoint(role->GetID(),iter->Count);
				roRes.set_errcode(KKSG::ERR_SUCCESS);
			}
			isget = true;
		}

		if (isget)
		{
			iter->roleID = role->GetID();
			_NotifyDoodadDisappear(role, arg);

			XAIEventArgs args;
			args.MsgTargetTemplateId = 0;
			args.EventArg = "fetchdoodad";
			args.Templateid = iter->doodadID;
			args.Pos = iter->pos;

			m_pScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
			break;
		}

		if (iter->beginRollTime)
		{
			roRes.set_errcode(KKSG::ERR_FAILED);
			return;
		}

		Role* tempRole = NULL;
		if (_InitRollInfo(&*iter))
		{
			roRes.set_errcode(KKSG::ERR_SUCCESS);
			_NotifyDoodadDisappear(arg);
			return;
		}
		else
		{
			if (!m_pScene->IsHelper(role->GetID()))
			{
				tempRole = role;
			}
			else
			{
				std::vector<Role*> roles;
				m_pScene->GetPlayersNotHelper(roles);
				if (!roles.empty())
				{
					UINT32 rand = XRandom::randInt(0,roles.size());
					if (rand < roles.size())
					{
						tempRole = roles[rand];
					}
				}
			}
		}
		if (!tempRole)
		{
			return;
		}
		KKSG::ErrorCode code = SpecialFetchHandle(tempRole, *iter);
		if (code != KKSG::ERR_SUCCESS)
		{
			roRes.set_errcode(code);
			return;
		}
		iter->roleID = tempRole->GetID();
		iter->bRand = bRand;

		_AddSyncSceneFetchReward(role, iter->ItemID, iter->Count);
		_NotifyDoodadDisappear(tempRole, arg);
		roRes.set_errcode(KKSG::ERR_SUCCESS);
		break;
	}

	/*if (roRes.errcode() == KKSG::ERR_SUCCESS && m_pScene->GetSpawner())
	{
		m_pScene->GetSpawner()->OnDoodadFetch(arg.waveid());
	}*/
	if (roRes.errcode() == KKSG::ERR_SUCCESS)
	{
		_OnDoodadFetch(arg.waveid());
	}
}

KKSG::ErrorCode SceneDoodad::SpecialFetchHandle(Role* pRole, PickServerModeDoodadInfo& info)
{
	if (m_pScene->GetSceneType() == KKSG::SCENE_WEDDING)
	{
		if (pRole->Get<RoleMisc>()->CheckAndAddWeddingCandyCount())
		{
			BagGiveItemTransition trans(pRole);
			trans.SetReason(ItemFlow_Wedding, ItemFlow_WeddingCandy);
			trans.GiveItem(info.ItemID, info.Count, info.IsBind);
			trans.NotifyClient();
			info.bRewarded = true;
		}
		else
		{
			return KKSG::ERR_DAILY_WEDDY_CANDY_OVERMAX;
		}
	}
	return KKSG::ERR_SUCCESS;
}

void SceneDoodad::_OnDoodadFetch(int waveID)
{
	if (m_pScene->GetSpawner())
	{
		m_pScene->GetSpawner()->OnDoodadFetch(waveID);
	}
}

void SceneDoodad::_AddSyncSceneFetchReward(Role* role, int itemID, int itemCount)
{
	if (m_pScene->GetSceneType() != KKSG::SCENE_TOWER)
	{
		return;
	}
	SceneTeam* team = m_pScene->GetSceneTeam();
	if (team)
	{
		team->AddSyncSceneFetchReward(role, itemID, itemCount);
		if(m_pScene->GetSceneTower())
		{
			m_pScene->GetSceneTower()->SendTowerInfo(role);
		}

	}
	else if (m_pScene->GetSceneTower())
	{
		m_pScene->GetSceneTower()->AddFetchReward(role, itemID, itemCount);
	}
}

void SceneDoodad::_ConvertToKKSG(const PickServerModeDoodadInfo& info, KKSG::EnemyDoodadInfo& doodadInfo)
{
	doodadInfo.set_index(info.id);
	doodadInfo.set_waveid(info.waveID);
	doodadInfo.set_type(info.Type);
	doodadInfo.set_id(info.ItemID);
	doodadInfo.set_count(info.Count);
	doodadInfo.set_droppertemplateid(info.TemplateID);
	doodadInfo.set_roleid(info.canPickRole);
	KKSG::Vec3* vec3 = doodadInfo.mutable_pos();
	vec3->set_x(info.pos.x);
	vec3->set_y(info.pos.y);
	vec3->set_z(info.pos.z);
}

Role* SceneDoodad::_GetRollMaxRole(PickServerModeDoodadInfo& info, bool& isAllSelected, UINT32& max)
{
	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);
	bool isAllRoll = true;
	Role* maxRole = NULL;
	max = 0;
	foreach (i in info.m_rollsRes)
	{
		if (!i->second)
		{
			isAllRoll = false;
			break;
		}
		if (i->second != (UINT32)-1 && i->second > max)
		{
			foreach(j in roles)
			{
				if ((*j)->GetID() == i->first)
				{
					maxRole = *j;
					max = i->second;
					break;
				}
			}
		}
	}

	isAllSelected = isAllRoll;
	return maxRole;
}

bool SceneDoodad::_IsRollEnd(PickServerModeDoodadInfo& info)
{
	if (!info.beginRollTime)
	{
		return true;
	}

	bool isAllRoll = true;
	Role* maxRole = NULL;
	UINT32 max = 0;
	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);

	maxRole = _GetRollMaxRole(info, isAllRoll, max);
	if(isAllRoll)
	{
		KKSG::EnemyDoodadInfo tempInfo;
		_ConvertToKKSG(info, tempInfo);
		if (!maxRole)
		{
			_ReRoll(&info, roles);
			/*foreach(i in roles)
			{
				fetchDoodadRoll(*i, 1, tempInfo);
			}*/
			maxRole = _GetRollMaxRole(info, isAllRoll, max);
		}
		ItemConf *pConf = ItemConfig::Instance()->GetItemConf(info.ItemID);
		if (pConf)
		{
			XItem item(0, info.ItemID, info.Count, pConf->ItemType);
			NoticeMgr::Instance()->NoticeAllFetchRoll(maxRole, &item, max);

			_AddSyncSceneFetchReward(maxRole, info.ItemID, info.Count);
		}

		_OnDoodadFetch(info.waveID);
		if (maxRole)
		{
			info.roleID = maxRole->GetID();
		}
		return true;
	}

	return false;
}

void SceneDoodad::_ReRoll(PickServerModeDoodadInfo* info, std::vector<Role*>& roles)
{
	KKSG::EnemyDoodadInfo tempInfo;
	_ConvertToKKSG(*info, tempInfo);
	foreach(i in roles)
	{
		auto find = info->m_rollsRes.find((*i)->GetID());
		if (find != info->m_rollsRes.end())
		{
			find->second = 0;
			ChooseRoll(*i, 1, tempInfo);
		}
	}

}

void SceneDoodad::_NotifyBeginRoll(PickServerModeDoodadInfo* info)
{
	if (!info)
	{
		return;
	}
	info->beginRollTime = time(0);
	PtcG2C_StartRollNtf ntf;
	KKSG::EnemyDoodadInfo* pInfo = ntf.m_Data.mutable_info();
	_ConvertToKKSG(*info, *pInfo);
	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);
	foreach(i in roles)
	{
		(*i)->Send(ntf);
	}
}


bool SceneDoodad::_InitRollInfo(PickServerModeDoodadInfo* info)
{
	if (!info)
	{
		return false;
	}
	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (!pTeam)
	{
		return false;
	}
	ItemConf* pItemConfig = ItemConfig::Instance()->GetItemConf(info->ItemID);
	if (!pItemConfig || !pItemConfig->ReqRoll)
	{
		return false;
	}
	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);
	if (pItemConfig->ReqRoll > info->Count || roles.size() <= 1)
	{
		return false;
	}
	info->beginRollTime = time(0);
	foreach (i in roles)
	{
		info->m_rollsRes[(*i)->GetID()] = 0;
	}
	for (size_t i = 100; i > 0; --i)
	{
		info->seeds.insert(std::make_pair<UINT32, UINT32>(i, i));
	}

	_AddRollInfo(info);

	if (m_RollList.size() == 1)
	{
		_NotifyBeginRoll(info);
	}
	
	LogDebug("scene:%u,add doodad id:%u, type:%u, doodadid:%u,itemid:%u, count:%u,wave:%u", m_pScene->GetSceneID(), info->id, info->Type, info->doodadID,info->ItemID, info->Count, info->waveID);

	return true;
}

void SceneDoodad::_AddRollInfo(PickServerModeDoodadInfo* info)
{
	m_RollList.push_back(info);
}

void SceneDoodad::ChooseRoll(Role* role, int type, const KKSG::EnemyDoodadInfo& doodadInfo/*, KKSG::RollInfoRes& roRes*/)
{
	if (!role)
	{
		return;
	}
	//std::list<PickServerModeDoodadInfo*> m_RollList;
	PickServerModeDoodadInfo* pInfo = NULL;
	foreach (i in m_RollList)
	{
		if (isSameDoodad((*i), doodadInfo))
		{
			pInfo = *i;
			break;
		}
	}
	if(!pInfo)
		return;

	bool canRoll = false;
	foreach (i in pInfo->m_rollsRes)
	{
		if (i->first == role->GetID())
		{
			if (!i->second)
			{
				canRoll = true;
			}
			break;
		}
	}
	if(!canRoll)
		return;

	if (type == 2)
	{
		pInfo->m_rollsRes[role->GetID()] = (UINT32)-1;
		ItemConf *pConf = ItemConfig::Instance()->GetItemConf(pInfo->ItemID);
		if (pConf)
		{
			XItem item(0, pInfo->ItemID, pInfo->Count, pConf->ItemType);
			NoticeMgr::Instance()->NoticeGiveUpFetchRoll(role, &item);

		}
		return;
	}

	if (pInfo->seeds.empty())
	{
		return;
	}
	UINT32 rand = XRandom::randInt(1,pInfo->seeds.size() + 1);
	UINT32 index = 0;
	auto iterSeed = pInfo->seeds.begin();
	for (; index < rand && iterSeed != pInfo->seeds.end(); ++index, ++iterSeed)
	{
		if (index == (rand - 1))
		{
			pInfo->m_rollsRes[role->GetID()] =  iterSeed->second;
			pInfo->seeds.erase(iterSeed);
			break;
		}
	}

	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(pInfo->ItemID);
	if (pConf)
	{
		XItem item(0, pInfo->ItemID, pInfo->Count, pConf->ItemType);
		NoticeMgr::Instance()->NoticeFetchRoll(role, &item, pInfo->m_rollsRes[role->GetID()]);
	}

}


Role* SceneDoodad::_FetchDoodadRoll(KKSG::RollInfoRes& roRes)
{
	std::map<UINT64, UINT32> mapRoll;
	std::map<UINT32, UINT32> seeds;

	for (size_t i = 100; i > 0; --i)
	{
		seeds.insert(std::make_pair<UINT32, UINT32>(i, i));
	}

	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);
	for (auto iter = roles.begin(); iter != roles.end(); ++iter)
	{
		if (seeds.empty())
		{
			return 0;
		}
		UINT32 rand = XRandom::randInt(1,seeds.size() + 1);
		UINT32 index = 0;
		auto iterSeed = seeds.begin();
		for (; index < rand && iterSeed != seeds.end(); ++index, ++iterSeed)
		{
			if (index == (rand - 1))
			{
				mapRoll.insert(std::make_pair((*iter)->GetID(), iterSeed->second));
				seeds.erase(iterSeed);
				break;
			}
		}
	}

	if (mapRoll.empty())
	{
		return 0;
	}
	UINT32 max = 0;
	auto maxIter = mapRoll.begin();
	for (auto iter = mapRoll.begin(); iter != mapRoll.end(); ++iter)
	{
		if (iter->second > max)
		{
			max = iter->second;
			maxIter = iter;
		}
		KKSG::RollInfo* info = roRes.add_info();
		info->set_roleid(iter->first);
		info->set_rollvalue(iter->second);
	}

	return RoleManager::Instance()->FindByRoleID(maxIter->first);
}

bool SceneDoodad::_CheckEnemyDoodadDroppable()
{
	if (!m_droppableInfo)
	{
		return true;
	}

	int total = m_droppableInfo->totalCount;
	int count = m_droppableInfo->nonHelperCount;

	int result = XRandom::randInt(1, total + 1);
	if (count >= result)
	{
		return true;
	}

	return false;
}

int SceneDoodad::_FillEnemyDoodad(PtcG2C_EnemyDorpDoodadNtf& send, const std::list<PickServerModeDoodadInfo>& doodadList)
{
	for (auto iter = doodadList.begin(); iter != doodadList.end(); ++iter)
	{
		if (!iter->roleID)
		{
			KKSG::EnemyDoodadInfo* pInfo = send.m_Data.add_doodadinfo();
			_ConvertToKKSG(*iter, *pInfo);
		}
	}
	return send.m_Data.doodadinfo_size();
}

void SceneDoodad::GenerateEnemyDroppedDoodad(Enemy* pEnemy, bool isLastEnemy)
{
	if (!pEnemy)
	{
		return;
	}

	auto find = m_staticDropDoodad.find(pEnemy->GetWaveID());
	if (find == m_staticDropDoodad.end())
	{
		return;
	}

	std::vector<XLevelDoodad>& vecDoodad = find->second;

	if (!vecDoodad.empty())
	{
		UINT32 randCount = XRandom::randInt(1,vecDoodad.size() + 1);
		if (isLastEnemy)
		{
			randCount = vecDoodad.size();
		}

		if (randCount > vecDoodad.size())
		{
			return;
		}

		std::list<PickServerModeDoodadInfo> tempDrop;
		tempDrop.clear();

		//UINT32 radius =  GetGlobalConfig().GetInt("EnemyDropRadius",2);
		//UINT32 degree =  GetGlobalConfig().GetInt("EnemyDropDegree",2);
		//UINT32 max = (UINT32)(XRound / degree);
		UINT32 entercount = 0;
		while (randCount -- )
		{
			UINT32 index = XRandom::randInt(0,vecDoodad.size());
			if (index >= vecDoodad.size())
			{
				return;
			}
			if (!_CheckEnemyDoodadDroppable())
			{
				vecDoodad.erase(vecDoodad.begin() + index);
				continue;
			}
			DoodadConf *pConf = ItemConfig::Instance()->GetRandomDoodadDrop(vecDoodad[index].m_DoodadID);
			if (pConf && pConf->ItemID != 0)
			{
				PickServerModeDoodadInfo pick;
				pick.id = _GetNextDoodadID();
				pick.index = vecDoodad[index].index;
				pick.roleID = 0;
				pick.waveID = vecDoodad[index].m_WaveID;
				pick.doodadID = vecDoodad[index].m_DoodadID;
				pick.Type = pConf->DoodadType;
				pick.ItemID = pConf->ItemID;
				pick.Count = pConf->ItemCount;
				pick.TemplateID = pEnemy->GetTemplateID();

				/*Vector3 dir = pEnemy->GetXObject()->GetFace_p();
				dir = - dir;

				do 
				{
					if (temp > max)
					{
						pick.pos = pEnemy->GetXObject()->GetPosition_p();
						break;
					}
					Vector3 vec = Vector3::HorizontalRotate(dir, (float)degree*(temp++));
					pick.pos = pEnemy->GetXObject()->GetPosition_p() + vec * (float)radius;
				} while (m_pScene->GetGrid() && m_pScene->GetGrid()->IsCellBlocked(pick.pos));

				pick.pos.y = m_pScene->GetGrid() ? m_pScene->GetGrid()->GetHeight(pick.pos.x, pick.pos.z) : 0;
				*/
				pick.pos = _GenerateDoodadPos(pEnemy, entercount, true);

				m_ServerModeDoodads.push_back(pick);
				LogDebug("scene:%u,add doodad id:%u, type:%u, doodadid:%u,itemid:%u, count:%u,wave:%u", m_pScene->GetSceneID(), pick.id, pick.Type, pick.doodadID,pick.ItemID, pick.Count, pick.waveID);
				tempDrop.push_back(pick);
			}

			vecDoodad.erase(vecDoodad.begin() + index);
		}

		_NotifyAllNewDoodadAppear(pEnemy, tempDrop);
	}
}

Vector3 SceneDoodad::_GenerateDoodadPos(Enemy* pEnemy, UINT32& entercount, bool positive)
{
	Vector3 pos;

	UINT32 radius =  GetGlobalConfig().GetInt("EnemyDropRadius",2);
	float degree =  GetGlobalConfig().GetInt("EnemyDropDegree",2);
	UINT32 max = (UINT32)(XRound / degree);

	if (!positive)
	{
		degree = -degree;
	}

	Vector3 dir = pEnemy->GetXObject()->GetFace_p();
	dir = - dir;

	do 
	{
		if (entercount > max)
		{
			pos = pEnemy->GetXObject()->GetPosition_p();
			break;
		}
		Vector3 vec = Vector3::HorizontalRotate(dir, (float)degree*(entercount++));
		pos = pEnemy->GetXObject()->GetPosition_p() + vec * (float)radius;
	} while (m_pScene->GetGrid() && m_pScene->GetGrid()->IsCellBlocked(pos));

	pos.y = m_pScene->GetGrid() ? m_pScene->GetGrid()->GetHeight(pos.x, pos.z) : 0;

	return pos;
}

void SceneDoodad::GenerateDoodad(UINT32 doodadID, UINT32 waveID, Vector3& pos)
{
	DoodadConf *pConf = ItemConfig::Instance()->GetRandomDoodadDrop(doodadID);
	if (pConf && pConf->ItemID != 0)
	{
		PickServerModeDoodadInfo pick;
		pick.id = _GetNextDoodadID();
		pick.index = 0;
		pick.roleID = 0;
		pick.waveID = waveID;
		pick.doodadID = doodadID;
		pick.Type = pConf->DoodadType;
		pick.ItemID = pConf->ItemID;
		pick.Count = pConf->ItemCount;
		pick.pos = pos;

		m_ServerModeDoodads.push_back(pick);
		LogDebug("scene:%u,add doodad id:%u, type:%u, doodadid:%u,itemid:%u, count:%u,wave:%u", m_pScene->GetSceneID(), pick.id, pick.Type, pick.doodadID,pick.ItemID, pick.Count, pick.waveID);

		std::list<PickServerModeDoodadInfo> tempDrop;
		tempDrop.clear();
		tempDrop.push_back(pick);

		PtcG2C_EnemyDorpDoodadNtf send;

		if (_FillEnemyDoodad(send, tempDrop))
		{
			m_pScene->Broadcast(send, NULL);
		}
	}
}

void SceneDoodad::GenerateEnemyPandoraDroppedDoodad(Enemy* pEnemy, Role* role)
{
	if (!pEnemy || !role)
	{
		return;
	}

	const EnemyTemplate* data = pEnemy->GetTemplate();
	if (!data)
	{
		return;
	}

	std::list<PickServerModeDoodadInfo> tempDrop;

	UINT32 entercount = 0;

	for (auto i = data->PandoraDropIDs.begin(); i != data->PandoraDropIDs.end(); i++)
	{
		const ItemDesc* itemDesc = role->Get<LotteryRecord>()->GetPandoraDropItem(*i);
		if (itemDesc == NULL || !itemDesc->itemID || !itemDesc->itemCount)
		{
			continue;
		}

		PickServerModeDoodadInfo pick;
		pick.id = _GetNextDoodadID();
		pick.index = 0;
		pick.roleID = 0;
		pick.waveID = pEnemy->GetWaveID();
		pick.doodadID = 0;
		pick.Type = 0;
		pick.ItemID = itemDesc->itemID;
		pick.Count = itemDesc->itemCount;
		pick.IsBind = itemDesc->isbind;
		pick.TemplateID = pEnemy->GetTemplateID();
		pick.pos = _GenerateDoodadPos(pEnemy, entercount, false);
		m_ServerModeDoodads.push_back(pick);

		tempDrop.push_back(pick);

		LogDebug("scene:%u,add doodad id:%u, type:%u, doodadid:%u,itemid:%u, count:%u,wave:%u", m_pScene->GetSceneID(), pick.id, pick.Type, pick.doodadID,pick.ItemID, pick.Count, pick.waveID);
	}

	for (auto i = data->DropIDs.begin(); i != data->DropIDs.end(); i++)
	{
		const ItemDesc* itemDesc = ItemDropConfig::Instance()->GetRandomDrop(*i, role->GetLevel());
		if (itemDesc == NULL || !itemDesc->itemID || !itemDesc->itemCount)
		{
			continue;
		}

		PickServerModeDoodadInfo pick;
		pick.id = _GetNextDoodadID();
		pick.index = 0;
		pick.roleID = 0;
		pick.waveID = pEnemy->GetWaveID();
		pick.doodadID = 0;
		pick.Type = 0;
		pick.ItemID = itemDesc->itemID;
		pick.Count = itemDesc->itemCount;
		pick.IsBind = itemDesc->isbind;
		pick.TemplateID = pEnemy->GetTemplateID();
		pick.pos = _GenerateDoodadPos(pEnemy, entercount, false);
		m_ServerModeDoodads.push_back(pick);

		tempDrop.push_back(pick);

		LogDebug("scene:%u,add doodad id:%u, type:%u, doodadid:%u,itemid:%u, count:%u,wave:%u", m_pScene->GetSceneID(), pick.id, pick.Type, pick.doodadID,pick.ItemID, pick.Count, pick.waveID);
	}

	std::vector<const ItemDesc*> hellDoorItems;
	for (size_t i = 0; i<data->HellDrop.size();i++)
	{
		HellDoorDropMgr::Instance()->GetItem(role, 	data->HellDrop[i], hellDoorItems);
	}

	for (auto iter = hellDoorItems.begin(); iter != hellDoorItems.end(); ++iter)
	{
		if (*iter == NULL || !(*iter)->itemID || !(*iter)->itemCount)
		{
			continue;
		}
		PickServerModeDoodadInfo pick;
		pick.id = _GetNextDoodadID();
		pick.index = 0;
		pick.roleID = 0;
		pick.canPickRole = role->GetID();
		pick.waveID = pEnemy->GetWaveID();
		pick.doodadID = 0;
		pick.Type = 0;
		pick.ItemID = (*iter)->itemID;
		pick.Count = (*iter)->itemCount;
		pick.IsBind = (*iter)->isbind;
		pick.TemplateID = pEnemy->GetTemplateID();
		pick.pos = _GenerateDoodadPos(pEnemy, entercount, false);
		m_ServerModeDoodads.push_back(pick);

		tempDrop.push_back(pick);

		LogDebug("scene:%u,add doodad id:%u, type:%u, doodadid:%u,itemid:%u, count:%u,wave:%u", m_pScene->GetSceneID(), pick.id, pick.Type, pick.doodadID,pick.ItemID, pick.Count, pick.waveID);
	}

	_NotifyAllNewDoodadAppear(pEnemy, tempDrop);
}

void SceneDoodad::_NotifyAllNewDoodadAppear(Enemy* enemy, std::list<PickServerModeDoodadInfo>& doodadList)
{
	PtcG2C_EnemyDorpDoodadNtf send;

	if (_FillEnemyDoodad(send, doodadList))
	{
		m_pScene->Broadcast(enemy, send, NULL);
	}
}

void SceneDoodad::NotifyDoodadAppearOnEnter(Role* role)
{
	if (!role)
	{
		return;
	}
	PtcG2C_EnemyDorpDoodadNtf send;

	if (_FillEnemyDoodad(send, m_ServerModeDoodads))
	{
		role->Send(send);
	}
}

void SceneDoodad::_NotifyDoodadDisappear(const KKSG::EnemyDoodadInfo& arg)
{
	PtcG2C_FetchEnemyDoodadNtf send;
	KKSG::EnemyDoodadInfo* pInfo = send.m_Data.mutable_doodadinfo();
	*pInfo = arg;

	m_pScene->Broadcast(send);
}

void SceneDoodad::_NotifyDoodadDisappear(Role* role, const KKSG::EnemyDoodadInfo& arg/*, const KKSG::RollInfoRes& roRes*/)
{
	if (!role)
	{
		return;
	}
	PtcG2C_FetchEnemyDoodadNtf send;
	KKSG::EnemyDoodadInfo* pInfo = send.m_Data.mutable_doodadinfo();
	*pInfo = arg;

	m_pScene->Broadcast(send, role);

	KKSG::RollInfo* info = send.m_Data.add_rollinfos();
	info->set_roleid(role->GetID());
	info->set_rollvalue(100);
	role->Send(send);
}

void SceneDoodad::RandLeftDoodad()
{
	if (m_hasrandleft)
	{
		return;
	}
	m_hasrandleft = true;

	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);
	for (auto iter = roles.begin(); iter != roles.end();)
	{
		if ((*iter)->IsDead())
		{
			iter = roles.erase(iter);
		}
		else
			++iter;
	}
	if (roles.empty())
	{
		return;
	}
	foreach (i in m_ServerModeDoodads)
	{
		if (i->roleID)
		{
			continue;
		}
		Role* role = NULL;
		if (i->canPickRole)
		{
			Role* pickRole = RoleManager::Instance()->FindByRoleID(i->canPickRole);
			if (pickRole && pickRole->GetCurrScene() == m_pScene)
			{
				role = pickRole;
			}
		}
		else
		{
			UINT32 index = XRandom::randInt(0,roles.size());
			if (index >= roles.size())
			{
				continue;
			}
			role = roles[index];
		}
		if (!role)
		{
			continue;
		}

		KKSG::EnemyDoodadInfo arg;
		_ConvertToKKSG(*i, arg);

		KKSG::RollInfoRes roRes;
		FetchDoodad(role, arg, roRes, true);
	}
}

static bool DoodadWithNoRoleID(const PickServerModeDoodadInfo info)
{
	return info.roleID == 0;
}

static bool DoodadItemWithNoRoleID(const PickServerModeDoodadInfo info)
{
	return info.roleID == 0 && info.Type == 0;
}

bool SceneDoodad::IsDoodadNotPicked()
{
	return std::find_if(m_ServerModeDoodads.begin(), m_ServerModeDoodads.end(), DoodadWithNoRoleID) != m_ServerModeDoodads.end();
}

bool SceneDoodad::IsDoodadItemNotPicked()
{
	return std::find_if(m_ServerModeDoodads.begin(), m_ServerModeDoodads.end(), DoodadItemWithNoRoleID) != m_ServerModeDoodads.end();
}

bool SceneDoodad::IsRollDoodadItemNotPicked()
{
	std::vector<Role*> roles;
	m_pScene->GetPlayersNotHelper(roles);
	if (roles.empty())
	{
		return false;
	}
	for (auto iter = m_ServerModeDoodads.begin(); iter != m_ServerModeDoodads.end(); ++iter)
	{
		if(iter->roleID == 0 && iter->Type == 0)
		{
			ItemConf* pItemConfig = ItemConfig::Instance()->GetItemConf(iter->ItemID);
			if (!pItemConfig)
			{
				continue;
			}
			if (pItemConfig->ReqRoll <= iter->Count)
			{
				if (roles.size() > 1)
				{
					return true;
				}
				if (roles.size() == 1)
				{
					iter->roleID = roles[0]->GetID();
					_AddSyncSceneFetchReward(roles[0], iter->ItemID, iter->Count);
					_OnDoodadFetch(iter->waveID);
				}
			}
		}
	}

	m_RollList.clear();

	return false;
}

