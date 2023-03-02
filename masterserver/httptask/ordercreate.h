#ifndef _ORDER_CREATE_H__
#define _ORDER_CREATE_H__


#include "WebFetchThread.h"

class OrderCreateTask : public IWebFetchTask
{
public:
	OrderCreateTask();
	virtual ~OrderCreateTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void SetRpcId(UINT32 id) { m_rpcId = id; }
	//void BuildPostData(KKSG::OrderCreateArg oData);

private:
	UINT32 m_rpcId;
};

#endif
