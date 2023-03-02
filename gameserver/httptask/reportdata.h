#ifndef __REPORTDATA_H__
#define	__REPORTDATA_H__

#include "unit/role.h"
#include "reportdata/reportdata_def.h"
#include "reportdata/reportdatabase.h"
class ReportDataTask : public ReportDataBaseTask
{
public:
	ReportDataTask(Role* role);
	virtual ~ReportDataTask();
	void BuildBaseData(Role* role);
};

#endif
