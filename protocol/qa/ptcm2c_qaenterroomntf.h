#ifndef __PTCM2C_QAENTERROOMNTF_H__
#define __PTCM2C_QAENTERROOMNTF_H__

// generate by ProtoGen at date: 2017/2/7 3:14:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_QAENTERROOMNTF_TYPE 38488

class PtcM2C_QAEnterRoomNtf : public CProtocol
{
public:
    explicit PtcM2C_QAEnterRoomNtf() : CProtocol(PTCM2C_QAENTERROOMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_QAEnterRoomNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QAEnterRoomNtf m_Data;
};

#endif