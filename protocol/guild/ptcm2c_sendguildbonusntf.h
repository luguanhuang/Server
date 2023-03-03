#ifndef __PTCM2C_SENDGUILDBONUSNTF_H__
#define __PTCM2C_SENDGUILDBONUSNTF_H__

// generate by ProtoGen at date: 2016/11/2 11:28:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SENDGUILDBONUSNTF_TYPE 36841

class PtcM2C_SendGuildBonusNtf : public CProtocol
{
public:
    explicit PtcM2C_SendGuildBonusNtf() : CProtocol(PTCM2C_SENDGUILDBONUSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SendGuildBonusNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SendGuildBonusNtfData m_Data;
};

#endif