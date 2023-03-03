#ifndef __PTCM2G_WORLDBOSSSYNCGS_H__
#define __PTCM2G_WORLDBOSSSYNCGS_H__

// generate by ProtoGen at date: 2016/8/8 11:45:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_WORLDBOSSSYNCGS_TYPE 59176

class PtcM2G_WorldBossSyncGS : public CProtocol
{
public:
    explicit PtcM2G_WorldBossSyncGS() : CProtocol(PTCM2G_WORLDBOSSSYNCGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_WorldBossSyncGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldBossSyncGS m_Data;
};

#endif