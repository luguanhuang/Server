#ifndef _H_Control_Profiler_H__
#define _H_Control_Profiler_H__

#include "profile/profiler.h"


class CControlProfiler : public CProfiler
{
public:
	CControlProfiler();
	~CControlProfiler();
	DECLARE_SINGLETON(CControlProfiler);

public:
	bool Init();
	void Uninit();

	virtual void DoProfile(FILE* fp);
private:
};


#endif
