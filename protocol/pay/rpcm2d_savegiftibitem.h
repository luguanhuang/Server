#ifndef __RPCM2D_SAVEGIFTIBITEM_H__
#define __RPCM2D_SAVEGIFTIBITEM_H__

// generate by ProtoGen at date: 2017/3/18 15:00:31

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_SAVEGIFTIBITEM_TYPE 50211

class RpcM2D_SaveGiftIbItem : public CRpc
{
private:

	RpcM2D_SaveGiftIbItem():CRpc(RPCM2D_SAVEGIFTIBITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_SaveGiftIbItem *CreateRpc()
	{
		return new RpcM2D_SaveGiftIbItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GiftIbItemArg &roArg, const GiftIbItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GiftIbItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GiftIbItemArg &roArg, GiftIbItemRes &roRes);
	void OnDelayReplyRpc(const GiftIbItemArg &roArg, GiftIbItemRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	GiftIbItemArg m_oArg;
	GiftIbItemRes m_oRes;
};

#endif
