#ifndef __PTCM2C_RESWARSTATENTF_H__
#define __PTCM2C_RESWARSTATENTF_H__

// generate by ProtoGen at date: 2016/11/8 17:53:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARSTATENTF_TYPE 18481

class PtcM2C_ResWarStateNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarStateNtf() : CProtocol(PTCM2C_RESWARSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarStateInfo m_Data;
};

#endif