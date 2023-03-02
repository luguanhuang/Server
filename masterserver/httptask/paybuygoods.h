#ifndef __PAYBUYGOODS_H__
#define __PAYBUYGOODS_H__

#include "WebFetchThread.h"

class PayBuyGoodsTask : public IWebFetchTask
{
public:
	PayBuyGoodsTask();
	virtual ~PayBuyGoodsTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void OnReply();
	void SetParamData(KKSG::PayParameterInfo oData);
	void SetPayItem(KKSG::PayGiftIbItem oItem, UINT32 price);
	void SetGoodsMeta(UINT64 roleid, const std::string& name, const std::string& desc);
	void SetRpcId(UINT32 rpcid) { m_rpcId = rpcid; }
public:
	std::map<std::string, std::string> m_oparams;

private:
	UINT64 m_RoleId;
	UINT32 m_rpcId;
	INT32 m_Ret;
	KKSG::PayGiftIbItem m_Data;
};
#endif
