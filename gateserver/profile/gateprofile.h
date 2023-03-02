#ifndef _H_Gs_Profiler_H__
#define _H_Gs_Profiler_H__

#include "profile/profiler.h"


class CGateProfile : public CProfiler
{
public:
	CGateProfile();
	~CGateProfile();
	DECLARE_SINGLETON(CGateProfile);

public:
	bool Init();
	void Uninit();

	virtual void DoProfile(FILE* fp);

	inline void AddBroadCastNum(UINT32 value = 1) { m_broadCastNum += value; }
	inline void AddBroadCastClient(UINT32 value) { m_broadCastClientNum += value; }

private:
	UINT64 m_broadCastNum;
	UINT64 m_broadCastClientNum;
};


#endif
