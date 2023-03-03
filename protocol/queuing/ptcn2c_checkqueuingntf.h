#ifndef __PTCN2C_CHECKQUEUINGNTF_H__
#define __PTCN2C_CHECKQUEUINGNTF_H__

// generate by ProtoGen at date: 2016/11/16 16:37:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2C_CHECKQUEUINGNTF_TYPE 25553

class PtcN2C_CheckQueuingNtf : public CProtocol
{
public:
    explicit PtcN2C_CheckQueuingNtf() : CProtocol(PTCN2C_CHECKQUEUINGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2C_CheckQueuingNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CheckQueuingNtf m_Data;
};

#endif