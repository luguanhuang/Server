#ifndef __PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_H__
#define __PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_H__

// generate by ProtoGen at date: 2017/7/13 20:25:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_TYPE 64412

class PtcG2C_NotifyStartUpTypeToClient : public CProtocol
{
public:
    explicit PtcG2C_NotifyStartUpTypeToClient() : CProtocol(PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyStartUpTypeToClient()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyStartUpTypeToClient m_Data;
};

#endif