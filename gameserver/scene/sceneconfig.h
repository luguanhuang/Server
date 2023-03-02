#ifndef __SCENECONFIG_H__
#define __SCENECONFIG_H__

#include "math/XVector3.h"
#include "table/SceneTable.h"
#include "table/SceneConsumeTable.h"
#include "table/KillMonsterScoreTable.h"
#include "table/SceneMorphTable.h"

typedef SceneTable::RowData SceneConf;

enum SyncModeDef
{
	SOLOMODE = 0,   ///> 纯单机刷怪
	SERVERMODE = 1, ///> 服务器刷怪
	// remove arena
	//MIXMODE = 2,    ///> 服务器把刷怪数据给客户端，客户端刷怪
};

enum MorphCondition
{
	MORPH_WEEKDAY = 1,
	MORPH_CONDITION_MAX,
};

struct EnterSceneInfo
{
	bool isenterhall;
	bool isenterpvp;
	UINT32 sceneType;

	EnterSceneInfo()
	{
		Reset();
	}

	void Reset()
	{
		isenterhall = false;
		isenterpvp = false;
		sceneType = 0;
	}
};

class Grid;
struct BuffDesc;
struct SceneInfo
{
	Grid* m_pGrid;
	SceneConf* m_pConf;

	bool m_IsEndofChapter;
	std::string m_strSpawnConfig;

	Vector3 m_StartRot;
	std::vector<Vector3> m_StartPos;
	float m_face;
	std::vector<float> m_StartFace;

	std::vector<BuffDesc> m_buffs;

	float GetStartFace(UINT32 count);
	Vector3 GetStartPos(UINT32 count, bool rand = false);
	int GetRandStartPos(Vector3 &refPos);
	float GetHurtCoef() { return m_pConf->HurtCoef; }
	UINT32 GetSelectBoxTime() { return m_pConf->SelectBoxTime[0] == 0 ? 20 : m_pConf->SelectBoxTime[0]; }
	UINT32 GetSelectBoxEndTime(){ return m_pConf->SelectBoxTime[1] == 0 ? 10 : m_pConf->SelectBoxTime[1]; }
};

struct ChapterChestInfo
{
	int startNeed;
	int dropID;
};

typedef void (*TravelCallback)(UINT32 sceneID, SceneInfo *pSceneInfo, void *arg);

class Role;
struct BoxRewardInfo;
struct RoleBoxRewardInfo;
class SceneConfig
{
	SceneConfig();
	~SceneConfig();
	DECLARE_SINGLETON(SceneConfig)
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	SceneInfo *FindSceneInfo(UINT32 sceneTemplateID);

	bool IsSceneValid(UINT32 type);
	bool IsSpawnExcept(UINT32 type);
	bool IsPrepareExcept(UINT32 type);
	bool IsBattleSceneExcept(UINT32 type);
	bool IsHasReadyScene(UINT32 type);
	bool IsMsCreateScene(UINT32 type);
	bool IsPVPScene(UINT32 type);
	bool IsPVPAttr(UINT32 type);
	bool IsMultiBattle(UINT32 type);
	bool IsChangeFightGroupScene(UINT32 type);
	bool IsCollideExcept(UINT32 type);
	bool IsPosRandExcept(UINT32 type);
	bool IsTransScene(UINT32 type);

	void MarkPvpAttr(UINT32 type);
	
	UINT32 GetSceneGroupID(UINT32 sceneTemplateID);
	UINT32 GetSceneGroupID(SceneInfo *pSceneInfo);
	bool IsCostFatigue(UINT32 scenetype, bool iswin);
	bool IsCostCount(UINT32 scenetype, bool iswin);
	bool IsCoolDown(UINT32 scenetype, bool iswin);
	bool CheckPreScene(Role* role, std::vector<int>& scenes);
	bool GetBox(UINT32 star, SceneInfo* info, UINT64 roleid, UINT32 rolelv, RoleBoxRewardInfo& boxinfo);
	UINT32 GetBoxDropId(SceneInfo* info, UINT32 box);
	void FillBox(UINT32 type, UINT32 dropid, UINT32 rolelv, BoxRewardInfo& box);

	std::vector<Sequence<UINT32, 2>>* GetBoxConf(UINT32 star, SceneInfo* info);

	bool CanFollowPet(UINT32 sceneTemplateID);

	ChapterChestInfo *GetChapterChestInfo(UINT32 chapterID, UINT32 chestID);
	UINT32 GetChapterChestNum(UINT32 chapterID);
	std::vector<UINT32> *GetChapterScene(UINT32 chapterID);
	std::vector<int>* GetDrop(SceneInfo* sceneinfo, int rank);
	std::vector<Sequence<UINT32, 2>>* GetExpSealReward(SceneInfo* sceneinfo);
	std::vector<BuffDesc>* GetEnterSceneBuff(UINT32 scenetemplateid);

	EnterSceneInfo GetEnterSceneInfo(UINT32 type, UINT32 scenetemplateid);
	bool IsSoloScene(UINT32 scenetemplateid);

	void TravelAllSceneInfo(TravelCallback cb, void *arg);

	KillMonsterScoreTable& GetKillMonsterScoreTable();
	int CalcKillMonsterSocre(UINT32 sceneID, const std::map<UINT32, UINT32>& killInfo);

	int GetMorphTime(UINT32 sceneTemplateID);
	const SceneMorphTable::RowData* GetMorphTableByID(UINT32 sceneID);
	
	void GetStageUntil(UINT32 sceneID, std::vector<UINT32>& sceneIDs, std::vector<UINT32>& chapterIDs);

	const std::unordered_map<UINT32, SceneInfo>& GetAllSceneConf(){return m_SceneInfos;}

	bool _CheckDefiniteDrop(UINT64 roleID, UINT32& quality, UINT32 scenetype, bool resetTimes);

private:
	SceneTable m_oTable;
	SceneConsumeTable m_oConsumeTable;
	KillMonsterScoreTable m_oKillMonsterScoreTable;
	SceneMorphTable m_sceneMorphTable;

	UINT32 m_feature[KKSG::SceneType_MAX + 1];

	std::unordered_map<UINT32, SceneInfo> m_SceneInfos;
	// chapter
	std::unordered_map<UINT32, std::vector<UINT32>> m_ChapterSceneID;
	std::unordered_map<UINT32, ChapterChestInfo> m_ChapterChest;
};

#endif // __SCENECONFIG_H__