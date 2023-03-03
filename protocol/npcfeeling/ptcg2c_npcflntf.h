#ifndef __PTCG2C_NPCFLNTF_H__
#define __PTCG2C_NPCFLNTF_H__

// generate by ProtoGen at date: 2017/9/18 14:59:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NPCFLNTF_TYPE 18961

class PtcG2C_NpcFlNtf : public CProtocol
{
public:
    explicit PtcG2C_NpcFlNtf() : CProtocol(PTCG2C_NPCFLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NpcFlNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NpcFlRes m_Data;
};

#endif