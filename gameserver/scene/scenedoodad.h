#ifndef __SCENEDOODAD_H__
#define __SCENEDOODAD_H__

#include "math/XVector3.h"
#include "pb/project.pb.h"
#include "scene.h"

class Scene;
class Role;
class Enemy;
class PtcG2C_EnemyDorpDoodadNtf;

struct PickDoodadInfo
{
	UINT32 waveID;
	UINT32 Type;
	UINT32 ItemID;
	UINT32 Count;
	bool   IsPicked;

	PickDoodadInfo()
	{
		waveID = 0;
		Type = 0;
		ItemID = 0;
		Count = 0;
		IsPicked = false;
	}
};

struct PickServerModeDoodadInfo
{
	UINT32 id;
	UINT32 index;
	UINT64 roleID;
	UINT64 canPickRole;
	UINT32 waveID;
	UINT32 doodadID;
	UINT32 Type;
	UINT32 ItemID;
	UINT32 Count;
	bool IsBind;
	UINT32 TemplateID;
	Vector3 pos;
	bool bRand;
	UINT32 beginRollTime;
	std::map<UINT64, UINT32> m_rollsRes;
	std::map<UINT32, UINT32> seeds;
	bool bRewarded;
	PickServerModeDoodadInfo()
	{
		id = 0;
		index = 0;
		roleID = 0;
		canPickRole = 0;
		waveID = 0;
		doodadID = 0;
		Type = 0;
		ItemID = 0;
		Count = 0;
		IsBind = true;
		bRand = false;
		TemplateID = 0;
		beginRollTime = 0;
		m_rollsRes.clear();
		bRewarded = false;
	}
};

struct DroppableInfo
{
	DroppableInfo()
	{
		nonHelperCount = 0;
		totalCount = 0;
	}

	int nonHelperCount;
	int totalCount;
};

class CheckDoodadTimer;
class SceneDoodad
{
public:
	SceneDoodad(Scene* scene);
	~SceneDoodad();
	void Update();
	void Clear();
	void InitDroppableInfo(DroppableInfo& info);

	void SetPickedDoodad(UINT32 waveID);
	bool IsDoodadNotPicked();
	bool IsDoodadItemNotPicked();
	bool IsRollDoodadItemNotPicked();

	void FetchDoodad(Role* role, const KKSG::EnemyDoodadInfo& arg, KKSG::RollInfoRes& roRes, bool bRand=false);
	void ChooseRoll(Role* role, int type, const KKSG::EnemyDoodadInfo& doodadInfo/*, KKSG::RollInfoRes& roRes*/);
	void RandLeftDoodad();

	void FillDoodadInfo(KKSG::SceneCfg* pData);

	void GenerateDoodad(UINT32 doodadID, UINT32 waveID, Vector3& pos);
	void GenerateEnemyDroppedDoodad(Enemy* pEnemy, bool isLastEnemy);
	void GenerateEnemyPandoraDroppedDoodad(Enemy* pEnemy, Role* role);

	void NotifyDoodadAppearOnEnter(Role* role);

	void RemoveBuffDoodadById(int buffid);

	const std::list<PickDoodadInfo> & GetDoodadsInfo();
	std::list<PickServerModeDoodadInfo> & GetServerDoodadsInfo();
	void GetServerDoodadsInfo(int type, std::list<PickServerModeDoodadInfo>& listDoodad);

	KKSG::ErrorCode SpecialFetchHandle(Role* pRole, PickServerModeDoodadInfo& info);

protected:
	UINT32 _GetNextDoodadID(){return ++m_curDoodadID;}
	Vector3 _GenerateDoodadPos(Enemy* pEnemy, UINT32& entercount, bool positive);

	bool _CheckEnemyDoodadDroppable();
	int _FillEnemyDoodad(PtcG2C_EnemyDorpDoodadNtf& send, const std::list<PickServerModeDoodadInfo>& doodadList);

	void _NotifyAllNewDoodadAppear(Enemy* enemy, std::list<PickServerModeDoodadInfo>& doodadList);
	void _NotifyDoodadDisappear(Role* role, const KKSG::EnemyDoodadInfo& arg/*, const KKSG::RollInfoRes& roRes*/);
	void _NotifyDoodadDisappear(const KKSG::EnemyDoodadInfo& arg);

	void _OnDoodadFetch(int waveID);
	Role* _FetchDoodadRoll(KKSG::RollInfoRes& roRes);
	void _AddSyncSceneFetchReward(Role* role, int itemID, int itemCount);

	void _ConvertToKKSG(const PickServerModeDoodadInfo& info, KKSG::EnemyDoodadInfo& doodadInfo);

	bool _InitRollInfo(PickServerModeDoodadInfo* info);
	void _AddRollInfo(PickServerModeDoodadInfo* info);
	void _NotifyBeginRoll(PickServerModeDoodadInfo* info);
	bool _IsRollEnd(PickServerModeDoodadInfo& info);
	void _ReRoll(PickServerModeDoodadInfo* info, std::vector<Role*>& roles);
	Role* _GetRollMaxRole(PickServerModeDoodadInfo& info, bool& isAllSelected, UINT32& max);

private:
	bool m_hasrandleft;
	UINT32 m_curDoodadID;
	Scene* m_pScene;
	CheckDoodadTimer* m_doodadTimer;
	DroppableInfo* m_droppableInfo;

	std::map<int, std::vector<XLevelDoodad>> m_staticDropDoodad;

	std::list<PickDoodadInfo> m_Doodads;
	///> Áª»ú³¡¾° É±¹Öµô±¦Ïä
	std::list<PickServerModeDoodadInfo> m_ServerModeDoodads;
	std::list<PickServerModeDoodadInfo*> m_RollList;
};


#endif // __SCENEDOODAD_H__
