#ifndef __SQLTASK_H__
#define __SQLTASK_H__

#include "mysql.h"

class ISQLTask
{
public:
	virtual ~ISQLTask() {}

	virtual int  Execute(MYSQL *mysql) = 0;
	virtual void FinishTask() = 0;

public:
	int m_result;
};

#endif // __SQLTASK_H__