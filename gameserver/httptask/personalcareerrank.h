#ifndef _PERSONAL_CAREER_RANK_H_
#define _PERSONAL_CAREER_RANK_H_

#include "WebFetchThread.h"
#include "unit/role.h"
#include "util/jsonutil.h"

class PersonalCarrerRankTask : public IWebFetchTask
{
public:
	PersonalCarrerRankTask();
	virtual ~PersonalCarrerRankTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();


	void SetServerID(UINT32 server_id) {server_id_ = server_id;};
	void SetOpenID(const std::string &open_id) {open_id_ = open_id;};
	void SetRoleID(UINT64 role_id) {role_id_ = role_id;};
	void SetRPCID(UINT32 rpc_id) {rpc_id_ = rpc_id;};

	void SetPkRank(UINT32 pk_rank) { pk_rank_ = pk_rank;}
	void SetProfession(UINT32 profession_rank) { profession_rank_ = profession_rank;}


private:
	int GetIntFromString(const rapidjson::Value& json, const char* key);
private:	
	INT32		server_id_;
	std::string open_id_;
	UINT64		role_id_;
	UINT32		rpc_id_;

	UINT32 pk_rank_;
	UINT32 profession_rank_;
};
#endif
