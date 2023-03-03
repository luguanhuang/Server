#ifndef __PTCG2C_SKILLCHANGEDNTF_H__
#define __PTCG2C_SKILLCHANGEDNTF_H__

// generate by ProtoGen at date: 2015/4/14 14:15:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKILLCHANGEDNTF_TYPE 38872

class PtcG2C_SkillChangedNtf : public CProtocol
{
public:
    explicit PtcG2C_SkillChangedNtf() : CProtocol(PTCG2C_SKILLCHANGEDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkillChangedNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillChangedData m_Data;
};

#endif