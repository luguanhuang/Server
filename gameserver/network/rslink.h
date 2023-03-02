#ifndef _GS_RSLINK_H_
#define _GS_RSLINK_H_

#include "linelink.h"
#include "pb/enum.pb.h"
#include <set>


class RSLink : public LineLink
{
	RSLink();
	virtual ~RSLink();
	DECLARE_SINGLETON(RSLink)

public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual const std::string & GetName();

	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);

	KKSG::ErrorCode AddRouterServer(UINT32 connId, UINT32 line);

	void AddToConnect(UINT32 line, const std::string& ip , UINT32 port);

private:
	std::set<UINT32> m_allLines;			// 所有需要加到ReconnectManager的RS
	std::map<UINT32, UINT32> m_line2Conn;	// 多存一份line跟connection的映射
};

#endif