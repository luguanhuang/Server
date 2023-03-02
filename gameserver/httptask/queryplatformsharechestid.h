#ifndef _QUERY_PLATFORM_SHARE_CHEST_ID_
#define _QUERY_PLATFORM_SHARE_CHEST_ID_

#include "WebFetchThread.h"
#include "unit/role.h"
#include "util/jsonutil.h"

class QueryPlatformShareChestid : public IWebFetchTask
{
public:
	QueryPlatformShareChestid();
	virtual ~QueryPlatformShareChestid();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

public:
	void SetRPCID(const UINT32 rpc_id) {rpc_id_ = rpc_id;};
	void SetOpenID(const std::string &open_id) {open_id_ = open_id;};
	void SetPf(const std::string &pf) {pf_ = pf;};
	void SetActId( const UINT32 act_id) {act_id_ = act_id;};
	void SetNum(const UINT32 num ) {num_ = num;};
	void SetPeopleNum(const UINT32 people_num) { people_num_ = people_num;};
	void SetToken(const std::string &token) {token_ = token;};
	std::string GetSign(UINT32 now, const std::string& key);

	void BuildPostData();
private:		
	UINT32 rpc_id_;
	std::string open_id_;
	std::string pf_;
	UINT32 act_id_;
	UINT32  num_;
	UINT32 people_num_;


	std::string token_;
};
#endif
