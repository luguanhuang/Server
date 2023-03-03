#ifndef __PTCG2C_CORRECTPOSITION_H__
#define __PTCG2C_CORRECTPOSITION_H__

// generate by ProtoGen at date: 2016/3/10 20:48:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CORRECTPOSITION_TYPE 53665

class PtcG2C_CorrectPosition : public CProtocol
{
public:
    explicit PtcG2C_CorrectPosition() : CProtocol(PTCG2C_CORRECTPOSITION_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CorrectPosition()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::Position m_Data;
};

#endif