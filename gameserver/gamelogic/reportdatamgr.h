#ifndef __REPORTDATAMGR_H__
#define __REPORTDATAMGR_H__
#include "unit/role.h"
#include "pb/enum.pb.h"
#include "table/ReportDataNestTable.h"
#include "table/RecommendFightNum.h"

class ReportDataMgr
{
public:
	ReportDataMgr();
	~ReportDataMgr();
	DECLARE_SINGLETON(ReportDataMgr);

public:
	bool Init();
	void Uninit();
	void DailyOnline(Role* role, INT32 iOnlineTime);
	void AtlasActive(Role* role, UINT32 groupId, UINT32 Num);
	void PkChange(Role* role, KKSG::PkResultType result);//天梯赛相关数据上报;
	void PvpChange(Role* role,  KKSG::PvpOneRec& rec);
	void TowerChange(Role* role, INT32 iFloor);
	void NestFirstPass(Role* role, INT32 id);
	void NestResult(Role* role, INT32 id, INT32 result);
	void PvpTime(Role* role, UINT32 iTime);
	void PkTime(Role* role, UINT32 iTime);
	void Within20Mins(Role* role);
	void OnChangePPT(Role* pRole);
	void HeroBattleResult(Role* pRole, UINT32 resoult, UINT32 killnum, UINT32 deadnum); //0失败 1胜利 3平局
	void WeekNest(Role* pRole, UINT32 iTime, UINT32 Count);
	void Common(Role* role, INT32 type, const string& data);
	UINT32 GetRecommendPoint(UINT32 level);
	void MobaBattleResult(Role* pRole, UINT32 result, UINT32 killnum, UINT32 deadnum);
	void FashionCompose(Role* pRole, bool isSuccess, UINT32 quality);


	std::string GetWinRate(UINT32 win, UINT32 total);

private:
	ReportDataNestTable m_oTable;
	RecommendFightNum m_oRecommondTable;
	bool m_bIsQQ;
};
#endif
