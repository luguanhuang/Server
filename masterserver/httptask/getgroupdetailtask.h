#ifndef _GET_GROUP_DETAIL_H_
#define _GET_GROUP_DETAIL_H_

#include "qqgrouptaskbase.h"

class GetGroupDetailTask : public QQGroupTaskBase
{
public:
	GetGroupDetailTask();
	virtual ~GetGroupDetailTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

private:
	INT32 m_platCode;
};

#endif