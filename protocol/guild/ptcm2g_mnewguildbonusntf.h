#ifndef __PTCM2G_MNEWGUILDBONUSNTF_H__
#define __PTCM2G_MNEWGUILDBONUSNTF_H__

// generate by ProtoGen at date: 2016/9/18 21:23:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_MNEWGUILDBONUSNTF_TYPE 7302

class PtcM2G_MNewGuildBonusNtf : public CProtocol
{
public:
    explicit PtcM2G_MNewGuildBonusNtf() : CProtocol(PTCM2G_MNEWGUILDBONUSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_MNewGuildBonusNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MNewGuildBonusData m_Data;
};

#endif