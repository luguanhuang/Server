#ifndef __PTCG2C_HORSERANKNTF_H__
#define __PTCG2C_HORSERANKNTF_H__

// generate by ProtoGen at date: 2016/12/6 20:39:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSERANKNTF_TYPE 22250

class PtcG2C_HorseRankNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseRankNtf() : CProtocol(PTCG2C_HORSERANKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HorseRankNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HorseRank m_Data;
};

#endif