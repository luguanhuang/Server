#ifndef __RPCM2D_SAVEGIFTIBHIST_H__
#define __RPCM2D_SAVEGIFTIBHIST_H__

// generate by ProtoGen at date: 2017/4/17 16:53:31

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_SAVEGIFTIBHIST_TYPE 9710

class RpcM2D_SaveGiftIbHist : public CRpc
{
private:

	RpcM2D_SaveGiftIbHist():CRpc(RPCM2D_SAVEGIFTIBHIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_SaveGiftIbHist *CreateRpc()
	{
		return new RpcM2D_SaveGiftIbHist;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SaveIbGiftRecordArg &roArg, const SaveIbGiftRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SaveIbGiftRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SaveIbGiftRecordArg &roArg, SaveIbGiftRecordRes &roRes);
	void OnDelayReplyRpc(const SaveIbGiftRecordArg &roArg, SaveIbGiftRecordRes &roRes, const CUserData &roUserData);

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
	SaveIbGiftRecordArg m_oArg;
	SaveIbGiftRecordRes m_oRes;
};

#endif
