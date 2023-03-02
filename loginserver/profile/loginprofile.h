#ifndef _H_Login_Profiler_H__
#define _H_Login_Profiler_H__

#include "profile/profiler.h"

class CLoginProfiler : public CProfiler
{
public:
	CLoginProfiler();
	~CLoginProfiler();
	DECLARE_SINGLETON(CLoginProfiler);

public:
	bool Init();
	void Uninit();

	virtual void DoProfile(FILE* fp);
};


#endif
