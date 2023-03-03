#ifndef __PTCG2C_TRANSSKILLNOTFIY_H__
#define __PTCG2C_TRANSSKILLNOTFIY_H__

// generate by ProtoGen at date: 2017/6/11 22:40:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TRANSSKILLNOTFIY_TYPE 1366

class PtcG2C_TransSkillNotfiy : public CProtocol
{
public:
    explicit PtcG2C_TransSkillNotfiy() : CProtocol(PTCG2C_TRANSSKILLNOTFIY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TransSkillNotfiy()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TransSkillNotfiy m_Data;
};

#endif