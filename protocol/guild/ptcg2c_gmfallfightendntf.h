#ifndef __PTCG2C_GMFALLFIGHTENDNTF_H__
#define __PTCG2C_GMFALLFIGHTENDNTF_H__

// generate by ProtoGen at date: 2016/7/21 1:43:08

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFALLFIGHTENDNTF_TYPE 42921

class PtcG2C_GmfAllFightEndNtf : public CProtocol
{
public:
    explicit PtcG2C_GmfAllFightEndNtf() : CProtocol(PTCG2C_GMFALLFIGHTENDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfAllFightEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfAllFightEnd m_Data;
};

#endif