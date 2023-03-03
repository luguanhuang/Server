#ifndef __PTCM2C_QAROOMRANKNTF_H__
#define __PTCM2C_QAROOMRANKNTF_H__

// generate by ProtoGen at date: 2016/10/9 11:19:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_QAROOMRANKNTF_TYPE 36888

class PtcM2C_QARoomRankNtf : public CProtocol
{
public:
    explicit PtcM2C_QARoomRankNtf() : CProtocol(PTCM2C_QAROOMRANKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_QARoomRankNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QARoomRankNtf m_Data;
};

#endif