#ifndef __REPORTDATA_BASE_H__
#define __REPORTDATA_BASE_H__

#include "WebFetchThread.h"
#include "reportdata/reportdata_def.h"
struct stReportParam
{
	stReportParam()
	{
		type = 0;
		bcover = 0;
	}
	INT32 type;
	INT32 bcover;
	std::string data;
	std::string expires;
};

class ReportDataBaseTask : public IWebFetchTask
{
public:
	ReportDataBaseTask();
	virtual ~ReportDataBaseTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void AddData(INT32 iType, std::string strData, INT32 iBcover=Tx_Reportdata_BcoverNormal);
	std::string MakeSign(UINT32 ts, std::string key);
	void BuildPostData();
protected:
	std::string strAppID;
	std::string strAppKey;
	std::string strOpenId;
	std::string strToken;
	std::vector<stReportParam> m_oData;
	std::string strUrl;
};
#endif
