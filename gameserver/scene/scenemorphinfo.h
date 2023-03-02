#ifndef __SCENEMORPH_H__
#define __SCENEMORPH_H__

#include "scene.h"

enum MorphState
{
	MORPH_CHOOSE = 0,
	MORPH_CHOOSEOVER,
};

struct MorphRoleData
{
	MorphRoleData()
	{
		roleid = 0;
		//state = 0;
		heroid = 0;
	}

	UINT64 roleid;	
	//UINT32 state; ///> 0->初始选英雄阶段，1->游戏阶段
	UINT32 heroid;
	std::vector<UINT32> freeWeekHero;
	std::map<UINT32, UINT32> experienceHero;
};

class SceneMorphInfo : public ISceneEventListener, public ITimer
{
public:
	SceneMorphInfo(Scene* pScene);
	~SceneMorphInfo();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	virtual void OnPreEnterSceneFinally(Scene *pScene, Role* role);

	virtual void OnEnterSceneFinally(Scene *pScene, Role* role);

	virtual void OnRoleReconnect(Role* pRole);

	bool Init(const KKSG::SceneSwitchTeamData& roParam);

	void Update();

	bool CanMorph();

	KKSG::ErrorCode SetHero(UINT64 roleid, UINT32 heroid, bool checktime=true);

	const std::unordered_map<UINT64, MorphRoleData>& GetRoleDatas(){return m_roleData;}

	void AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime);

	MorphRoleData* GetRoleData(UINT64 roleId);

	UINT32 GetLeftMorphTime();

	UINT32 GetMorphTime(){return m_sceneMorphTime;}

	void OnRevive(Role* role);

private:
	void _SendTeamRoleToClient(Role* pRole=NULL);
	void _CheckAllChoose();
	UINT32 _CheckSetHero(Role* role, UINT32 heroid);
	void _SendCanUseHeroToClient(Role* pRole);
	void _CheckAutoChooseHero();
	bool _CheckCrossServerInfo(const KKSG::SceneSwitchTeamData& data);
private:
	Scene* m_pScene;

	UINT32 m_sceneMorphTime;

	UINT32 m_state;

	std::unordered_map<UINT64, MorphRoleData> m_roleData;  ///> 应该进入的role

	HTIMER m_timerHandler;

	//std::string m_battleUniqueTag;		///> tlog 一场唯一标识 
};

#endif
