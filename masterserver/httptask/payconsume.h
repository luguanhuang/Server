#ifndef __PAYCONSUME_H__
#define __PAYCONSUME_H__

#include "WebFetchThread.h"
#include "role/role.h"

class PayConsumeTask : public IWebFetchTask
{
public:
	PayConsumeTask();
	virtual ~PayConsumeTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void BuildData(INT32 iAmt, std::string billno);
	void SetParamData(KKSG::PayParameterInfo oData);
	void SetRoleId(UINT64 roleid) { m_RoleId = roleid; }
	void SetBillNo(std::string billno) { m_strBillNo = billno; }
	void SetRpcID(INT32 ID) { m_rpcid=ID;}
	virtual void OnReply(INT32 err);
	void SetIsDirectly(bool isdir) { m_isdirectly = isdir; }
	void SetFirstHint(bool isHint) { m_isFirstHint = isHint; }
	void AddGoods(CRole*role, INT32 iBalance);
	void SetListInfo(const KKSG::Payconsumelist& list) { m_Listinfo=list;}
	void NotifyClient();
	void OnRetFailed();
	std::map<std::string, std::string> m_oparams;

protected:
	UINT64 m_RoleId;
	std::string m_strBillNo;
	INT32 m_rpcid;
	bool m_isdirectly;//直接的订单，不需要走存储;
	bool m_isFirstHint;//第一次发起时，有错误时是否提示;
	INT32 m_iRet;
	INT32 m_MidasErrCount;//midas返回3000111错误需要立马重试一下。
	KKSG::Payconsumelist m_Listinfo;
};

class PayConsumeTeamTask : public PayConsumeTask
{
public:
	PayConsumeTeamTask():PayConsumeTask()
	{
		m_type = 0;	
	}
	void SetArg(const KKSG::TeamOPArg& roArg);
	void SetArg(const KKSG::TeamTransData& roArg);
	virtual void OnReply(INT32 err);

private:
	KKSG::TeamOPArg m_roArg;//本服
	KKSG::TeamTransData m_roData;//跨服
	int m_type;
};

class PayConsumeIdipTask : public PayConsumeTask
{
public:
	PayConsumeIdipTask();
	virtual ~PayConsumeIdipTask();
	void SetID(UINT32 id);
	virtual void OnReply(INT32 err);
	UINT32 GetID(){return m_dwID;}

private:
	UINT32 m_dwID;
};
#endif

