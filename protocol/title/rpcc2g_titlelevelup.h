#ifndef __RPCC2G_TITLELEVELUP_H__
#define __RPCC2G_TITLELEVELUP_H__

// generate by ProtoGen at date: 2016/8/23 18:02:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TITLELEVELUP_TYPE 24381

class RpcC2G_TitleLevelUp : public CRpc
{
private:

	RpcC2G_TitleLevelUp():CRpc(RPCC2G_TITLELEVELUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TitleLevelUp *CreateRpc()
	{
		return new RpcC2G_TitleLevelUp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TitleLevelUpArg &roArg, const TitleLevelUpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TitleLevelUpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TitleLevelUpArg &roArg, TitleLevelUpRes &roRes);
	void OnDelayReplyRpc(const TitleLevelUpArg &roArg, TitleLevelUpRes &roRes, const CUserData &roUserData);

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
	TitleLevelUpArg m_oArg;
	TitleLevelUpRes m_oRes;
};

#endif
