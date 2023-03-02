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
	UINT32 m_LastDayOnlineTime;//���һ�ε�¼���������ʱ��;
	UINT32 m_LastDayUpdate;
	UINT32 m_LastWeekUpdate;
	UINT32 m_WeekNestFastTime;//������Ѩ���ͨ��ʱ��;
	UINT32 m_WeekNestDayTimes;//���쾺����Ѩ���볡��;
	UINT32 m_LastRecommondTime;//����ϱ��Ƽ�ս�����ʱ��;
	UINT32 m_AbyssDayCount;//�������������ظ�������;
	UINT32 m_SuperRiskTodayCount;//�����ð�ղ������;
	UINT32 m_BuyIbShopCount;//������ʯ�̵���ߴ�����ͬʱ������������һ�Σ�;
	UINT32 m_PokerCount; //����С���˿˴�������ʷ�ܲ����������ÿ����һ�Σ�;
	UINT32 m_LastPokerTime; //������С���˿˵�ʱ��;
	UINT32 m_HorseMacthCount;//����ȫ�������������ʷ�ܲ��������;
	std::map<KKSG::WeekReportDataType, KKSG::WeekReportData> m_vWeekData;
	Role* m_poRole;
};
#endif
