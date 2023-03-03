#ifndef __PTCG2C_SKILLCOOLNTF_H__
#define __PTCG2C_SKILLCOOLNTF_H__

// generate by ProtoGen at date: 2016/10/18 22:05:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKILLCOOLNTF_TYPE 55142

class PtcG2C_SkillCoolNtf : public CProtocol
{
public:
    explicit PtcG2C_SkillCoolNtf() : CProtocol(PTCG2C_SKILLCOOLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkillCoolNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillCoolPara m_Data;
};

#endif