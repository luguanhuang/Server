#ifndef __PTCM2K_CUSTOMBATTLEDELMATCH_H__
#define __PTCM2K_CUSTOMBATTLEDELMATCH_H__

// generate by ProtoGen at date: 2017/5/8 21:19:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_CUSTOMBATTLEDELMATCH_TYPE 34876

class PtcM2K_CustomBattleDelMatch : public CProtocol
{
public:
    explicit PtcM2K_CustomBattleDelMatch() : CProtocol(PTCM2K_CUSTOMBATTLEDELMATCH_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_CustomBattleDelMatch()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CustomBattleDelMatch m_Data;
};

#endif