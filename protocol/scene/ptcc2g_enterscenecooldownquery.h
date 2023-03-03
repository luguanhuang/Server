#ifndef __PTCC2G_ENTERSCENECOOLDOWNQUERY_H__
#define __PTCC2G_ENTERSCENECOOLDOWNQUERY_H__

// generate by ProtoGen at date: 2015/7/30 18:15:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_ENTERSCENECOOLDOWNQUERY_TYPE 40442

class PtcC2G_EnterSceneCoolDownQuery : public CProtocol
{
public:
    explicit PtcC2G_EnterSceneCoolDownQuery() : CProtocol(PTCC2G_ENTERSCENECOOLDOWNQUERY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_EnterSceneCoolDownQuery()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterSceneCoolDownQuery m_Data;
};

#endif