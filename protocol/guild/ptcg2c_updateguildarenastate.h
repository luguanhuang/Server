#ifndef __PTCG2C_UPDATEGUILDARENASTATE_H__
#define __PTCG2C_UPDATEGUILDARENASTATE_H__

// generate by ProtoGen at date: 2016/7/18 11:58:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATEGUILDARENASTATE_TYPE 21909

class PtcG2C_UpdateGuildArenaState : public CProtocol
{
public:
    explicit PtcG2C_UpdateGuildArenaState() : CProtocol(PTCG2C_UPDATEGUILDARENASTATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdateGuildArenaState()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateGuildArenaState m_Data;
};

#endif