#ifndef _H_Gs_Profiler_H__
#define _H_Gs_Profiler_H__

#include "profile/profiler.h"


class CGsProfiler : public CProfiler
{
public:
	CGsProfiler();
	~CGsProfiler();
	DECLARE_SINGLETON(CGsProfiler);

public:
	bool Init();
	void Uninit();

	virtual void DoProfile(FILE* fp);
private:
};


#endif
