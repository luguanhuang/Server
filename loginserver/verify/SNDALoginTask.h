#ifndef __SNDALOGINTASK_H__
#define __SNDALOGINTASK_H__

#include "WebFetchThread.h"

class SNDALoginTask : public IWebFetchTask
{
public:

	SNDALoginTask();
	~SNDALoginTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	inline void SetToken(const std::string &token) { m_token = token; }
	inline void SetRpcID(UINT32 dwRpcID) { m_dwRpcID = dwRpcID; }
	inline void SetIP(UINT32 ip) { m_ip = ip; }
	
	void SetPF(const std::string &pf);
	const std::string GetHttpParam();

private:
	UINT32 m_ip;
	UINT32 m_dwRpcID;
	std::string m_pf;
	std::string m_token;
};

#endif // __SNDALOGINTASK_H__