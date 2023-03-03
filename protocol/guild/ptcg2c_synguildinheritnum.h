#ifndef __PTCG2C_SYNGUILDINHERITNUM_H__
#define __PTCG2C_SYNGUILDINHERITNUM_H__

// generate by ProtoGen at date: 2016/11/19 10:45:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDINHERITNUM_TYPE 14386

class PtcG2C_SynGuildInheritNum : public CProtocol
{
public:
    explicit PtcG2C_SynGuildInheritNum() : CProtocol(PTCG2C_SYNGUILDINHERITNUM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynGuildInheritNum()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildInheritNum m_Data;
};

#endif