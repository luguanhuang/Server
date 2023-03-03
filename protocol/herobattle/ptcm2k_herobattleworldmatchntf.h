#ifndef __PTCM2K_HEROBATTLEWORLDMATCHNTF_H__
#define __PTCM2K_HEROBATTLEWORLDMATCHNTF_H__

// generate by ProtoGen at date: 2017/2/4 19:26:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_HEROBATTLEWORLDMATCHNTF_TYPE 15732

class PtcM2K_HeroBattleWorldMatchNtf : public CProtocol
{
public:
    explicit PtcM2K_HeroBattleWorldMatchNtf() : CProtocol(PTCM2K_HEROBATTLEWORLDMATCHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_HeroBattleWorldMatchNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleWorldMatchData m_Data;
};

#endif