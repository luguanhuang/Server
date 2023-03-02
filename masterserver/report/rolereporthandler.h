#ifndef _ROLE_REPORT_HANDLER_H_
#define _ROLE_REPORT_HANDLER_H_

#include "cvsreader.h"

struct SceneReportInfo;
class IReportHandler
{
public:
	IReportHandler() {}
	virtual ~IReportHandler() {}

	virtual void UpdateBan(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 oldNum) = 0;
	void DoBanLog(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 time);
};

class ReportHeroBattleHandler : public IReportHandler
{
public:
	virtual void UpdateBan(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 oldNum);

	void DoSendHeroBattleBanMail(UINT64 roleId, Sequence<UINT32, 5>& timeSeq);
};

class ReportMobaBattleHandler : public IReportHandler
{
	virtual void UpdateBan(SceneReportInfo* pSInfo, UINT64 roleId, UINT32 oldNum);

	void DoSendMobaBattleBanMail(UINT64 roleId, Sequence<UINT32, 5>& timeSeq);
};



#endif