#ifndef __MOBARECORD_H__
#define __MOBARECORD_H__


class CRole;

class MobaRecord
{
public:
	MobaRecord();
	MobaRecord(UINT64 roleid);
	~MobaRecord();

	void ToKKSG(KKSG::MobaBattleRoleRecord& record);
	void ToThis(const KKSG::MobaBattleRoleRecord& record);

	void SetChanged();

	void AddGameRecord(const KKSG::AddMobaBattleRecordData& data);
	
	void FillGameBriefRecord(KKSG::GetMobaBattleBriefRecordRes& res);
	void FillOneGameRecord(UINT32 tag, KKSG::GetMobaBattleGameRecordRes& res);

	inline double GetELOPoint() const { return m_eloPoint; }

	void Update();

	UINT32 GetWeekPrize() { Update(); return m_weekPrize; }
	bool CanGetNextWeekPrize();

	void FillMobaBattleInfo(KKSG::GetMobaBattleInfoRes& res);
	KKSG::ErrorCode GetWeekNextReward(CRole* pRole);

private:
	void _Init();

private:
	UINT64 m_roleid;
	double m_eloPoint;
	UINT32 m_tagCount;
	UINT32 m_totalNum;
	UINT32 m_winNum;
	UINT32 m_lastUpdateTime;
	UINT32 m_winThisWeek;
	UINT32 m_weekPrize;
	UINT32 m_todayNum;
	UINT32 m_realTotalNum;
	UINT32 m_realWinNum;
	std::list<KKSG::MobaBattleOneGame> m_gameRecord;
};



#endif
