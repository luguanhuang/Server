#ifndef __PTCM2G_SYNGUILDSIMPLEDATA_H__
#define __PTCM2G_SYNGUILDSIMPLEDATA_H__

// generate by ProtoGen at date: 2016/9/12 20:33:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNGUILDSIMPLEDATA_TYPE 49439

class PtcM2G_SynGuildSimpleData : public CProtocol
{
public:
    explicit PtcM2G_SynGuildSimpleData() : CProtocol(PTCM2G_SYNGUILDSIMPLEDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynGuildSimpleData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildSimpleData m_Data;
};

#endif