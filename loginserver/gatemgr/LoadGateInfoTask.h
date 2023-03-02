#ifndef _LOAD_SERVER_TASK_H_
#define _LOAD_SERVER_TASK_H_

#include "SQLTask.h"
#include "gateinfo.h"

class LoadGateInfoTask : public ISQLTask
{
public:
	LoadGateInfoTask(bool reload = false);
	~LoadGateInfoTask();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

private:
	std::vector<GateInfo> m_infos;
	bool m_reload;
};

#endif