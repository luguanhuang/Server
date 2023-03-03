#ifndef __PTCG2C_GMFGUILDCOMBATNTF_H__
#define __PTCG2C_GMFGUILDCOMBATNTF_H__

// generate by ProtoGen at date: 2016/10/29 11:01:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFGUILDCOMBATNTF_TYPE 55102

class PtcG2C_GmfGuildCombatNtf : public CProtocol
{
public:
    explicit PtcG2C_GmfGuildCombatNtf() : CProtocol(PTCG2C_GMFGUILDCOMBATNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfGuildCombatNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfGuildCombatPara m_Data;
};

#endif