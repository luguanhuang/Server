#ifndef _PERSONAL_CAREER_INFO_H_
#define _PERSONAL_CAREER_INFO_H_

#include "WebFetchThread.h"
#include "unit/role.h"
#include "util/jsonutil.h"

class PersonalCarrerInfoTask : public IWebFetchTask
{
public:
	PersonalCarrerInfoTask();
	virtual ~PersonalCarrerInfoTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void SetServerID(UINT32 server_id) {server_id_ = server_id;};
	void SetOpenID(const std::string &open_id) {open_id_ = open_id;};
	void SetRoleID(UINT64 role_id) {role_id_ = role_id;};
	void SetRPCID(UINT32 rpc_id) {rpc_id_ = rpc_id;};

	void SetServerName(const std::string& server_name) {server_name_ = server_name;};

private:	
	INT32		server_id_;
	std::string open_id_;
	UINT64		role_id_;
	UINT32		rpc_id_;
	std::string server_name_;
};
#endif
