#ifndef _ROLE_REPORT_H_
#define _ROLE_REPORT_H_

#include "cvsreader.h"

struct ReportInfo
{
	ReportInfo(): reportRoleId(0), time(0), sceneType(0) {}

	UINT64 reportRoleId;
	UINT32 time;
	UINT32 sceneType;
	std::vector<UINT32> reasons;
	void FromKKSG(const KKSG::ReportBadPlayerData& data);
	void ToKKSG(KKSG::ReportBadPlayerData& data);
};

struct SceneReportInfo
{
	SceneReportInfo(): sceneType(0), banEndTime(0), reportNum(0) {}
	UINT32 sceneType;
	UINT32 banEndTime;
	UINT32 reportNum;

	void FromKKSG(const KKSG::RoleSceneReportData& data);
	void ToKKSG(KKSG::RoleSceneReportData& data);
};

class RoleReportInfo
{
public:
	RoleReportInfo();

	void FromKKSG(const KKSG::RoleReportData& data);
	void ToKKSG(KKSG::RoleReportData& data);
	void UpdateToDB();

	void SetRoleId(UINT64 roleId) { m_roleId = roleId; }

	SceneReportInfo* GetSceneReportInfo(UINT32 sceneType, bool add = false);
	void AddReportNum(UINT32 sceneType, int num);
	UINT32 GetReportNum(UINT32 sceneType);

	void SetDirty(bool dirty = true);

	UINT32 GetBanTime(UINT32 sceneType);
	void ResetBanTime(UINT32 sceneType);
	void CheckWeekReset();

private:
	bool m_dirty;
	UINT64 m_roleId;
	UINT32 m_updateTime;
	std::map<UINT32, SceneReportInfo> m_infos;
};

#endif