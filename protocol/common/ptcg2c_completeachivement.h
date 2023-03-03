#ifndef __PTCG2C_COMPLETEACHIVEMENT_H__
#define __PTCG2C_COMPLETEACHIVEMENT_H__

// generate by ProtoGen at date: 2015/2/4 19:48:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_COMPLETEACHIVEMENT_TYPE 26346

class PtcG2C_CompleteAchivement : public CProtocol
{
public:
    explicit PtcG2C_CompleteAchivement() : CProtocol(PTCG2C_COMPLETEACHIVEMENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CompleteAchivement()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AchivementInfo m_Data;
};

#endif