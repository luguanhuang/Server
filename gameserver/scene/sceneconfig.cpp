#include "pch.h"
#include "sceneconfig.h"
#include "grid.h"
#include "table/globalconfig.h"
#include "table/XChapter.h"
#include "pb/project.pb.h"
#include "foreach.h"
#include "unit/role.h"
#include "gamelogic/stagemgr.h"
#include "util/XCommon.h"
#include "gamelogic/teamreward.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/stagerankmgr.h"
#include "config/itemdropconfig.h"
#include "buff/XBuffTemplateManager.h"
#include "util/gametime.h"
#include "gamelogic/worldlevelmgr.h"
#include "gamelogic/nestconfig.h"
#include "gamelogic/levelsealMgr.h"
#include "unit/rolemanager.h"
#include "gamelogic/dragonrecord.h"

Vector3 SceneInfo::GetStartPos(UINT32 count, bool rand/* = false*/)
{
	UINT32 index = count % m_StartPos.size();
	Vector3 position = m_StartPos[index];
	
	if(rand)
	{
		position.x += (float)(XRandom::randDouble(-1.3, 1.3));
		position.z += (float)(XRandom::randDouble(-1.3, 1.3));
	}

	return position;
}


int SceneInfo::GetRandStartPos(Vector3 &refPos)
{
	if (m_StartPos.empty() == true)
	 return -1;
	UINT32 index = XRandom::randInt (0,m_StartPos.size());
	refPos.x = m_StartPos[index].x + (float)(XRandom::randDouble(-1.3, 1.3));
	refPos.z = m_StartPos[index].z + (float)(XRandom::randDouble(-1.3, 1.3));
	return index;
}




float SceneInfo::GetStartFace(UINT32 count)
{
	if(!m_StartFace.empty())
	{
		UINT32 index = count % m_StartFace.size();
		return m_StartFace[index];
	}
	if (count % 2 != 0)	
	{
		return (m_face - 180.0f);
	}
	return m_face;	
}

//////////////////////////////////////////////////////////////////////////////////////
INSTANCE_SINGLETON(SceneConfig)

SceneConfig::SceneConfig()
{

}

SceneConfig::~SceneConfig()
{

}

enum
{
	///> 合法场景
	Scene_Valid = 0,
	///> 非刷怪脚本场景
	Scene_SpawnExcept = 1,
	///> 不用相互等待
	Scene_PrepareExcept = 2,
	///> 有准备阶段的场景
	Scene_Ready = 3,
	///> 非战斗场景
	Scene_FightGroupExcept = 4,
	///> MS场景的场景
	Scene_MSCreate = 5,
	///> 多人分线战斗场景
	Scene_MultiBattle = 6,
	///> PVP场景
	Scene_PVPScene = 7,
	///> PVP战斗过程中变阵营关系
	Scene_ChangeFightGroup = 8,
	///> PVP公平模式
	Scene_PVPAttr = 9,
};

#define SceneValidShift				(1 << Scene_Valid)
#define SceneSpawnExcept			(1 << Scene_SpawnExcept)
#define ScenePrepareExcept			(1 << Scene_PrepareExcept)
#define ScenePVPScene				(1 << Scene_PVPScene)
#define SceneReady					(1 << Scene_Ready | 1 << Scene_PVPScene | 1 << Scene_PrepareExcept)
#define SceneFightGroupExcept		(1 << Scene_FightGroupExcept | 1 << Scene_SpawnExcept | 1 << Scene_PrepareExcept)
#define SceneMSCreate				(1 << Scene_MSCreate)
#define SceneMultiBattle			(1 << Scene_MultiBattle)
#define SceneChangeFightGroup		(1 << Scene_ChangeFightGroup)
#define ScenePVPAttr				(1 << Scene_PVPAttr)

bool SceneConfig::Init()
{
	memset(m_feature, 0, sizeof(m_feature));

	///> 非战斗场景
	m_feature[KKSG::SCENE_HALL]				= SceneValidShift | SceneFightGroupExcept;
  	m_feature[KKSG::SCENE_GUILD_HALL]		= SceneValidShift | SceneFightGroupExcept | SceneMSCreate;
  	m_feature[KKSG::SCENE_FAMILYGARDEN]		= SceneValidShift | SceneFightGroupExcept | SceneMSCreate;
  	m_feature[KKSG::SKYCITY_WAITING]		= SceneValidShift | SceneFightGroupExcept;
	m_feature[KKSG::SCENE_CASTLE_WAIT]      = SceneValidShift | SceneFightGroupExcept;
  	m_feature[KKSG::SCENE_HORSE]			= SceneValidShift | SceneFightGroupExcept;
	m_feature[KKSG::SCENE_WEDDING]			= SceneValidShift | (1 << Scene_FightGroupExcept);

	m_feature[KKSG::SCENE_LEISURE]			= SceneValidShift | (1 << Scene_FightGroupExcept) | 1 << Scene_PrepareExcept | SceneMSCreate;

	///> 通用场景
  	m_feature[KKSG::SCENE_BATTLE]			= SceneValidShift;
  	m_feature[KKSG::SCENE_CALLBACK]			= SceneValidShift;
  	m_feature[KKSG::SCENE_ABYSSS]			= SceneValidShift;
  	m_feature[KKSG::SCENE_NEST]				= SceneValidShift;
  	m_feature[KKSG::SCENE_ACTIVITY_ONE]		= SceneValidShift;
  	m_feature[KKSG::SCENE_ACTIVITY_TWO]		= SceneValidShift;
  	m_feature[KKSG::SCENE_ACTIVITY_THREE]	= SceneValidShift;
	m_feature[KKSG::SCENE_BIOHELL]			= SceneValidShift;
	m_feature[KKSG::SCENE_DUCK]				= SceneValidShift;

	///> 玩法场景
  	m_feature[KKSG::SCENE_TOWER]			= SceneValidShift;	
  	m_feature[KKSG::SCENE_DRAGON]			= SceneValidShift; 
  	m_feature[KKSG::SCENE_COMPETEDRAGON]	= SceneValidShift; 
  	m_feature[KKSG::SCENE_GODDESS]			= SceneValidShift; 
  	m_feature[KKSG::SCENE_DRAGON_EXP]		= SceneValidShift; 
  	m_feature[KKSG::SCENE_ENDLESSABYSS]		= SceneValidShift; 
  	m_feature[KKSG::SCENE_RISK]				= SceneValidShift; 
  	m_feature[KKSG::SCENE_RESWAR_PVE]		= SceneValidShift; 
  	m_feature[KKSG::SCENE_WEEK_NEST]		= SceneValidShift;	
  	m_feature[KKSG::SCENE_GUILD_CAMP]		= SceneValidShift; 
  	m_feature[KKSG::SCENE_BOSSRUSH]			= SceneValidShift; 
  	m_feature[KKSG::SCENE_COUPLE]			= SceneValidShift;	

	///> 其他
  	//m_feature[KKSG::SCENE_ARENA]			= SceneValidShift; 
  	m_feature[KKSG::SCENE_AIRSHIP]			= SceneValidShift;
  	m_feature[KKSG::SCENE_PROF_TRIALS]		= SceneValidShift; 

	///> 多人分线战斗场景
  	m_feature[KKSG::SCENE_WORLDBOSS]		= SceneValidShift | SceneMultiBattle | ScenePrepareExcept | SceneMSCreate;
  	m_feature[KKSG::SCENE_GUILD_BOSS]		= SceneValidShift | SceneMultiBattle | ScenePrepareExcept | SceneMSCreate;

	///> pk相关场景
  	m_feature[KKSG::SCENE_CUSTOMPK]			= SceneValidShift | SceneReady | SceneSpawnExcept;
  	m_feature[KKSG::SCENE_PK]				= SceneValidShift | SceneReady | SceneSpawnExcept;
  	m_feature[KKSG::SCENE_PKTWO]			= SceneValidShift | SceneReady | SceneSpawnExcept;
  	m_feature[KKSG::SCENE_CUSTOMPKTWO]		= SceneValidShift | SceneReady | SceneSpawnExcept;
  	m_feature[KKSG::SKYCITY_FIGHTING]		= SceneValidShift | SceneReady | SceneSpawnExcept;
  	m_feature[KKSG::SCENE_RESWAR_PVP]		= SceneValidShift | SceneReady | SceneSpawnExcept | ScenePVPAttr;
  	m_feature[KKSG::SCENE_PVP]				= SceneValidShift | SceneReady;
  	m_feature[KKSG::SCENE_HORSE_RACE]		= SceneValidShift | SceneReady;
  	m_feature[KKSG::SCENE_HEROBATTLE]		= SceneValidShift | SceneReady;
  	m_feature[KKSG::SCENE_INVFIGHT]			= SceneValidShift | SceneReady;
  	m_feature[KKSG::SCENE_MOBA]				= SceneValidShift | SceneReady;

  	m_feature[KKSG::SCENE_GMF]				= SceneValidShift | ScenePVPScene | ScenePrepareExcept | SceneChangeFightGroup | SceneSpawnExcept;
  	m_feature[KKSG::SCENE_GPR]				= SceneValidShift | ScenePVPScene | ScenePrepareExcept | SceneChangeFightGroup;

  	m_feature[KKSG::SCENE_CASTLE_WAIT]		= SceneValidShift | ScenePVPScene | ScenePrepareExcept;
  	m_feature[KKSG::SCENE_CASTLE_FIGHT]		= SceneValidShift | ScenePVPScene | ScenePrepareExcept;

  	m_feature[KKSG::SCENE_SURVIVE]			= SceneValidShift | ScenePVPScene | ScenePrepareExcept;

  	m_feature[KKSG::SCENE_LEAGUE_BATTLE]	= SceneValidShift | ScenePVPScene | ScenePrepareExcept | SceneChangeFightGroup;

	return LoadFile();
}

void SceneConfig::Uninit()
{	
	ClearFile();
	Grid::DestoryAllGrid();
}

bool SceneConfig::CheckFile()
{
	SceneTable tmpSceneTable;
	if (!tmpSceneTable.LoadFile("table/SceneList.txt"))
	{
		LogWarn("load scene config [table/SceneList.txt] failed!");
		return false;
	}

	SceneConsumeTable tmpSceneConsumeTable;
	if (!tmpSceneConsumeTable.LoadFile("table/SceneConsume.txt"))
	{
		SSWarn<<"load table/SceneConsume.txt failed"<<END;
		return false;
	}

	XChapter tmpXChapter;
	if (!tmpXChapter.LoadFile("table/ChapterList.txt"))
	{
		SSWarn << "load table/ChapterList.txt failed"<<END;
		return false;
	}

	KillMonsterScoreTable tempMosterScoreTable;
	if (!tempMosterScoreTable.LoadFile("table/MonsterScore.txt"))
	{
		SSWarn << "load table/MonsterScore.txt failed"<<END;
		return false;
	}

	return true;
}

bool SceneConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	SceneTable tmpSceneTable;
	tmpSceneTable.LoadFile("table/SceneList.txt");
	m_oTable.CopyFrom(&tmpSceneTable);

	int prevChapter = 1;
	int prevScene = -1;
	m_ChapterSceneID.clear();
	UINT32 n = m_oTable.Table.size();
	for (UINT32 i = 0; i < n; ++i)
	{
		SceneInfo oInfo;
		SceneTable::RowData &row = *m_oTable.Table[i];
		oInfo.m_pConf = m_oTable.Table[i];

		oInfo.m_strSpawnConfig = row.configFile;

		// pos
		oInfo.m_face = (float)row.StartRot[1];
		oInfo.m_StartRot = Vector3(row.StartRot[0], row.StartRot[1], row.StartRot[2]);
		oInfo.m_StartFace = row.StartFace;
		oInfo.m_StartPos.clear();
		for (auto j = row.StartPos.begin(); j != row.StartPos.end(); ++j)
		{
			oInfo.m_StartPos.push_back(Vector3((*j)[0], (*j)[1], (*j)[2]));
		}

		// grid
		oInfo.m_pGrid = NULL;
		if (!row.BlockFilePath.empty())
		{
			std::string blockpath = row.BlockFilePath;
			int start = blockpath.find("T");
			if(std::string::npos != (size_t)start)
			{
				blockpath.replace(start, 1, "t");	
			}
			else
			{
				LogWarn("line:%u, Table's 'T' is lower case 't', should be upper case 'T'", i+3);
			}
			oInfo.m_pGrid = Grid::GetGrid(blockpath + ".bytes");
		}

		// chapter
		oInfo.m_IsEndofChapter = 0;
		if (row.Chapter != prevChapter && prevScene != -1)
		{
			m_SceneInfos[prevScene].m_IsEndofChapter = true;
		}
		m_ChapterSceneID[row.Chapter].push_back(row.id);

		prevChapter = row.Chapter;
		prevScene = row.id;

		// buff
		if (!row.Buff.empty())
		{
			for (auto i = row.Buff.begin(); i != row.Buff.end(); ++i)
			{
				oInfo.m_buffs.push_back(BuffDesc(i->seq[0], i->seq[1]));
			}
		}

		m_SceneInfos[row.id] = oInfo;
	}

	m_SceneInfos[prevScene].m_IsEndofChapter = true;	


	XChapter chapterTable;
	chapterTable.LoadFile("table/ChapterList.txt");
	m_ChapterChest.clear();
	for (UINT32 i = 0; i < chapterTable.Table.size(); ++i)
	{
		XChapter::RowData *pRow = chapterTable.Table[i];
		UINT32 key = (pRow->ChapterID << 8);
		for (UINT32 j = 0; j < pRow->Drop.size(); ++j)
		{
			ChapterChestInfo info;
			info.startNeed = pRow->Drop[j].seq[0];
			info.dropID = pRow->Drop[j].seq[1];
			m_ChapterChest[key|(1<<j)] = info;
		}
	}

	m_oConsumeTable.Clear();
	m_oConsumeTable.LoadFile("table/SceneConsume.txt");

	m_oKillMonsterScoreTable.Clear();
	m_oKillMonsterScoreTable.LoadFile("table/MonsterScore.txt");

	m_sceneMorphTable.Clear();
	m_sceneMorphTable.LoadFile("table/SceneMorph.txt");

	return true;
}

void SceneConfig::ClearFile()
{
	m_SceneInfos.clear();
	m_ChapterSceneID.clear();
	m_ChapterChest.clear();
	m_oTable.Clear();
	m_oConsumeTable.Clear();
	m_oKillMonsterScoreTable.Clear();
	m_sceneMorphTable.Clear();
}

SceneInfo * SceneConfig::FindSceneInfo(UINT32 sceneTemplateID)
{
	auto i = m_SceneInfos.find(sceneTemplateID);
	return i == m_SceneInfos.end() ? NULL : &i->second;
}

void SceneConfig::TravelAllSceneInfo(TravelCallback cb, void *arg)
{
	for (auto i = m_SceneInfos.begin(); i != m_SceneInfos.end(); ++i)
	{
		cb(i->first, &i->second, arg);
	}
}

KillMonsterScoreTable& SceneConfig::GetKillMonsterScoreTable()
{
	return m_oKillMonsterScoreTable;
}

int SceneConfig::CalcKillMonsterSocre(UINT32 sceneID, const std::map<UINT32, UINT32>& killInfo)
{
	auto data = m_oKillMonsterScoreTable.GetBysceneID(sceneID);
	if (!data)
	{
		return 0;
	}
	int score = 0;
	for (auto iter = data->monsterScore.begin(); iter != data->monsterScore.end(); ++iter)
	{
		auto find = killInfo.find(iter->seq[0]);
		if (find != killInfo.end())
		{
			score += find->second * iter->seq[1];
		}
	}

	return score;
}

int SceneConfig::GetMorphTime(UINT32 sceneTemplateID)
{
	auto data = m_sceneMorphTable.GetBySceneID(sceneTemplateID);
	if (!data)
	{
		return 0;
	}

	if(!GSConfig::Instance()->IsCrossGS())
	{
		UINT32 type = GetGlobalConfig().PVEMorphLevelSealType;
		if (CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType < type)
		{
			return 0;
		}
	}

	bool canMorph = false;
	int morphTime = 0;
	for(int i = 0; i < data->MorphCondition.size(); ++i)
	{
		int con = data->MorphCondition[i][0];
		if (con > MORPH_CONDITION_MAX || con < MORPH_WEEKDAY)
		{
			continue;
		}
		int value = data->MorphCondition[i][1];
		switch(con)
		{
		case MORPH_WEEKDAY:
			{
				int now = GameTime::GetTime();
				int weekday = XCommon::GetWeekNum(now);
				if (value == weekday)
				{
					canMorph = true;
				}
			}
			break;
		default:
			break;
		}
		if (canMorph)
		{
			morphTime = data->MorphTime;
			break;
		}
	}

	return morphTime;
}

const SceneMorphTable::RowData* SceneConfig::GetMorphTableByID(UINT32 sceneID)
{
	return m_sceneMorphTable.GetBySceneID(sceneID);
}

UINT32 SceneConfig::GetSceneGroupID(SceneInfo *pSceneInfo)
{
	if (pSceneInfo->m_pConf->DayLimitGroupID != 0)
	{
		return pSceneInfo->m_pConf->DayLimitGroupID;
	}
	return pSceneInfo->m_pConf->id;
}

UINT32 SceneConfig::GetSceneGroupID(UINT32 sceneTemplateID)
{
	SceneInfo* info = FindSceneInfo(sceneTemplateID);
	if(info)
	{
		return GetSceneGroupID(info);
	}
	return 0;
}

bool SceneConfig::IsCostFatigue(UINT32 scenetype, bool iswin)
{
	SceneConsumeTable::RowData* rowdata = m_oConsumeTable.GetBySceneType(scenetype);
	if (NULL == rowdata)
	{
		return true;
	}
	if (0 == rowdata->SceneCost || 1 == rowdata->SceneCost)
	{
		return (iswin == (1 == rowdata->SceneCost));	
	}
	return true;
}

bool SceneConfig::IsCostCount(UINT32 scenetype, bool iswin)
{
	SceneConsumeTable::RowData* rowdata = m_oConsumeTable.GetBySceneType(scenetype);
	if (NULL == rowdata)
	{
		return true;
	}
	if (0 == rowdata->SceneCount || 1 == rowdata->SceneCount)
	{
		return (iswin == (1 == rowdata->SceneCount));	
	}
	return true;
}

bool SceneConfig::IsCoolDown(UINT32 scenetype, bool iswin)
{
	SceneConsumeTable::RowData* rowdata = m_oConsumeTable.GetBySceneType(scenetype);
	if (NULL == rowdata)
	{
		return true;
	}
	if (0 == rowdata->SceneCoolDown || 1 == rowdata->SceneCoolDown)
	{
		return (iswin == (1 == rowdata->SceneCoolDown));	
	}
	return true;
}

std::vector<UINT32> * SceneConfig::GetChapterScene(UINT32 chapterID)
{
	auto it = m_ChapterSceneID.find(chapterID);
	return (it == m_ChapterSceneID.end()) ? NULL : &it->second;
}

ChapterChestInfo *SceneConfig::GetChapterChestInfo(UINT32 chapterID, UINT32 chestID)
{
	auto it = m_ChapterChest.find((chapterID << 8)|(1<<chestID));
	return (it == m_ChapterChest.end()) ? NULL : &it->second;
}

UINT32 SceneConfig::GetChapterChestNum(UINT32 chapterID)
{
	UINT32 num = 0;
	for (auto it = m_ChapterChest.begin(); it != m_ChapterChest.end(); ++it)
	{
		UINT32 value = it->first;
		if ((value >> 8) == chapterID)
		{
			num++;
		}
	}
	return num;
}

bool SceneConfig::IsSceneValid(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_Valid)) > 0);
}

bool SceneConfig::IsSpawnExcept(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_SpawnExcept)) > 0);
}

bool SceneConfig::IsPrepareExcept(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_PrepareExcept)) > 0);
}

bool SceneConfig::IsBattleSceneExcept(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_FightGroupExcept)) > 0);
}

bool SceneConfig::IsHasReadyScene(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_Ready)) > 0);
}

bool SceneConfig::IsMsCreateScene(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_MSCreate)) > 0);
}

bool SceneConfig::IsMultiBattle(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_MultiBattle)) > 0);
}

bool SceneConfig::IsPVPScene(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_PVPScene)) > 0);
}

bool SceneConfig::IsTransScene(UINT32 type)
{
	return GetGlobalConfig().TransSceneType.end() != GetGlobalConfig().TransSceneType.find(type);
}

void SceneConfig::MarkPvpAttr(UINT32 type)
{
	if (type > KKSG::SceneType_MAX)
	{
		return;
	}
	m_feature[type] |= (1 << Scene_PVPAttr);
}

bool SceneConfig::IsPVPAttr(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_PVPAttr)) > 0);
}

bool SceneConfig::IsChangeFightGroupScene(UINT32 type)
{
	return (type > KKSG::SceneType_MAX) ? false : ((m_feature[type] & (1 << Scene_ChangeFightGroup)) > 0);
}

bool SceneConfig::IsCollideExcept(UINT32 type)
{
	return (KKSG::SCENE_PK == type || KKSG::SCENE_PKTWO == type || KKSG::SCENE_CUSTOMPK == type || KKSG::SCENE_CUSTOMPKTWO == type || KKSG::SCENE_PVP == type || KKSG::SCENE_INVFIGHT == type);
}

bool SceneConfig::IsPosRandExcept(UINT32 type)
{
	return (KKSG::SCENE_PK == type || KKSG::SCENE_CUSTOMPK == type || KKSG::SCENE_INVFIGHT == type);
}

bool SceneConfig::CheckPreScene(Role* role, std::vector<int>& scenes)
{
	if (!scenes.empty())
	{
		for (auto i = scenes.begin(); i != scenes.end(); ++i)
		{
			if (role->Get<StageMgr>()->GetRank(*i) <= 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool SceneConfig::CanFollowPet(UINT32 sceneTemplateID)
{
	auto i = m_SceneInfos.find(sceneTemplateID);
	if (i != m_SceneInfos.end() && i->second.m_pConf != NULL)
	{
		return i->second.m_pConf->DisplayPet;
	}
	return false;
}

std::vector<int>* SceneConfig::GetDrop(SceneInfo* sceneinfo, int rank)
{
	std::vector<int> *pDrop = NULL;
	UINT32 type = sceneinfo->m_pConf->type;
	if (type == KKSG::SCENE_BATTLE || type == KKSG::SCENE_ABYSSS || type == KKSG::SCENE_ENDLESSABYSS 
		|| type == KKSG::SCENE_GODDESS || type == KKSG::SCENE_ACTIVITY_ONE || type == KKSG::SCENE_ACTIVITY_THREE  
		|| type == KKSG::SCENE_BIOHELL || type == KKSG::SCENE_DUCK || type == KKSG::SCENE_COUPLE)
	{
		///> 普通关卡和深渊评级是1.2.3星
		switch(rank)
		{
		case RANK_S:    pDrop = &sceneinfo->m_pConf->Drop1;break;
		case RANK_SS_1: pDrop = &sceneinfo->m_pConf->Drop2;break;
		case RANK_SS_2: pDrop = &sceneinfo->m_pConf->Drop2;break;
		case RANK_SSS:  pDrop = &sceneinfo->m_pConf->Drop3;break;
		default:break;
		}
	}
	else if (type == KKSG::SCENE_NEST || type == KKSG::SCENE_WEEK_NEST || type == KKSG::SCENE_DRAGON || type == KKSG::SCENE_COMPETEDRAGON
		|| type == KKSG::SCENE_ACTIVITY_TWO)
	{
		///> 龙本和巢穴(包括竞技巢穴)
		switch(rank)
		{
		case RANK_B: pDrop = &sceneinfo->m_pConf->Drop1;break;
		case RANK_A: pDrop = &sceneinfo->m_pConf->Drop2;break;
		case RANK_S: pDrop = &sceneinfo->m_pConf->Drop3;break;
		case RANK_SS_1: pDrop = &sceneinfo->m_pConf->Drop4;break;
		case RANK_SS_2: pDrop = &sceneinfo->m_pConf->Drop4;break;
		case RANK_SSS:  pDrop = &sceneinfo->m_pConf->Drop5;break;
		default:break;
		}
	}
	return pDrop;
}

std::vector<Sequence<UINT32, 2>>* SceneConfig::GetExpSealReward(SceneInfo* sceneinfo)
{
	return &sceneinfo->m_pConf->ExpSealReward;
}

std::vector<BuffDesc>* SceneConfig::GetEnterSceneBuff(UINT32 scenetemplateid)
{
	SceneInfo* info = FindSceneInfo(scenetemplateid);
	if (NULL == info)
	{
		return NULL;
	}
	else
	{
		return &info->m_buffs;
	}
}

bool SceneConfig::IsSoloScene(UINT32 scenetemplateid)
{
	SceneInfo* info = FindSceneInfo(scenetemplateid);
	if (NULL == info)
	{
		return false;
	}
	return info->m_pConf->syncMode != SERVERMODE;
}

EnterSceneInfo SceneConfig::GetEnterSceneInfo(UINT32 type, UINT32 scenetemplateid)
{
	EnterSceneInfo info;
	if (type == KKSG::ENTER_SCENE_SELECT_ROLE)
	{
		info.isenterhall = true;
	}
	else
	{
		///> 非战斗场景 || 单机
		SceneInfo* conf = FindSceneInfo(scenetemplateid);
		if (NULL != conf)
		{
			if (IsBattleSceneExcept(conf->m_pConf->type) || IsSoloScene(scenetemplateid))
			{
				info.isenterhall = true;
			}
			else
			{
				///> pvp
				info.isenterpvp = (conf->m_pConf->CombatType == 1);
			}
			info.sceneType = (UINT32)conf->m_pConf->type;
		}
	}
	return info;
}

void SceneConfig::GetStageUntil(UINT32 sceneID, std::vector<UINT32>& sceneIDs, std::vector<UINT32>& chapterIDs)
{
	SceneInfo* pInfo = FindSceneInfo(sceneID);
	if (pInfo == NULL)
	{
		return;
	}

	UINT32 chapter = -1;
	for (size_t i = 0; i < m_oTable.Table.size(); ++i)
	{
		SceneTable::RowData* pData = m_oTable.Table[i];
		if (pInfo->m_pConf->type != pData->type)
		{
			continue;
		}
		if (NestConfig::Instance()->IsStarNestStart(pData->id))
		{
			continue;
		}

		if (pData->Chapter != 0 && pData->Chapter != chapter)
		{
			chapter = pData->Chapter;
			chapterIDs.push_back(chapter);
		}

		sceneIDs.push_back(pData->id);
		if (pData->id == sceneID)
		{
			break;
		}
	}
}

bool SceneConfig::_CheckDefiniteDrop(UINT64 roleID, UINT32& quality, UINT32 scenetype, bool resetTimes)
{
	if (!roleID)
	{
		return false;
	}

	Role* role = RoleManager::Instance()->FindByRoleID(roleID);
	if (!role)
	{
		return false;
	}

	if (scenetype == KKSG::SCENE_DRAGON)
	{
		return role->Get<CDragonRecord>()->CheckDefiniteDrop(quality, resetTimes);
	}

	return false;
}

std::vector<Sequence<UINT32, 2>>* SceneConfig::GetBoxConf(UINT32 star, SceneInfo* info)
{
	switch (star)
	{
	case 1:
		return &info->m_pConf->SBox;
	case 2:
		return &info->m_pConf->SSBox;
	case 3:
		return &info->m_pConf->SSSBox;
	default:
		return &info->m_pConf->SBox;
	}
}

bool SceneConfig::GetBox(UINT32 star, SceneInfo* info, UINT64 roleid, UINT32 rolelv, RoleBoxRewardInfo& boxinfo)
{
	// get box through star
	std::vector<Sequence<UINT32, 2>> conf;
	switch (star)
	{
	case 1:
		conf = info->m_pConf->SBox;
		break;
	case 2:
		conf = info->m_pConf->SSBox;
		break;
	case 3:
		conf = info->m_pConf->SSSBox;
		break;
	default:
		conf = info->m_pConf->SBox;
	}
	// random one box
	UINT32 all = 0;
	for (auto i = conf.begin(); i != conf.end(); ++i)
	{
		all += i->seq[1];
	}
	UINT32 boxid = 1;

	std::deque<UINT32> types;
	if (_CheckDefiniteDrop(roleid, boxid, info->m_pConf->type, false))
	{
		for (int i = 1; i <= 4; ++i)
		{
			if (i != boxid)
			{
				types.push_back(i);
			}
		}
	}
	else
	{
		UINT32 allrandom = 0;
		UINT32 random = XRandom::randInt(1, all + 1);
		bool isin = false;
		for (auto j = conf.begin(); j != conf.end(); ++j)
		{
			allrandom += j->seq[1];
			if (!isin && random <= allrandom)
			{
				boxid = j->seq[0];
				isin = true;
			}
			else
			{
				types.push_back(j->seq[0]);
			}
		}
	}
	
	_CheckDefiniteDrop(roleid, boxid, info->m_pConf->type, true);

	// fill info
	std::random_shuffle(types.begin(), types.end());
	types.push_front(boxid);
	for (auto k = types.begin(); k != types.end(); ++k)
	{
		BoxRewardInfo tempbox;
		FillBox(*k, GetBoxDropId(info, *k), rolelv, tempbox);
		boxinfo.boxs.push_back(tempbox);
	}
	return true;
}

UINT32 SceneConfig::GetBoxDropId(SceneInfo* info, UINT32 box)
{
	UINT32 dropid = 0;
	switch (box)
	{
	case 1:
		dropid = info->m_pConf->CopperDropID;
		break;
	case 2:
		dropid = info->m_pConf->SilverDropID;
		break;
	case 3:
		dropid = info->m_pConf->GoldDropID;
		break;
	case 4:
		dropid = info->m_pConf->DiamondDropID;
		break;
	default:
		dropid = info->m_pConf->CopperDropID;
		break;
	}
	return dropid;
}

void SceneConfig::FillBox(UINT32 type, UINT32 dropid, UINT32 rolelv, BoxRewardInfo& box)
{
	box.type = type;
	const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(dropid, rolelv);
	if (NULL != item)
	{
		box.item = *item;
	}
}


