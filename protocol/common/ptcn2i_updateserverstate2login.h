#ifndef __PTCN2I_UPDATESERVERSTATE2LOGIN_H__
#define __PTCN2I_UPDATESERVERSTATE2LOGIN_H__

// generate by ProtoGen at date: 2016/12/23 11:10:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2I_UPDATESERVERSTATE2LOGIN_TYPE 5865

class PtcN2I_UpdateServerState2Login : public CProtocol
{
public:
    explicit PtcN2I_UpdateServerState2Login() : CProtocol(PTCN2I_UPDATESERVERSTATE2LOGIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2I_UpdateServerState2Login()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateServerState2Login m_Data;
};

#endif