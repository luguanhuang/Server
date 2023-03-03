#ifndef __PTCM2C_DERANKCHANGNTF_H__
#define __PTCM2C_DERANKCHANGNTF_H__

// generate by ProtoGen at date: 2016/9/19 12:42:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_DERANKCHANGNTF_TYPE 11404

class PtcM2C_DERankChangNtf : public CProtocol
{
public:
    explicit PtcM2C_DERankChangNtf() : CProtocol(PTCM2C_DERANKCHANGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_DERankChangNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DERankChangePara m_Data;
};

#endif