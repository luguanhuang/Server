#ifndef __PTCG2C_WORLDCHANNELLEFTTIMESNTF_H__
#define __PTCG2C_WORLDCHANNELLEFTTIMESNTF_H__

// generate by ProtoGen at date: 2016/7/25 12:08:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WORLDCHANNELLEFTTIMESNTF_TYPE 37503

class PtcG2C_WorldChannelLeftTimesNtf : public CProtocol
{
public:
    explicit PtcG2C_WorldChannelLeftTimesNtf() : CProtocol(PTCG2C_WORLDCHANNELLEFTTIMESNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WorldChannelLeftTimesNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldChannelLeftTimesNtf m_Data;
};

#endif