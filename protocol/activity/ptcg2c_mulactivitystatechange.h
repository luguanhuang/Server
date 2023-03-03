#ifndef __PTCG2C_MULACTIVITYSTATECHANGE_H__
#define __PTCG2C_MULACTIVITYSTATECHANGE_H__

// generate by ProtoGen at date: 2016/7/20 17:34:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MULACTIVITYSTATECHANGE_TYPE 13448

class PtcG2C_MulActivityStateChange : public CProtocol
{
public:
    explicit PtcG2C_MulActivityStateChange() : CProtocol(PTCG2C_MULACTIVITYSTATECHANGE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MulActivityStateChange()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MulActivityCha m_Data;
};

#endif