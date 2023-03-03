#ifndef __PTCG2C_NOTIFYGUILDBOSSADDATTR_H__
#define __PTCG2C_NOTIFYGUILDBOSSADDATTR_H__

// generate by ProtoGen at date: 2016/7/23 16:46:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYGUILDBOSSADDATTR_TYPE 42027

class PtcG2C_NotifyGuildBossAddAttr : public CProtocol
{
public:
    explicit PtcG2C_NotifyGuildBossAddAttr() : CProtocol(PTCG2C_NOTIFYGUILDBOSSADDATTR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyGuildBossAddAttr()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddAttrCount m_Data;
};

#endif