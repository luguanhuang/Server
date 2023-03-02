
#ifndef __GUILDTERRITORYBATTLE_H__
#define __GUILDTERRITORYBATTLE_H__

#include "role/switch.h"
#include "table/TerritoryBattle.h"
#include "guild/guild.h"

struct TerritoryGuild 
{
	TerritoryGuild()
	{
		m_guildId = 0;
		m_allianceId = 0;
		m_roleId = 0;
		m_reqAllianceSet.clear();
	}

	TerritoryGuild(UINT64 guildId)
	{
		m_guildId = guildId;
		m_allianceId = 0;
		m_roleId = 0;
		m_reqAllianceSet.clear();
	}

	UINT64 m_guildId;
	UINT64 m_allianceId;               // ����id
	UINT64 m_roleId;                   // ��ս���id
	std::map<UINT64, UINT32/*ʱ��*/> m_reqAllianceSet; // ���������ѵ�id
	bool operator< (const TerritoryGuild &x)const;
};

struct CityUnit
{
	UINT64 m_guildId;   // ����id
	UINT32 m_time;      // ռ��ʱ��
};

class CGuildTerritoryBattle : public ITimer
{

private:
	const static UINT32 UPDATECD = 10;  // 10����´洢һ������
	CGuildTerritoryBattle();
	~CGuildTerritoryBattle();
	DECLARE_SINGLETON(CGuildTerritoryBattle);

public:
	bool Init(); 
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool Load();
	void Update();
	std::string ToBinary();

public:
	inline void SetChange() { m_switch.Set(); }
	inline bool IsChange() { return m_switch.TestAndReset(); }

public:
	bool DeclareWar(UINT64 roleId, UINT32 id);    // ��ս
	bool CanDeclareWar(UINT64 roleId, UINT32 id); // ��ս�������
	UINT32 ReqAlliance(UINT64 roleId, UINT64 guildId, UINT32 id); // �������� 
	UINT32 RcvAlliance(UINT64 roleId, UINT64 guildId);            // ��������
	void SetBattleResult(UINT32 id, UINT64 guildId);              // ����ս�����
	UINT32 GetDeclareCityId(UINT64 guildId);                      // ��ѯ��ս�����id
	UINT32 GetTimeType();                          // ��ȡʱ��׶�
	UINT32 GetGameType(UINT32 id,bool haveTerritory,Guild* guild); // ��ȡ��ͼ״̬ 
	UINT32 ClearAlliance(UINT64 guildId);          // �������������б�
	void DoOnlineCheck(UINT64 roleId);             // ����icon���
	void GetAllCityId(std::set<UINT32>& idSet);    // ��ȡ���е�ͼid
	UINT64 GetAllianceId(UINT64 guildId);          // ��ȡ����id 
	const TerritoryGuild* GetUnit(UINT64 guildId); 
	void ClearData(bool all = false);
	UINT32 GetCDTime();
	bool IsInWaring(UINT64 guildId); 
	void ReSort(UINT32 id);
	bool IsDeclareTime();
	void DoGuildDismiss(UINT64 guildId); // �����ɢ����
	//bool IsGuildWaring(UINT64 guildId);  // �����Ƿ���ս����
	void OnDesignation(UINT64 nGuildID,UINT32 nTerritoryID);

	inline std::map<UINT32, CityUnit>& GetCityMap() { return m_cityMap; } 
	inline std::map<UINT32, std::set<TerritoryGuild> >& GetChallList() { return m_challList; }
	inline TerritoryBattle::RowData* GetTerrConf(UINT32 id) { return m_territoryTable.GetByID(id); }

	
private:
	void _DoStart();        // ��ս
	void _StartAucReward(); // ��ʼ�����н���
	void _RemoveAlliance(UINT32 id, UINT64 guildId); // ������ѹ�ϵ
	void _ClearDeclareWar(UINT64 guildId);           // ������ս����
	void _SetAllianceId(UINT64 guilId, UINT64 allianId); // ��������id
	UINT32 _GetCityType(UINT32 id);                  // ��ѯ��id������� 
	UINT32 _GetGuildTerrId(UINT64 guildId);          // ��ȡ����Ŀǰռ�е�id
	UINT32 _GetGuildTerrType(UINT64 guildId);        // ��ȡ������ռ��ص�����
	UINT32 _GetAlliacnceNum(UINT64 guildId);         // ��ȡ�����б����
	bool _IsDeclare(UINT32 id, UINT64 guildId);      // �Ƿ��Ѿ��������ս
	bool _IsWarTime();       
	bool _IsWaitTime();
	bool _IsDeclareHasGuild(UINT32 id, UINT64 guildId);
	//bool _IsWarTime();
	bool _HasAlliance(UINT64 guildId);                // �Ƿ��Ѿ���������
	bool _IsCityBeDeclare(UINT32 id);
	void _DoDeclareTimeEnd();
	void _ReturnDrogn(UINT64 roleid);                 // ��ѡ�󷵻��������
	void _DoWarTimeEnd();
	void _NoticeWarIcon(UINT64 guildId, bool isBegin);   // ����ս��icon
	void _NoticeAllIcon(UINT64 guildId);   // ��������icon
	void _CheckAllIcon(UINT64 roleId);
	void _CheckWarIcon(UINT64 roleId);
	void _NoticeBeginWarning();
	void _GetAllDeclare(UINT32 id, std::set<UINT64>& declareSet);
	void _NoticeCityMaster();
	void _NoticeLoseDeclare(UINT32 id, UINT64 guildId);
	void _GetWarGuildSet(std::set<UINT64>& declareSet);
	void _ClearAllianceList(UINT64 guildId);
	void _NoticeCanDeclare(UINT32 id, UINT64 guildId);
	void _SendReward();

	bool _IsGuildWaring(UINT64 guildId);
	void _SendFailReward();
	UINT32 _GetWeekNum();
	void _DoClearJobe();
	void _DelGigIcon();



private:
	UINT32 m_battleBeginTime;         // �����ʱ���¼
	UINT32 m_aucBeginTime;            // ������������ʱ���¼
	UINT32 m_declareEndTime;          // ��սʱ�����
	UINT32 m_warEndTime;              // ս������ʱ��
	UINT32 m_rewardStartTime;         // �����н�����ʼʱ��

	HTIMER m_timehandler;
	UINT32 m_UpDataTime;              // ���ݴ洢��¼ʱ��
	Switch m_switch;                  // ���ݱ�����
	TerritoryBattle m_territoryTable; // ���ñ�
	std::set<UINT32> m_tempWaringId;  // ս�������е����id
	std::map<UINT32, CityUnit> m_cityMap;  // �����Ϣ
	std::map<UINT32, std::set<TerritoryGuild> > m_challList; // ��ս�б�
	std::set<UINT64> m_noticeRec;     // ����icon��¼
};


#endif