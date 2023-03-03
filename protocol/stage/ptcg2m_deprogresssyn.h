#ifndef __PTCG2M_DEPROGRESSSYN_H__
#define __PTCG2M_DEPROGRESSSYN_H__

// generate by ProtoGen at date: 2016/9/6 10:50:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_DEPROGRESSSYN_TYPE 36322

class PtcG2M_DEProgressSyn : public CProtocol
{
public:
    explicit PtcG2M_DEProgressSyn() : CProtocol(PTCG2M_DEPROGRESSSYN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_DEProgressSyn()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DEProgressSynPara m_Data;
};

#endif