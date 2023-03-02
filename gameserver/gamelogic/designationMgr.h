#ifndef __DESGIGNATION_MGR_H__
#define __DESGIGNATION_MGR_H__

#include "table/DesignationTable.h"
#include "table/EquipSuitTable.h"
#include "table/FashionSuitTable.h"
#include "item.h"
#include "define/eventdef.h"


//称号红点推送：;
//1. 获得称号，如果称号比当前最高战力称号战力更高，推送红点;
//2. 设置能力称号：设置的称号不是当前最高战力称号;
//3. 客户端点开称号界面，如果当前能力称号战力不等于最高称号战力，则推送红点;

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


	//获取table;
	DesignationTable* GetTable();

	//称号系统红点提示;
	void HintNotify(Role* role);

	//首次进入场景红点;
	void FirstEnterSceneNotify(Role* pRole);

	//获得新的称号推送;
	void SendReachDesignationNotify(Role* pRole, UINT32 designationID);

	//推送封面称号;
	void SendCoverDesignationNotify(Role* pRole);

	//显示所有获得的称号;
	void OnShowDesignation(Role* pRole,  std::string mark);


	void OnAddGetDesigantion(Role* pRole, UINT32 did,UINT32 nTime);

	//获取属性;
	void ApplyEffect(Role* pRole, CombatAttribute* attr);

	void OnDelDesigantion(Role* pRole, UINT32 did);


	//是否获得更高战力的称号;
	void IsHigherCombat(Role* pRole, UINT32 designationId);

	//获取称号的战力;
	UINT32 GetDesignationPower( UINT32 prof, UINT32 designationId );

	//获取最高战力;
	UINT32 GetMaxPPT(Role* pRole);

	//获取大类红点;
	void GetBigTypeRedPoint(Role* pRole, std::vector<bool>& vecRedPoint);



	//刷新获得的称号;
	void UpdateDesignation(Role* pRole, UINT32 desigantionId,std::string &name,UINT32 oper,UINT32 nParam, UINT32 time = 0);


	void UpdateDesignation(Role* pRole, UINT32 designationId, UINT32 time=0);
	//更新鲜花历史排行榜称号;
	void UpdateFlowerHistoryRankDesignation(Role* pRole, UINT32 designationId);



	//登录时更新玩家鲜花称号;
	void LoginUpdateFlowerDesignation( Role* pRole );


	//设置封面称号;
	void SetCoverDesignation( Role* pRole, UINT32 coverDid );

//////////////外部调用//////////////////////////////////////////////
	//等级达到;
	void OnLevelUp(Role* pRole);

	//任意装备等级达到多少级;
	void OnAnyEquipLevel(Role* pRole, UINT32 equipLevel);

	//多少件装备套装---不可以在bag.cpp中调用，会导致宕机;
	void OnHaveSuitEquipCount(Role* pRole, UINT32 itemId);

	//获得多少件首饰套装;
	void OnHaveJewelryCount(Role* pRole, UINT32 itemId);

	//完成章节;
	void OnFinishChapter(Role* pRole, UINT32 scendId);

	//限时通关关卡;
	void OnLimitTimeFinishRaid(Role* pRole, UINT32 sceneId, UINT32 timespan);

	//3星通关关卡;
	void On3StarFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);

	//无伤通关;
	void OnNoHurtFinishRaid(Role* pRole, UINT32 sceneId, UINT32 hpPercent);

	//拥有多少金币---不可以在bag.cpp中调用，会导致宕机;
	void OnHaveGoldCount(Role* pRole);

	//获得时装;
	void OnGetFashion(Role* pRole, UINT32 fashionId);

	//1星通关关卡;
	void OnOneStarFinishRaid(Role* pRole, UINT32 sceneI);

	//钻石达到数量;
	void OnHaveDiamondCount(Role* pRole);

	//送出鲜花数量;
	void OnGiveFlower(Role* pRole);

	//加入公会;
	void OnJoinGuild(Role* pRole);

	//双人无死亡通关;
	void OnTwoNoDieFinishRaid(Role* pRole, UINT32 sceneId, UINT32 noDieNum);

	//独自通关;
	void OnAloneFinishRaid(Role* pRole, UINT32 sceneId, UINT32 roleCount);

	//四人无死亡通关;
	void OnFourNoDieFinishRaid(Role* pRole, UINT32 sceneId);

	//3S高评分通关
	void On3SFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);

	//通关输出最高;
	void OnMostHurtFinishRaid(Role* pRole, UINT32 sceneId);

	//天梯赛获胜场次;
	void OnSkyLadderWinTimes(Role* pRole,UINT32 nWin);

	//天梯赛积分;
	void OnSkyLadderScore(Role* pRole,UINT32 nPoint);

	void On2V2SkyLadderScore(Role* pRole,UINT32 nPoint);
	//到达黑暗神殿层数;
	void OnTowerFloor(Role* pRole,UINT32 nFloor);

	//完成任务获得;
	void OnFinishTask(Role* pRole, UINT32 taskId);

	//累计登陆天数;
	void OnTotalLogin(Role* pRole);

	//充值数额;
	void OnRechargeAmount(Role* pRole, UINT32 totalPay);

	//抽次充值获得;
	void OnFirstRecharge(Role* pRole);

	//鲜花排行榜：昨日;
	void OnFlowerRank(Role *pRole,UINT32 nRank, int time);

	//鲜花排行榜：历史;
	void OnFlowerHistoryRank(Role *pRole,UINT32 nRank, int time);

	void OnGuilActivityRank(Role *pRole,UINT32 nRank, int time);
	//无死亡通关;
	void OnNoDieFinishRaid( Role* pRole, UINT32 sceneId );

	//参与首次击杀公会boss;
	void OnFirstTimeKillGuildBoss( UINT64 roleID, UINT32 bossID );



	//起服时，初始化鲜花历史排行榜;
	void InitFlowerHistoryRank();

	///>战力改变
	void OnPPT( Role* pRole );

	///>检测超过时效性的称号
	void UpdateExpireDesignation( Role* pRole );

	void DoEvent(Role *pRole,const KKSG::DesignationEvent *pEvent);
	///>公会擂台战冠军
	void OnGuildArenaChampion(Role *pRole,UINT32 nWarType, int time);

	///>天梯赛排名
	void OnPkWeekRank( UINT64 roleID, UINT32 rank, int time);

	///>竞技场排名
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


	//昨日鲜花排行，元素：角色ID;
	std::vector<UINT64> m_vecFlowerRank;

	//历史鲜花排行;
	std::vector<UINT64> m_vecHistoryFlowerRank;

	///>上次检测时间
	UINT32 m_dwLastUpTime;

	//小优化
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