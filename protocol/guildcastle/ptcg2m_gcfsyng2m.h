#ifndef __PTCG2M_GCFSYNG2M_H__
#define __PTCG2M_GCFSYNG2M_H__

// generate by ProtoGen at date: 2016/12/19 10:33:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GCFSYNG2M_TYPE 58155

class PtcG2M_GCFSynG2M : public CProtocol
{
public:
    explicit PtcG2M_GCFSynG2M() : CProtocol(PTCG2M_GCFSYNG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GCFSynG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GCFSynPara m_Data;
};

#endif