#ifndef __PTCM2G_SYNGUILDINFO_H__
#define __PTCM2G_SYNGUILDINFO_H__

// generate by ProtoGen at date: 2016/8/31 17:53:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNGUILDINFO_TYPE 42824

class PtcM2G_SynGuildInfo : public CProtocol
{
public:
    explicit PtcM2G_SynGuildInfo() : CProtocol(PTCM2G_SYNGUILDINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynGuildInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildInfo m_Data;
};

#endif