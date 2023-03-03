#ifndef __RPCK2M_NOTIFYENTERCROSSBATTLE2MS_H__
#define __RPCK2M_NOTIFYENTERCROSSBATTLE2MS_H__

// generate by ProtoGen at date: 2017/4/13 16:29:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2M_NOTIFYENTERCROSSBATTLE2MS_TYPE 49625

class RpcK2M_NotifyEnterCrossBattle2MS : public CRpc
{
private:

	RpcK2M_NotifyEnterCrossBattle2MS():CRpc(RPCK2M_NOTIFYENTERCROSSBATTLE2MS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2M_NotifyEnterCrossBattle2MS *CreateRpc()
	{
		return new RpcK2M_NotifyEnterCrossBattle2MS;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NotifyEnterCrossBattle2MSArg &roArg, const NotifyEnterCrossBattle2MSRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NotifyEnterCrossBattle2MSArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NotifyEnterCrossBattle2MSArg &roArg, NotifyEnterCrossBattle2MSRes &roRes);
	void OnDelayReplyRpc(const NotifyEnterCrossBattle2MSArg &roArg, NotifyEnterCrossBattle2MSRes &roRes, const CUserData &roUserData);

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
	NotifyEnterCrossBattle2MSArg m_oArg;
	NotifyEnterCrossBattle2MSRes m_oRes;
};

#endif
