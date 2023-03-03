#ifndef __PTCG2M_WORLDBOSSSYNCMS_H__
#define __PTCG2M_WORLDBOSSSYNCMS_H__

// generate by ProtoGen at date: 2016/8/8 11:45:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_WORLDBOSSSYNCMS_TYPE 58842

class PtcG2M_WorldBossSyncMS : public CProtocol
{
public:
    explicit PtcG2M_WorldBossSyncMS() : CProtocol(PTCG2M_WORLDBOSSSYNCMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_WorldBossSyncMS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldBossSyncMS m_Data;
};

#endif