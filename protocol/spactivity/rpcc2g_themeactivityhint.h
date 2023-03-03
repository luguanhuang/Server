#ifndef __RPCC2G_THEMEACTIVITYHINT_H__
#define __RPCC2G_THEMEACTIVITYHINT_H__

// generate by ProtoGen at date: 2017/8/1 19:22:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_THEMEACTIVITYHINT_TYPE 39987

class RpcC2G_ThemeActivityHint : public CRpc
{
private:

	RpcC2G_ThemeActivityHint():CRpc(RPCC2G_THEMEACTIVITYHINT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ThemeActivityHint *CreateRpc()
	{
		return new RpcC2G_ThemeActivityHint;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ThemeActivityHintArg &roArg, const ThemeActivityHintRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ThemeActivityHintArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ThemeActivityHintArg &roArg, ThemeActivityHintRes &roRes);
	void OnDelayReplyRpc(const ThemeActivityHintArg &roArg, ThemeActivityHintRes &roRes, const CUserData &roUserData);

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
	ThemeActivityHintArg m_oArg;
	ThemeActivityHintRes m_oRes;
};

#endif
