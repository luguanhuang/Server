#ifndef _QUERY_WEBCHAT_SHARE_CHEST_ID_
#define _QUERY_WEBCHAT_SHARE_CHEST_ID_

#include "WebFetchThread.h"
#include "unit/role.h"
#include "util/jsonutil.h"

class QueryWebChatShareChestid : public IWebFetchTask
{
public:
	QueryWebChatShareChestid();
	virtual ~QueryWebChatShareChestid();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

public:
	void SetRPCID(const UINT32 rpc_id) {rpc_id_ = rpc_id;};
	void SetOpenID(const std::string &open_id) {open_id_ = open_id;};

	std::string GetSign(UINT32 now);

private:		
	UINT32 rpc_id_;
	std::string open_id_;
	std::string bussid_;
};
#endif
