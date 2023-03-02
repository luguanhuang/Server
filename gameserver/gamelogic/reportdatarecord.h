#ifndef __REPORTDATA_RECORD_H__
#define __REPORTDATA_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pb/enum.pb.h"
#include "unit/role.h"

class CReportDataRecord : public RoleSerializeInterface
{
public:
	CReportDataRecord(Role* role);

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	void DayReset();
	void WeekReset();
	void Update();
	void AddOnlineTime(UINT32 onlinetime);
	void UpdataWeekLoginDays();
	void UpdateWeekNestFastTime(UINT32 time);
	UINT32 GetWeekLoginDays() { return m_WeekloginDays; }
	void UpdateWeekData(KKSG::WeekReportDataType type);
	INT32 GetWeekDataByType(KKSG::WeekReportDataType type);
	bool CheckRecommondTime();
	void UpdateAbyssCount();
	void UpdateSuperRiskCount();
	void UpdateBuyIbShopCount();
	void UpdatePokerCount();
	void UpdateHorseMatchCount();

private:
	Switch m_ismodify;
	UINT32 m_WeekloginDays;
	UINT32 m_With20minsDays;
	UINT32 m_LastDayOnlineTime;//最后一次登录该天的在线时长;
	UINT32 m_LastDayUpdate;
	UINT32 m_LastWeekUpdate;
	UINT32 m_WeekNestFastTime;//竞技巢穴最快通关时间;
	UINT32 m_WeekNestDayTimes;//当天竞技巢穴参与场数;
	UINT32 m_LastRecommondTime;//最后上报推荐战力差的时间;
	UINT32 m_AbyssDayCount;//当天参与龙魂禁地副本次数;
	UINT32 m_SuperRiskTodayCount;//当天大冒险参与次数;
	UINT32 m_BuyIbShopCount;//购买钻石商店道具次数（同时购买多个道具算一次）;
	UINT32 m_PokerCount; //参与小丑扑克次数（历史总参与次数）（每天算一次）;
	UINT32 m_LastPokerTime; //最后更新小丑扑克的时间;
	UINT32 m_HorseMacthCount;//参与全民赛马次数（历史总参与次数）;
	std::map<KKSG::WeekReportDataType, KKSG::WeekReportData> m_vWeekData;
	Role* m_poRole;
};
#endif
