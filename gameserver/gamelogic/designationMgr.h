#ifndef __DESGIGNATION_MGR_H__
#define __DESGIGNATION_MGR_H__

#include "table/DesignationTable.h"
#include "table/EquipSuitTable.h"
#include "table/FashionSuitTable.h"
#include "item.h"
#include "define/eventdef.h"


//�ƺź�����ͣ�;
//1. ��óƺţ�����ƺűȵ�ǰ���ս���ƺ�ս�����ߣ����ͺ��;
//2. ���������ƺţ����õĳƺŲ��ǵ�ǰ���ս���ƺ�;
//3. �ͻ��˵㿪�ƺŽ��棬�����ǰ�����ƺ�ս����������߳ƺ�ս���������ͺ��;

struct ItemDesc;
class Role;
struct StageRankInfo;
class DesignationMgr
{
	DesignationMgr();
	~DesignationMgr();
	DECLARE_SINGLETON(DesignationMgr);
public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool CheckFile();
	bool LoadFile();


	//��ȡtable;
	DesignationTable* GetTable();

	//�ƺ�ϵͳ�����ʾ;
	void HintNotify(Role* role);

	//�״ν��볡�����;
	void FirstEnterSceneNotify(Role* pRole);

	//����µĳƺ�����;
	void SendReachDesignationNotify(Role* pRole, UINT32 designationID);

	//���ͷ���ƺ�;
	void SendCoverDesignationNotify(Role* pRole);

	//��ʾ���л�õĳƺ�;
	void OnShowDesignation(Role* pRole,  std::string mark);


	void OnAddGetDesigantion(Role* pRole, UINT32 did,UINT32 nTime);

	//��ȡ����;
	void ApplyEffect(Role* pRole, CombatAttribute* attr);

	void OnDelDesigantion(Role* pRole, UINT32 did);


	//�Ƿ��ø���ս���ĳƺ�;
	void IsHigherCombat(Role* pRole, UINT32 designationId);

	//��ȡ�ƺŵ�ս��;
	UINT32 GetDesignationPower( UINT32 prof, UINT32 designationId );

	//��ȡ���ս��;
	UINT32 GetMaxPPT(Role* pRole);

	//��ȡ������;
	void GetBigTypeRedPoint(Role* pRole, std::vector<bool>& vecRedPoint);



	//ˢ�»�õĳƺ�;
	void UpdateDesignation(Role* pRole, UINT32 desigantionId,std::string &name,UINT32 oper,UINT32 nParam, UINT32 time = 0);


	void UpdateDesignation(Role* pRole, UINT32 designationId, UINT32 time=0);
	//�����ʻ���ʷ���а�ƺ�;
	void UpdateFlowerHistoryRankDesignation(Role* pRole, UINT32 designationId);



	//��¼ʱ��������ʻ��ƺ�;
	void LoginUpdateFlowerDesignation( Role* pRole );


	//���÷���ƺ�;
	void SetCoverDesignation( Role* pRole, UINT32 coverDid );

//////////////�ⲿ����//////////////////////////////////////////////
	//�ȼ��ﵽ;
	void OnLevelUp(Role* pRole);

	//����װ���ȼ��ﵽ���ټ�;
	void OnAnyEquipLevel(Role* pRole, UINT32 equipLevel);

	//���ټ�װ����װ---��������bag.cpp�е��ã��ᵼ��崻�;
	void OnHaveSuitEquipCount(Role* pRole, UINT32 itemId);

	//��ö��ټ�������װ;
	void OnHaveJewelryCount(Role* pRole, UINT32 itemId);

	//����½�;
	void OnFinishChapter(Role* pRole, UINT32 scendId);

	//��ʱͨ�عؿ�;
	void OnLimitTimeFinishRaid(Role* pRole, UINT32 sceneId, UINT32 timespan);

	//3��ͨ�عؿ�;
	void On3StarFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);

	//����ͨ��;
	void OnNoHurtFinishRaid(Role* pRole, UINT32 sceneId, UINT32 hpPercent);

	//ӵ�ж��ٽ��---��������bag.cpp�е��ã��ᵼ��崻�;
	void OnHaveGoldCount(Role* pRole);

	//���ʱװ;
	void OnGetFashion(Role* pRole, UINT32 fashionId);

	//1��ͨ�عؿ�;
	void OnOneStarFinishRaid(Role* pRole, UINT32 sceneI);

	//��ʯ�ﵽ����;
	void OnHaveDiamondCount(Role* pRole);

	//�ͳ��ʻ�����;
	void OnGiveFlower(Role* pRole);

	//���빫��;
	void OnJoinGuild(Role* pRole);

	//˫��������ͨ��;
	void OnTwoNoDieFinishRaid(Role* pRole, UINT32 sceneId, UINT32 noDieNum);

	//����ͨ��;
	void OnAloneFinishRaid(Role* pRole, UINT32 sceneId, UINT32 roleCount);

	//����������ͨ��;
	void OnFourNoDieFinishRaid(Role* pRole, UINT32 sceneId);

	//3S������ͨ��
	void On3SFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);

	//ͨ��������;
	void OnMostHurtFinishRaid(Role* pRole, UINT32 sceneId);

	//��������ʤ����;
	void OnSkyLadderWinTimes(Role* pRole,UINT32 nWin);

	//����������;
	void OnSkyLadderScore(Role* pRole,UINT32 nPoint);

	void On2V2SkyLadderScore(Role* pRole,UINT32 nPoint);
	//����ڰ�������;
	void OnTowerFloor(Role* pRole,UINT32 nFloor);

	//���������;
	void OnFinishTask(Role* pRole, UINT32 taskId);

	//�ۼƵ�½����;
	void OnTotalLogin(Role* pRole);

	//��ֵ����;
	void OnRechargeAmount(Role* pRole, UINT32 totalPay);

	//��γ�ֵ���;
	void OnFirstRecharge(Role* pRole);

	//�ʻ����а�����;
	void OnFlowerRank(Role *pRole,UINT32 nRank, int time);

	//�ʻ����а���ʷ;
	void OnFlowerHistoryRank(Role *pRole,UINT32 nRank, int time);

	void OnGuilActivityRank(Role *pRole,UINT32 nRank, int time);
	//������ͨ��;
	void OnNoDieFinishRaid( Role* pRole, UINT32 sceneId );

	//�����״λ�ɱ����boss;
	void OnFirstTimeKillGuildBoss( UINT64 roleID, UINT32 bossID );



	//���ʱ����ʼ���ʻ���ʷ���а�;
	void InitFlowerHistoryRank();

	///>ս���ı�
	void OnPPT( Role* pRole );

	///>��ⳬ��ʱЧ�Եĳƺ�
	void UpdateExpireDesignation( Role* pRole );

	void DoEvent(Role *pRole,const KKSG::DesignationEvent *pEvent);
	///>������̨ս�ھ�
	void OnGuildArenaChampion(Role *pRole,UINT32 nWarType, int time);

	///>����������
	void OnPkWeekRank( UINT64 roleID, UINT32 rank, int time);

	///>����������
	void OnArenaRank( Role* pRole, UINT32 rank, int time );

	void AssistDesignation(Role *pRole, UINT32 nCount);

	void DargonAssistDesignation(Role *pRole, UINT32 nCount);
	void AssistWeekDesignation(Role *pRole, UINT32 nCount, int time);

	void TwoConditionsDesignation(Role *pRole, UINT32 nType ,int time,UINT32 nRank);
	void OneLessCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime,bool bCheck=false);
	//void OneLessCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime);
	//void OneBigerCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime,bool bCheck=false);
	void OneBigerCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime);
	void OnTwoCompConditionTime(Role *pRole,UINT32 nEventID,UINT32 nParam,UINT32 nTime,bool bCheck=false);
	void OneMarry(Role *pRole, UINT32 nEventID,UINT32 nParam ,UINT32 nOp,std::string szName,UINT32 nTime);
	void TwoComCondition(Role *pRole, UINT32 nEventID, int nParam,UINT32 nTime,bool bCheck=false);
	void OrderDesignation(Role *pRole,UINT32 nBigType, UINT32 nType,int time);
	bool IsValidDesignationByType(UINT32 type);

	void OnFirstPass(Role *pRole, UINT32 nSceneID, int time);

	void OnEqualComTime(Role *pRole,UINT32 nBigType,UINT32 nParam1,UINT32 nParam2,UINT32 nTime=0,bool bCheck=false);
	// garden 
	void OnGarden(Role * role, UINT32 event_type);
	void OnPassDragonScene(Role*  role,UINT32 nExpID);

	void OnEventTypeCom(Role* role,UINT32 nEventID,UINT32 nTime,bool bCheck);
	void OnGay(Role* role);
	void OnUseItem(Role* role,UINT32 nDesID);
	void OnCommonType(Role *pRole,UINT32 nBigType,UINT32 nParam1,UINT32 nParam2,UINT32 nTime);
	void OnSceneFinished(Role* pRole, UINT32 nSceneTemplateID, const StageRankInfo& stageinfo);

	void OnDragonGuild(Role* pRole, UINT32 nEventID, UINT32 nOp, std::string szName);

	void OnBeginMind(Role *pRole,UINT32 nID);
private:
	DesignationTable     m_designationTable;


	//�����ʻ����У�Ԫ�أ���ɫID;
	std::vector<UINT64> m_vecFlowerRank;

	//��ʷ�ʻ�����;
	std::vector<UINT64> m_vecHistoryFlowerRank;

	///>�ϴμ��ʱ��
	UINT32 m_dwLastUpTime;

	//С�Ż�
	//////////////////////////////////////////////////////////////////////////////////
public:
	void LoadData();
	std::vector<DesignationTable::RowData *>* GetConfigType(UINT32 nType);
	DesignationTable::RowData * GetRowData(UINT32 nID);
	std::string GetCoverName(Role *pRole);
	void OnEqualBiger(Role *pRole,UINT32 nBigType,UINT32 nParam1,UINT32 nParam2,UINT32 nTime,bool bCheck);
private:
	std::map<UINT32,std::vector<DesignationTable::RowData*> > m_mapConfig;
};

#endif