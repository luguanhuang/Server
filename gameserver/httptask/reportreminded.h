#ifndef __REPORT_REMINDED_H__
#define __REPORT_REMINDED_H__

#include "WebFetchThread.h"
#include "unit/role.h"
#include "util/jsonutil.h"
class ReportRemindedTask : public IWebFetchTask
{
public:
	ReportRemindedTask();
	virtual ~ReportRemindedTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData(rapidjson::Value& oBodyList , JsonBuilder& oBuilder);
	void AddBodyData(Role* pRole, INT32 iType, rapidjson::Value& oBodyList , JsonBuilder& oBuilder);
	void SetPlatID(INT32 iPlatID);
	void SetServerID(UINT32 uServerID);

private:
	INT32 m_iPlatID;
	INT32 m_uServerID;
};
#endif
