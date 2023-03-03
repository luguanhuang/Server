#ifndef __PTCM2G_SYNGUILDBOSSDOEND_H__
#define __PTCM2G_SYNGUILDBOSSDOEND_H__

// generate by ProtoGen at date: 2017/1/14 19:34:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNGUILDBOSSDOEND_TYPE 51634

class PtcM2G_SynGuildBossDoEnd : public CProtocol
{
public:
    explicit PtcM2G_SynGuildBossDoEnd() : CProtocol(PTCM2G_SYNGUILDBOSSDOEND_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynGuildBossDoEnd()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildBossDoEnd m_Data;
};

#endif