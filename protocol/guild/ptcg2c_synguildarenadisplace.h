#ifndef __PTCG2C_SYNGUILDARENADISPLACE_H__
#define __PTCG2C_SYNGUILDARENADISPLACE_H__

// generate by ProtoGen at date: 2016/7/30 19:20:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDARENADISPLACE_TYPE 21037

class PtcG2C_synguildarenadisplace : public CProtocol
{
public:
    explicit PtcG2C_synguildarenadisplace() : CProtocol(PTCG2C_SYNGUILDARENADISPLACE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_synguildarenadisplace()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::guildarenadisplace m_Data;
};

#endif