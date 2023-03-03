#ifndef __PTCG2C_SYNCARDATTR_H__
#define __PTCG2C_SYNCARDATTR_H__

// generate by ProtoGen at date: 2016/9/2 21:48:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNCARDATTR_TYPE 5344

class PtcG2C_SynCardAttr : public CProtocol
{
public:
    explicit PtcG2C_SynCardAttr() : CProtocol(PTCG2C_SYNCARDATTR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynCardAttr()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AllSynCardAttr m_Data;
};

#endif