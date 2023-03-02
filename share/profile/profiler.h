#ifndef _H_Profiler_H__
#define _H_Profiler_H__

#include "timer.h"


class CProfiler : public ITimer
{
public:
	CProfiler();
	~CProfiler();

	void StartTimer();
	void StopTimer();

	virtual void DoProfile(FILE* fp) {}
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void SetFileName(const std::string& strName) { m_strFileName = strName; }
	const std::string& GetFileName() const { return m_strFileName; }
private:
	HTIMER		m_hCheckTimer;
	std::string	m_strFileName;
};

#endif