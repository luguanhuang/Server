#ifndef __PTCM2C_HALLICONMNTF_H__
#define __PTCM2C_HALLICONMNTF_H__

// generate by ProtoGen at date: 2016/11/16 23:07:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_HALLICONMNTF_TYPE 51500

class PtcM2C_HallIconMNtf : public CProtocol
{
public:
    explicit PtcM2C_HallIconMNtf() : CProtocol(PTCM2C_HALLICONMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_HallIconMNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HallIconPara m_Data;
};

#endif