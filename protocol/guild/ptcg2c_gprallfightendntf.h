#ifndef __PTCG2C_GPRALLFIGHTENDNTF_H__
#define __PTCG2C_GPRALLFIGHTENDNTF_H__

// generate by ProtoGen at date: 2016/10/29 11:10:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GPRALLFIGHTENDNTF_TYPE 58789

class PtcG2C_GprAllFightEndNtf : public CProtocol
{
public:
    explicit PtcG2C_GprAllFightEndNtf() : CProtocol(PTCG2C_GPRALLFIGHTENDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GprAllFightEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GprAllFightEnd m_Data;
};

#endif