#ifndef __PTCG2C_DOODADITEMSKILLSNTF_H__
#define __PTCG2C_DOODADITEMSKILLSNTF_H__

// generate by ProtoGen at date: 2017/9/23 15:52:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_DOODADITEMSKILLSNTF_TYPE 45490

class PtcG2C_DoodadItemSkillsNtf : public CProtocol
{
public:
    explicit PtcG2C_DoodadItemSkillsNtf() : CProtocol(PTCG2C_DOODADITEMSKILLSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_DoodadItemSkillsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DoodadItemAllSkill m_Data;
};

#endif