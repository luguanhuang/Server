#ifndef _SCENE_LEAGUE_H_
#define _SCENE_LEAGUE_H_

#include "scenevsbase.h"
#include "leaguebattlebase.h"
#include "timer.h"


class Scene;
class Role;
class SceneLeague : public SceneVsBase, public ITimer
{
public:
	SceneLeague(Scene* scene);
	~SceneLeague();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();

	virtual UINT32 GetGroup(UINT64 roleid);
	virtual UINT64 GetFightGroupUId(UINT64 roleid);

	virtual void Update();

	// load��Ϣ���� 
	virtual void OnEnterBeforeNtf(Role* role);
	virtual void OnRoleReconnect(Role* role);

	// ���̿���
	// WaitLoad
	virtual void OnEnterWaitLoad();
	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();

	// Ready�׶�
	void OnReadyStart();
	VsStateType CheckReadyEnd();
	void OnReadyEnd();

	// Prepare321
	virtual void OnEnterP321();

	// Fight
	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();

	// AfterFight
	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	// End
	virtual void OnStateEnd();

	// �¼�
	virtual bool OnEvent(const VsEvent& event);
	void OnRoleEnter(Role* pRole);		// ��ҽ��볡��������ս���͹�ս
	void OnRoleLoad(Role* pRole);
	void OnRoleQuit(Role* pRole);
	void OnRoleDie(Role* pRole);

	// ������
	int SetRoleUp(Role* pRole);
	int SetRoleDown(Role* pRole);

	bool IsFightRole(UINT64 roleId);
	void OnBeforeChangeWather(Role* pRole);

	LeagueBattleRoom& GetLeagueRoomInfo(){return m_room;}
	bool BeginRankUpdate();
	void EndRankUpdate(const KKSG::LeagueRankUpdateG2MRes* res);

	void GMSetReady();

private:
	UINT32 GetReadyLeftTime();
	UINT32 GetFightLeftTime();
	UINT32 GetAfterFightLeftTime();
	KKSG::LeagueBattleFightState GetBattleState(UINT32& leftTime);

	bool SetFightRole();

	bool CheckRoleFailed(UINT64 roleId);
	UINT64 GetHpPrecentLessID();
	void UpdateHp();
	void UpdateWinLeague();
	void ClearFailInfo();
	void RemoveFailRole();

	void NotifyLoadInfo(Role* pRole);
	void NotifyOneBattleResult();
	void NotifyBattleResult();
	void NotifyBattleState(Role* pRole = NULL);
	void NotifyBaseData(Role* pRole = NULL);
	void DoTxLog();

private:
	LeagueBattleRoom m_room;		// ս���������
	bool m_initLoaded;				// ��ʼ���أ���һ�����doenterscene���ó�true
	bool m_readyEndBefore;			// ׼���׶ν���֮ǰ���룬����֪ͨMS����������ǹ�ս��
	bool m_readyEnd;				// ׼���׶ν������

	UINT64 m_roleId1;				// ����ս����roleid1
	UINT64 m_roleId2;				// ����ս����roleid2

	// ÿ��ս����ʤ����Ϣ
	UINT64 m_winId;
	UINT64 m_failId;
	float m_winHpPercent;
	float m_failHpPercent;

	// ���ս��
	UINT64 m_winLeagueId;
	UINT64 m_loseLeagueId;

	bool m_updateRankDelay;
	HTIMER m_timer;
};

#endif