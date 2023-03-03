#ifndef __PTCM2G_SYNSCENEREWARDM2G_H__
#define __PTCM2G_SYNSCENEREWARDM2G_H__

// generate by ProtoGen at date: 2016/8/5 19:32:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNSCENEREWARDM2G_TYPE 59141

class PtcM2G_SynSceneRewardM2G : public CProtocol
{
public:
    explicit PtcM2G_SynSceneRewardM2G() : CProtocol(PTCM2G_SYNSCENEREWARDM2G_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynSceneRewardM2G()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildExpAndConData m_Data;
};

#endif