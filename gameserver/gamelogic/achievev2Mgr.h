#ifndef __ACHIEVE_V2_MGR_H__
#define __ACHIEVE_V2_MGR_H__

#include "table/AchievementV2Table.h"
#include "table/EquipSuitTable.h"
#include "table/AchievementPointRewardTable.h"
#include "table/FashionSuitTable.h"
#include "item.h"

struct ItemDesc;
class Role;
struct StageRankInfo;
class AchieveV2Mgr
{
	AchieveV2Mgr();
	~AchieveV2Mgr();
	DECLARE_SINGLETON(AchieveV2Mgr);
public:
	bool Init();
	void Uninit();
	bool LoadFile();
	void ClearFile();
	bool CheckFile();

	///>��ȡtable
	AchievementV2Table* GetTable();

	///>��ȡ����table
	AchievementPointRewardTable* GetRewardTable();

	///>ϵͳ�����ʾ
	void HintNotify(Role* role);

	///>����µĳɾ�����
	void SendReachAchieveNotify(Role* pRole, UINT32 achieveID);

	///>ˢ�»�óɾ�
	void UpdateAchieve(Role* pRole, int achieveId);

	///>GMָ���óƺ�
	void OnAddGetAchievement(Role* pRole, UINT32 aid);

	///>������װ��Ż�ȡ��װ�������ñ�ŵ���װ�м���
	UINT32 GetSuitCountBySuitID(Role* pRole, UINT32 suitId);

	///>ָ��ʱװID, ��ȡ��ʱװ����
	UINT32 GetFashionCountBySuitID(Role* pRole, UINT32 suitId);

	///>��ȡ��װ����
	//UINT32 GetSuitCount(Role* pRole);

	///>��ȡ������װ����
	UINT32 GetJewelrySuitCount(Role* pRole);

	///>��ȡװ����װ����
	UINT32 GetEquipSuitCount(Role* pRole);

	///>��ȡĳ��װ����װ�в�ͬװ������������ͬװ��ID��װ���������ظ���������
	UINT32 GetDiffEquipCountInSuit( Role* pRole, UINT32 equipSuitID );

	/////>��ȡ��Ʒ������װ������װ��
	//bool GetCurEquipAllSuitEquip(UINT32 itemId, std::vector<int>& vecEquip);

	///>��ȡ��ǰװ�����ڵ���װ��ǿ���ȼ�
	UINT32 GetEquipSuitLevel(Role* pRole, UINT64 equipUid, UINT32 needLevel,UINT32 needCount);

	///>��ȡʱװ����������ʱװ����
	UINT32 GetFashionCount(Role* pRole);

	///>��ȡĳ��ʱװ������װ��Ʒ�ʣ����û����������װ���򷵻�false
	bool GetFashionQuality(Role* pRole, UINT32 fashionId, UINT32& quality);


	///>�ⲿ���ú�������������ж�
	void OnJoinGuild(Role* pRole);
	void OnHaveSuitCount(Role* pRole, UINT32 itemID );
	void OnJewelrySuitCount( Role* pRole, UINT32 itemID );
	void OnEquipLevel(Role* pRole, UINT32 level);
	void OnSuitLevel(Role* pRole, UINT64 equipUid, UINT32 level);
	void OnJewlerySuitLevel(Role* pRole, UINT64 equipUid, UINT32 level);
	void OnChangeJob(Role* pRole);
	void OnLevelUp(Role* pRole);
	void OnDelGetAchievement(Role* pRole, UINT32 aid);
	void OnFinishChapter(Role* pRole, UINT32 sceneId);
	void OnLimitTimeFinishRaid(Role* pRole, UINT32 sceneId, UINT32 timespan);
	void OnStarFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);
	void OnNoHurtFinishRaid(Role* pRole, UINT32 sceneId, UINT32 hpPercent);
	void OnOneStarFinishRaid(Role* pRole, UINT32 sceneId);

	void OnGiveFlower(Role* pRole, int num);

	///>����ʱװ����
	void OnFashionCount(Role* pRole);
	void OnFashionQuality(Role* pRole, UINT32 fashionId);
	void OnNoDieFinishRaid(Role* pRole, UINT32 sceneId);
	void OnAloneFinishRaid(Role* pRole, UINT32 sceneId);
	void On3SFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);
	void OnFourNoDieFinishRaid(Role* pRole, UINT32 sceneId);
	void OnPKWinTimes(Role* pRole,UINT32 nWin);
	void OnPKScore(Role* pRole,UINT32 nPoint);
	void OnArenaRank(Role* pRole, UINT32 rank);
	void OnTowerFloor(Role* pRole,UINT32 nFloor);
	void OnTotalLogin(Role* pRole);
	void OnRechargeAmount(Role* pRole, UINT32 totalPay);
	void OnFirstRecharge(Role* pRole);

	void OnAssistFight(Role *pRole,UINT32 nCount);

	void OnSceneFinished(Role* pRole, UINT32 nSceneTemplateID, const StageRankInfo& stageinfo);

	void OnPassDragonScene(Role* pRole, UINT32 expID, UINT32 roleSize, UINT32 costTime);

	void OnMentor(Role* pRole, UINT32 nStudent);
	void OnMarry(Role* pRole);
	// garden achieve 
	void OnGarden(Role* pRole, UINT32 index);
	void DoEvent(Role *pRole,const KKSG::DesignationEvent *pEvent);
private:
	AchievementV2Table			m_achieveTable;
	AchievementPointRewardTable	m_rewardTable;

//�Ż�����
private:
	std::map<UINT32, std::vector<AchievementV2Table::RowData *> > m_mapConfig;
public:
	void LoadData();
	std::vector<AchievementV2Table::RowData *>* GetConfigType(UINT32 nType);
};

#endif