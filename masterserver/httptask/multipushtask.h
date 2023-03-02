#ifndef _MULTI_PUSH_TASK_H_
#define _MULTI_PUSH_TASK_H_

#include "pushtask.h"
#include "singlepushtask.h"

class CRoleSummary;
class XinGePushMgr;
class CreateMultiPushTask : public SinglePushTask
{
public:
	CreateMultiPushTask(UINT32 id, XinGePushMgr* mgr) :m_id(id), m_mgr(mgr) {}
	virtual ~CreateMultiPushTask() {}

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

private:
	UINT32 m_id;
	XinGePushMgr* m_mgr;
};

class MultiPushTask : public PushTask
{
public:
	const static UINT32 kMaxNumPerPush = 50;

	MultiPushTask(UINT32 pushId):m_pushId(pushId) {}
	virtual ~MultiPushTask() {}

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	// 表格中配置类型，用于过滤掉设置了不接受的玩家
	void PackTokens(std::list<CRoleSummary*>& roleList);

private:
	UINT32 m_pushId;
};

#endif
