#ifndef __PTCG2M_UPDATEWEDDINGSTATEG2M_H__
#define __PTCG2M_UPDATEWEDDINGSTATEG2M_H__

// generate by ProtoGen at date: 2017/7/27 21:41:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_UPDATEWEDDINGSTATEG2M_TYPE 23976

class PtcG2M_UpdateWeddingStateG2M : public CProtocol
{
public:
    explicit PtcG2M_UpdateWeddingStateG2M() : CProtocol(PTCG2M_UPDATEWEDDINGSTATEG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_UpdateWeddingStateG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateWeddingStateG2M m_Data;
};

#endif