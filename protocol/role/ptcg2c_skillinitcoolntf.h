#ifndef __PTCG2C_SKILLINITCOOLNTF_H__
#define __PTCG2C_SKILLINITCOOLNTF_H__

// generate by ProtoGen at date: 2017/1/4 16:17:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKILLINITCOOLNTF_TYPE 4132

class PtcG2C_SkillInitCoolNtf : public CProtocol
{
public:
    explicit PtcG2C_SkillInitCoolNtf() : CProtocol(PTCG2C_SKILLINITCOOLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkillInitCoolNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillInitCoolPara m_Data;
};

#endif