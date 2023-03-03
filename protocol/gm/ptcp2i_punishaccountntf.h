#ifndef __PTCP2I_PUNISHACCOUNTNTF_H__
#define __PTCP2I_PUNISHACCOUNTNTF_H__

// generate by ProtoGen at date: 2017/4/1 19:18:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCP2I_PUNISHACCOUNTNTF_TYPE 65094

class PtcP2I_PunishAccountNtf : public CProtocol
{
public:
    explicit PtcP2I_PunishAccountNtf() : CProtocol(PTCP2I_PUNISHACCOUNTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcP2I_PunishAccountNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PunishAccount m_Data;
};

#endif