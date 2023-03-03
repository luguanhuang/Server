#ifndef __PTCG2C_SKILLRESULT_H__
#define __PTCG2C_SKILLRESULT_H__

// generate by ProtoGen at date: 2015/12/16 10:14:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKILLRESULT_TYPE 1054

class PtcG2C_SkillResult : public CProtocol
{
public:
    explicit PtcG2C_SkillResult() : CProtocol(PTCG2C_SKILLRESULT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkillResult()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillReplyDataUnit m_Data;
};

#endif