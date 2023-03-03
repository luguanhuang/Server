#ifndef __PTCN2T_KICKACCOUNT_H__
#define __PTCN2T_KICKACCOUNT_H__

// generate by ProtoGen at date: 2016/11/3 10:24:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2T_KICKACCOUNT_TYPE 7387

class PtcN2T_KickAccount : public CProtocol
{
public:
    explicit PtcN2T_KickAccount() : CProtocol(PTCN2T_KICKACCOUNT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2T_KickAccount()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KickAccountData m_Data;
};

#endif