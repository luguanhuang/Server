#ifndef __SCENEHEROBATTLE_H__
#define __SCENEHEROBATTLE_H__

#include "scene.h"
#include "scenevsbase.h"
#include "vsstate.h"
#include "vshandler.h"
#include "define/noticedef.h"
#include "utility/tlogger.h"
#include "gamelogic/herobattlemgr.h"

enum HeroBattleState
{
	HeroBattleState_Normal,
	HeroBattleState_Loot,
	HeroBattleState_InFight,
};

enum HeroBattleTipsEnum
{
	HeroBattleTips_StartLoot			= 0,
	HeroBattleTips_Enemy_StartLoot		= 1,
	HeroBattleTips_StartInFight			= 2,
	HeroBattleTips_StartOverTime		= 3,
};

struct HeroBattleRoleData
{
	HeroBattleRoleData(){}
	HeroBattleRoleData(const KKSG::PvpRoleInfo& _info)
	{
		roleid = _info.roleid();
		info = _info;
		enterGameTime = 0;
		nextReviveTime = 0;
		isInCircle = false;
		heroid = 0;
		canChooseHero = false;
		state = 0;

		lastKillNum = 0;
		lastDamage = 0;
		lastHeroReviveTime = 0;

		ancientPower = 0;

		heroOrder = 1;
		preHero = -1;
		isChangeHero = false;
		kda = 0.0f;

		firstSendSignalTime = 0;
		signalCount = 0;
	}

	UINT64 roleid;	
	KKSG::PvpRoleInfo info;

	UINT64 enterGameTime;
	UINT64 nextReviveTime;

	bool isInCircle;

	UINT32 heroid;	
	bool canChooseHero;

	std::vector<UINT32> freeWeekHero;
	std::map<UINT32, UINT32> experienceHero;
	UINT32 state; ///> 0->��ʼѡӢ�۽׶Σ�1->��Ϸ�׶�

	UINT32 lastKillNum;				///> ѡ���Ӣ��ǰ������ͷ
	UINT32 lastDamage;				///> ѡ���Ӣ��ǰ�����˺�
	UINT32 lastHeroReviveTime;		///> ��ǰӢ�۸���ʱ��

	double ancientPower;			///> �Ϲ�����ֵ

	UINT32 heroOrder;				///> ��ǰӢ��˳��
	UINT32 preHero;					///> ǰһ��ѡ���Ӣ��
	bool isChangeHero;				///> ��ǰӢ�۸��ϴα��Ƿ�ı�
	float kda;
	std::set<UINT64> reportIds;		///> �ٱ����ҵ����ID

	UINT64 firstSendSignalTime;		///> ���źţ�ÿx�벻�ܳ���y�Σ�
	UINT32 signalCount;			
};

struct HeroBattleFightTeam
{
	HeroBattleFightTeam(UINT32 _teamid)
	{
		teamid = _teamid;
		point = 0;
	}
	std::vector<UINT64> teamMember;
	UINT32 teamid;
	UINT32 point;
	double eloPoint;
};

class SceneHeroBattle : public SceneVsBase
{
public:
	SceneHeroBattle(Scene* pScene);
	~SceneHeroBattle();
	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();

	virtual void OnEnterBeforeNtf(Role* pRole);
	virtual void OnRoleReconnectBeforeNtf(Role* role) {};

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual bool OnEvent(const VsEvent& event);
	virtual void Update();

	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();

	virtual VsStateType CheckFightEnd();

	virtual void OnStateEnd();

	virtual void OnRoleReconnect(Role* pRole);

	virtual void OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value);

	inline UINT32 GetTeam1() { return 1; }
	inline UINT32 GetTeam2() { return 2; }

	KKSG::ErrorCode SetHero(UINT64 roleid, UINT32 heroid);

	void FillResultAndReward(KKSG::HeroBattleResult* result, UINT64 roleId);
	UINT32 GetWinTeam();
	UINT64 GetMvpID();	

	const std::unordered_map<UINT64, HeroBattleRoleData>& GetRoleDatas(){return m_roleData;}

	void ResetHeroVar(HeroBattleRoleData& roledata);
	void PrintHeroTLog(HeroBattleRoleData& roledata, Role* pRole, bool isEnd);

	bool IsWinRole(UINT64 roleid);

	void GMEnd(Role* pRole);

	UINT32 GetRoleCamp(UINT64 roleid);

	void AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime);
	const CombatStatistics* GetCombatStatistics(UINT64 roleId);

	HeroBattleRoleData* GetRoleData(UINT64 roleId);

	virtual KKSG::ErrorCode CheckReportPlayer(UINT64 roleId, UINT64 badRoleId, int& num);

	KKSG::ErrorCode SelectHeroAncientPower(Role* pRole, UINT32 selectPower);

	void SendToTeamRole(UINT32 teamid, const CProtocol& oPtc);

	KKSG::ErrorCode SendSignal(Role* pRole, UINT32 type);

private:
	virtual UINT32 GetUpInterval() { return 0; }

	void _OnTickNormalState();
	void _OnTickLootState();
	void _OnTickInFightState();

	void _SetHeroBattleState(HeroBattleState state);

	void _OnEnterNormalState();
	void _OnEnterLootState();
	void _OnEnterInFightState();

	void _AddRoleDie(UINT64 roleid);
	void _CheckRoleRevive(UINT64 nowTime);
	void _RoleRevive(HeroBattleRoleData& data, bool isBeRevive);
	void _CheckChooseHero(UINT64 nowTime);

	void _SendGameNotice(Role* pRole, CNoticeType type);
	void _SendGameNoticeToAll(CNoticeType type);

	UINT32 _GetSameTeamInCircle();	///> 4�ַ���ֵ��2��teamid + 0->Ȧ��û�� + (UINT32)-1->Ȧ�ڲ�ͬ���
	UINT32 _GetTeamRoleNumInCircle(UINT32 teamid);

	bool _UpdateRoleInCircle();
	bool _IsInCircle(UINT64 roleid);	

	void _RecalLootRate();			///> ������������

	bool _CheckLastPointCondition(UINT32 teamid);
	bool _CheckEnd();

	void _SendCanUseHeroToClient(Role* pRole);
	void _SendTeamRoleToClient(Role* pRole = NULL);		///> ͬ�����Ӷ�����Ա��Ϣ����ѡӢ�ۣ�
	void _SendTeamDataToClient(Role* pRole = NULL);		///> ͬ����ͷ��������
	void _SendSyncDataToClient(Role* pRole = NULL);		///> ͬ��������ȣ��Ƿ�����������
	void _SendInCircleToClient(Role* pRole = NULL);		///> ͬ����Ȧ�ڵ���
	void _SendAncientPowerToClient(Role* pRole = NULL);	///> ͬ���Ϲ�����ֵ

	void _SyncAllDataToClient(Role* pRole);				///> ͬ����������

	void _AddOnePoint(bool force = false);

	void _ChangeOverTimeState(bool isStart);

	void _ClearLootData();

	void _CalcuKDA();
	void _MakeEndGameData();		///> ������Ϸ����¼����

	UINT64 _GetMVPID(UINT32 teamid = 0);	

	void _SendTipsToTeam(HeroBattleTipsEnum en, UINT32 teamid = (UINT32)-1);

	void _UpdateAncientPowerPerSecond();
	void _AddAncientPower(HeroBattleRoleData& data, UINT32 type, double percent = 0);

private:
	VsRoleDataMgr m_datamgr;
	VsLoadMgr m_loadmgr;

	HeroBattleMapCenterConf m_centerConf;

	HeroBattleFightTeam m_fightTeam1;
	HeroBattleFightTeam m_fightTeam2;

	std::unordered_map<UINT64, HeroBattleRoleData> m_roleData;  ///> Ӧ�ý����role

	UINT64 m_lastUpdateTime;			///> ��һ��updateʱ��

	UINT32 m_occupant;					///> ��ǰռ�����

	UINT32 m_lootTeam;					///> �������
	float m_lootProgress;				///> �������
	float m_lootRate;					///> ��������

	UINT64 m_lastAddPointTime;			///> ���һ�μӷ���ʱ��(ms)	

	UINT64 m_startOverTime;				///> ��ʼ��ʱ��ʱ��(ms)
	UINT32 m_overTimeCount;				///> ��ʱ����

	UINT64 m_lastAddAncientPowerTime;	///> ���һ�μ��Ϲ�����ʱ��

	HeroBattleState m_state;

	UINT64 m_mvpid;
	UINT64 m_loseMvpid;

	time_t m_noOneTime;					///> ����û�˵Ŀ�ʼʱ��(Ϊ�˱����������������ʱ����������٣�

	bool m_isSendNotice;

	std::string m_battleUniqueTag;		///> tlog һ��Ψһ��ʶ 
};



#endif