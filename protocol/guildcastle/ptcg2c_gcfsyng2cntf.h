#ifndef __PTCG2C_GCFSYNG2CNTF_H__
#define __PTCG2C_GCFSYNG2CNTF_H__

// generate by ProtoGen at date: 2017/2/7 21:48:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GCFSYNG2CNTF_TYPE 31469

class PtcG2C_GCFSynG2CNtf : public CProtocol
{
public:
    explicit PtcG2C_GCFSynG2CNtf() : CProtocol(PTCG2C_GCFSYNG2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GCFSynG2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GCFG2CSynPara m_Data;
};

#endif