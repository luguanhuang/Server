#ifndef __PTCG2M_SYNADDGUILDACTIVITY_H__
#define __PTCG2M_SYNADDGUILDACTIVITY_H__

// generate by ProtoGen at date: 2016/9/16 17:12:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNADDGUILDACTIVITY_TYPE 49479

class PtcG2M_SynAddGuildActivity : public CProtocol
{
public:
    explicit PtcG2M_SynAddGuildActivity() : CProtocol(PTCG2M_SYNADDGUILDACTIVITY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SynAddGuildActivity()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynAddGuildActivityArg m_Data;
};

#endif