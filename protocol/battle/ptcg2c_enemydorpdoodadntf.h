#ifndef __PTCG2C_ENEMYDORPDOODADNTF_H__
#define __PTCG2C_ENEMYDORPDOODADNTF_H__

// generate by ProtoGen at date: 2015/11/25 11:40:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ENEMYDORPDOODADNTF_TYPE 55996

class PtcG2C_EnemyDorpDoodadNtf : public CProtocol
{
public:
    explicit PtcG2C_EnemyDorpDoodadNtf() : CProtocol(PTCG2C_ENEMYDORPDOODADNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_EnemyDorpDoodadNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnemyDropDoodadInfo m_Data;
};

#endif