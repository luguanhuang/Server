#ifndef __PTCM2D_GUILDAUCTUPDATENTF_H__
#define __PTCM2D_GUILDAUCTUPDATENTF_H__

// generate by ProtoGen at date: 2016/11/2 20:10:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_GUILDAUCTUPDATENTF_TYPE 61195

class PtcM2D_GuildAuctUpdateNtf : public CProtocol
{
public:
    explicit PtcM2D_GuildAuctUpdateNtf() : CProtocol(PTCM2D_GUILDAUCTUPDATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_GuildAuctUpdateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildAuctPara m_Data;
};

#endif