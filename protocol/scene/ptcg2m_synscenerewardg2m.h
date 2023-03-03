#ifndef __PTCG2M_SYNSCENEREWARDG2M_H__
#define __PTCG2M_SYNSCENEREWARDG2M_H__

// generate by ProtoGen at date: 2016/8/5 19:31:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNSCENEREWARDG2M_TYPE 20148

class PtcG2M_SynSceneRewardG2M : public CProtocol
{
public:
    explicit PtcG2M_SynSceneRewardG2M() : CProtocol(PTCG2M_SYNSCENEREWARDG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SynSceneRewardG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildExpAndConData m_Data;
};

#endif