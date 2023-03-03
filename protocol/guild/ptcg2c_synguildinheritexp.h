#ifndef __PTCG2C_SYNGUILDINHERITEXP_H__
#define __PTCG2C_SYNGUILDINHERITEXP_H__

// generate by ProtoGen at date: 2016/11/17 19:53:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDINHERITEXP_TYPE 15872

class PtcG2C_synGuildInheritExp : public CProtocol
{
public:
    explicit PtcG2C_synGuildInheritExp() : CProtocol(PTCG2C_SYNGUILDINHERITEXP_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_synGuildInheritExp()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::synGuildInheritExp m_Data;
};

#endif