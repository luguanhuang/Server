#ifndef __PTCM2C_SYNGUILINTEGRALSTATE_H__
#define __PTCM2C_SYNGUILINTEGRALSTATE_H__

// generate by ProtoGen at date: 2016/11/7 21:22:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SYNGUILINTEGRALSTATE_TYPE 28075

class PtcM2C_SynGuilIntegralState : public CProtocol
{
public:
    explicit PtcM2C_SynGuilIntegralState() : CProtocol(PTCM2C_SYNGUILINTEGRALSTATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SynGuilIntegralState()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuilIntegralState m_Data;
};

#endif