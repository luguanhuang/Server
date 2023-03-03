#ifndef __PTCM2C_SYNGUILDINTEGRALSTATE_H__
#define __PTCM2C_SYNGUILDINTEGRALSTATE_H__

// generate by ProtoGen at date: 2016/10/25 19:33:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SYNGUILDINTEGRALSTATE_TYPE 4104

class PtcM2C_SynGuildIntegralState : public CProtocol
{
public:
    explicit PtcM2C_SynGuildIntegralState() : CProtocol(PTCM2C_SYNGUILDINTEGRALSTATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SynGuildIntegralState()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildIntegralState m_Data;
};

#endif