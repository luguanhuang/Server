#ifndef _H_Ms_Profiler_H__
#define _H_Ms_Profiler_H__

#include "profile/profiler.h"


class CMsProfiler : public CProfiler
{
public:
	CMsProfiler();
	~CMsProfiler();
	DECLARE_SINGLETON(CMsProfiler);

public:
	bool Init();
	void Uninit();

	virtual void DoProfile(FILE* fp);
private:
};


#endif
