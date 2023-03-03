#ifndef __PTCM2C_HALLICONMSNTF_H__
#define __PTCM2C_HALLICONMSNTF_H__

// generate by ProtoGen at date: 2017/9/30 15:03:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_HALLICONMSNTF_TYPE 49540

class PtcM2C_HallIconMsNtf : public CProtocol
{
public:
    explicit PtcM2C_HallIconMsNtf() : CProtocol(PTCM2C_HALLICONMSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_HallIconMsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HallIconPara m_Data;
};

#endif