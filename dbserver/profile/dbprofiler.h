#ifndef _H_Db_Profiler_H__
#define _H_Db_Profiler_H__

#include "profile/profiler.h"


class CDbProfiler : public CProfiler
{
public:
	CDbProfiler();
	~CDbProfiler();
	DECLARE_SINGLETON(CDbProfiler);

public:
	bool Init();
	void Uninit();

	virtual void DoProfile(FILE* fp);
};


#endif
