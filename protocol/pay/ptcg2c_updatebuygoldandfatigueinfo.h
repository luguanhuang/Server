#ifndef __PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_H__
#define __PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_H__

// generate by ProtoGen at date: 2015/7/20 10:16:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_TYPE 2587

class PtcG2C_UpdateBuyGoldAndFatigueInfo : public CProtocol
{
public:
    explicit PtcG2C_UpdateBuyGoldAndFatigueInfo() : CProtocol(PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdateBuyGoldAndFatigueInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BuyGoldFatInfo m_Data;
};

#endif