#ifndef __PTCM2G_UPDATEGUILDQAROOM_H__
#define __PTCM2G_UPDATEGUILDQAROOM_H__

// generate by ProtoGen at date: 2016/10/9 14:44:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_UPDATEGUILDQAROOM_TYPE 64072

class PtcM2G_UpdateGuildQARoom : public CProtocol
{
public:
    explicit PtcM2G_UpdateGuildQARoom() : CProtocol(PTCM2G_UPDATEGUILDQAROOM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_UpdateGuildQARoom()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QARoomData m_Data;
};

#endif