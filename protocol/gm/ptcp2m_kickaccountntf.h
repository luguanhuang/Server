#ifndef __PTCP2M_KICKACCOUNTNTF_H__
#define __PTCP2M_KICKACCOUNTNTF_H__

// generate by ProtoGen at date: 2017/4/2 16:16:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCP2M_KICKACCOUNTNTF_TYPE 23889

class PtcP2M_KickAccountNtf : public CProtocol
{
public:
    explicit PtcP2M_KickAccountNtf() : CProtocol(PTCP2M_KICKACCOUNTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcP2M_KickAccountNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PunishAccountBrief m_Data;
};

#endif