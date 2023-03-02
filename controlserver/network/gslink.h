#ifndef __GSLINK_H__
#define __GSLINK_H__

#include "pb/enum.pb.h"
#include "linelink.h"


class GSLink : public LineLink
{
	GSLink();
	~GSLink();
	DECLARE_SINGLETON(GSLink)

public:
	virtual const std::string &GetName();

	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);

	KKSG::ErrorCode AddGameServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine);
};

#endif