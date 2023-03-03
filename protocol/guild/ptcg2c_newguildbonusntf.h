#ifndef __PTCG2C_NEWGUILDBONUSNTF_H__
#define __PTCG2C_NEWGUILDBONUSNTF_H__

// generate by ProtoGen at date: 2016/6/15 9:53:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NEWGUILDBONUSNTF_TYPE 33515

class PtcG2C_NewGuildBonusNtf : public CProtocol
{
public:
    explicit PtcG2C_NewGuildBonusNtf() : CProtocol(PTCG2C_NEWGUILDBONUSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NewGuildBonusNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NewGuildBonusData m_Data;
};

#endif