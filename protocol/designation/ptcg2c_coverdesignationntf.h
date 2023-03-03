#ifndef __PTCG2C_COVERDESIGNATIONNTF_H__
#define __PTCG2C_COVERDESIGNATIONNTF_H__

// generate by ProtoGen at date: 2016/6/2 15:51:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_COVERDESIGNATIONNTF_TYPE 45821

class PtcG2C_CoverDesignationNtf : public CProtocol
{
public:
    explicit PtcG2C_CoverDesignationNtf() : CProtocol(PTCG2C_COVERDESIGNATIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CoverDesignationNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CoverDesignationNtf m_Data;
};

#endif