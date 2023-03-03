#ifndef __PTCG2C_PLAYDICENTF_H__
#define __PTCG2C_PLAYDICENTF_H__

// generate by ProtoGen at date: 2016/9/5 16:05:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PLAYDICENTF_TYPE 50453

class PtcG2C_PlayDiceNtf : public CProtocol
{
public:
    explicit PtcG2C_PlayDiceNtf() : CProtocol(PTCG2C_PLAYDICENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PlayDiceNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PlayDiceNtfData m_Data;
};

#endif