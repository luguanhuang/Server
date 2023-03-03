#ifndef __PTCG2C_LEVELCHANGENTF_H__
#define __PTCG2C_LEVELCHANGENTF_H__

// generate by ProtoGen at date: 2015/1/23 11:18:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEVELCHANGENTF_TYPE 38651

class PtcG2C_LevelChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_LevelChangeNtf() : CProtocol(PTCG2C_LEVELCHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LevelChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LevelChanged m_Data;
};

#endif