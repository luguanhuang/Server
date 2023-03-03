#ifndef __PTCM2G_SYNDRAGONGUILDSIMPLEDATA_H__
#define __PTCM2G_SYNDRAGONGUILDSIMPLEDATA_H__

// generate by ProtoGen at date: 2017/9/6 15:14:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNDRAGONGUILDSIMPLEDATA_TYPE 35494

class PtcM2G_SynDragonGuildSimpleData : public CProtocol
{
public:
    explicit PtcM2G_SynDragonGuildSimpleData() : CProtocol(PTCM2G_SYNDRAGONGUILDSIMPLEDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynDragonGuildSimpleData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynDragonGuildSimpleData m_Data;
};

#endif