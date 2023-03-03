#ifndef __PTCG2C_IDIPPUNISHINFONTF_H__
#define __PTCG2C_IDIPPUNISHINFONTF_H__

// generate by ProtoGen at date: 2016/9/19 10:44:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_IDIPPUNISHINFONTF_TYPE 46304

class PtcG2C_IdipPunishInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_IdipPunishInfoNtf() : CProtocol(PTCG2C_IDIPPUNISHINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_IdipPunishInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IdipPunishInfo m_Data;
};

#endif