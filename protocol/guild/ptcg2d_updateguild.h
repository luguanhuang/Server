#ifndef __PTCG2D_UPDATEGUILD_H__
#define __PTCG2D_UPDATEGUILD_H__

// generate by ProtoGen at date: 2016/12/27 23:57:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_UPDATEGUILD_TYPE 63741

class PtcG2D_UpdateGuild : public CProtocol
{
public:
    explicit PtcG2D_UpdateGuild() : CProtocol(PTCG2D_UPDATEGUILD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2D_UpdateGuild()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildAllDBData m_Data;
};

#endif