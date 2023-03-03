#ifndef __PTCG2C_SYNATLASATTR_H__
#define __PTCG2C_SYNATLASATTR_H__

// generate by ProtoGen at date: 2016/9/2 22:11:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNATLASATTR_TYPE 1285

class PtcG2C_SynAtlasAttr : public CProtocol
{
public:
    explicit PtcG2C_SynAtlasAttr() : CProtocol(PTCG2C_SYNATLASATTR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynAtlasAttr()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AllSynCardAttr m_Data;
};

#endif