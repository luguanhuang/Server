#ifndef _MULTI_ACTIVITY_MGR_BASE_H_
#define _MULTI_ACTIVITY_MGR_BASE_H_

#include "table/MultiActivityList.h"
#include "timer.h"
#include "pb/enum.pb.h"

enum MultActivityTypeID
{
	MULT_ACTIVITY_1 = 1,			// 1 ������̨ս
	MULT_ACTIVITY_GUILD_BOSS = 2,	// 2 ����BOSS
	MULT_ACTIVITY_WORLD_BOSS = 3,	// 3 ����BOSS
	MULT_ACTIVITY_GUILD_QA = 4,		// 4 ���������ɶ�
	MULT_ACTIVITY_5 = 5,			// 5 ������ս
	MULT_ACTIVITY_MULTI_QA = 6,		// 6 ȫ������ɶ�
	MULT_ACTIVITY_GUILD_BONUS = 7,	// 7 ������
	MULT_ACTIVITY_SKYCITY = 8,		// 8 ���֮��
	MULT_ACTIVITY_GUILDRESWAR = 9,  // 9  ������Դ����ս
	MULT_ACTIVITY_CARDMATCH = 13,	// 13 �˿˴��� 
	MULT_ACTIVITY_HORSE	= 14,		// 14 ����
	MULT_ACTIVITY_POKERTOUNAMENT = 15,	// 15 
	MULT_ACTIVITY_16 = 16,			//�������ս
	MULT_ACTIVITY_GUILDCAMP_PARTY = 17,	// ����Ӫ���Ŷ�
    MULT_ACTIVITY_WEEKEND4V4 = 18,	// ��ĩ�ɶ�
	MULT_ACTIVITY_BIGMELEE = 22,	//���Ҷ�
	MULT_ACTIVITY_WEEKENDNEST = 23, //����ս
	MULT_ACTIVITY_BATTLEFIELD = 24, //���ս��
	MULT_ACTIVITY_ID_MAX
};

struct MultActivityTime
{
	MultActivityTime():startSec(0), endSec(0) {}

	UINT32 startSec;	// ��ʼʱ��(���յ���)
	UINT32 endSec;		// ����ʱ��(���յ���)
};

struct MultActivityConf
{
	int id;
	std::vector<INT32> openDays;
	std::vector<MultActivityTime> openTime;
	UINT32 playerLevel;
	UINT32 guildLevel;
	UINT32 maxCount;
	UINT32 icon;
	UINT32 openServerWeek;

	KKSG::MulActivityTimeState curState;
};

class MultActivityMgrBase : public ITimer
{
public:
	MultActivityMgrBase();
	virtual ~MultActivityMgrBase();

	virtual bool Init();
	virtual void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool CheckFile();
	void ClearFile();
	bool LoadFile();

	// �����״̬�仯
	virtual void OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState);
	// ����״̬�仯��һ��check�б仯�ĻId
	virtual void OnActivitysChange(UINT32 opencount, std::vector<UINT32>& changeIds);


	// �ж������ʱ�䣬 now�ڿ����У����ص�ǰʱ��Σ�����ȡ >= now �������һ��ʱ���
	// ���첻�����򷵻ؿ��������ʱ��, ���쿪���򷵻ص�һ���ǽ����ʱ����������Ƿ����
	UINT32 GetOpenTime(UINT32 id);
	UINT32 GetEndTime(UINT32 id);
	//UINT32 GetNextGuildBossGapDay();

	// ͨ�ýӿ�
	bool IsActivityTimeOpen(UINT32 id);
	bool IsGuildLevelOpen(UINT32 id, UINT32 guildLvl);
	bool IsActivityOnDay(UINT32 id, UINT32 weekNum);

	KKSG::MulActivityTimeState GetOpenTimeState(UINT32 id);
	KKSG::MulActivityTimeState GetOpenTimeState(UINT32 id, UINT32& leftTime, MultActivityTime& openTime);
	KKSG::MulActivityTimeState GetOpenTimeState(MultActivityConf* config, UINT32& leftTime, MultActivityTime& openTime);

	// GM�ӻ
	void GMAddOpenDayTime(UINT32 id, UINT32 startTime, UINT32 endTime);
	// ���GM�ӵĻʱ��
	void ResetConfig(UINT32 id);

	// ����ϵͳ�Ľӿڷ�װ
	bool IsGuildBossOpen(UINT32 guildLvl);
	time_t GetGuilBossOpenTime();
	time_t GetGuilBossEndTime();
	void SetGuildBossTime(UINT32 startTime, UINT32 endTime);
	UINT32 GetGuildBossOpenLvl();
	bool IsGuildBossOnTime();
	void SetOpenTime(UINT32 nTime)
	{
		m_nOpenTime = nTime;
	}

	MultActivityConf* GetActivityConfig(UINT32 id);

	void CheckActivityStateChange();

private:
	void AddConfig(MultiActivityList::RowData* conf);
	MultActivityTime GetNearTime(std::vector<MultActivityTime>& times, UINT32 todaySec);
	UINT32 GetOpenDayZeroSecond(MultActivityConf* config);


protected:
	UINT32 m_nOpenTime;
	HTIMER m_timer;
	MultiActivityList m_tableList;
	std::map<UINT32, MultActivityConf> m_activitys;
};

#endif