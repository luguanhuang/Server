#ifndef __UPDATE_USERINFO_H__
#define __UPDATE_USERINFO_H__

#include "WebFetchThread.h"
#include "unit/role.h"
#include "util/jsonutil.h"

class UpdateUserInfoTask : public IWebFetchTask
{
public:
	UpdateUserInfoTask();
	virtual ~UpdateUserInfoTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData(rapidjson::Value& oBodyList , JsonBuilder& oBuilder);
	void AddBodyData(UINT64 roleid, std::string strOpenId, INT32 iOnlineTime, rapidjson::Value& oBodyList , JsonBuilder& oBuilder);
	void SetPlatID(INT32 iPlatID);
	void SetServerID(UINT32 uServerID);

private:
	INT32 m_iPlatID;
	INT32 m_uServerID;
};
#endif
