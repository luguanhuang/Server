#ifndef __PTCG2C_IBSHOPICON_H__
#define __PTCG2C_IBSHOPICON_H__

// generate by ProtoGen at date: 2016/9/7 20:28:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_IBSHOPICON_TYPE 56800

class PtcG2C_IBShopIcon : public CProtocol
{
public:
    explicit PtcG2C_IBShopIcon() : CProtocol(PTCG2C_IBSHOPICON_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_IBShopIcon()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IBShopIcon m_Data;
};

#endif