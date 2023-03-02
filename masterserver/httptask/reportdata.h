#ifndef __REPORT_DATA_H__
#define __REPORT_DATA_H__

#include "reportdata/reportdata_def.h"
#include "reportdata/reportdatabase.h"
#include "role/role.h"
class CReportDataTask : public ReportDataBaseTask
{
public:
	CReportDataTask(CRole* role);
	virtual ~CReportDataTask();
	void BuildBaseData(CRole* role);
};
#endif
