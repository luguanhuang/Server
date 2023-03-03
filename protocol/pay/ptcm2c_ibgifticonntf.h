#ifndef __PTCM2C_IBGIFTICONNTF_H__
#define __PTCM2C_IBGIFTICONNTF_H__

// generate by ProtoGen at date: 2017/5/2 11:27:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_IBGIFTICONNTF_TYPE 44659

class PtcM2C_IBGiftIconNtf : public CProtocol
{
public:
    explicit PtcM2C_IBGiftIconNtf() : CProtocol(PTCM2C_IBGIFTICONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_IBGiftIconNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IBGiftIcon m_Data;
};

#endif