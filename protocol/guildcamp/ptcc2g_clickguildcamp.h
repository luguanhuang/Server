#ifndef __PTCC2G_CLICKGUILDCAMP_H__
#define __PTCC2G_CLICKGUILDCAMP_H__

// generate by ProtoGen at date: 2016/11/5 15:32:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_CLICKGUILDCAMP_TYPE 32895

class PtcC2G_ClickGuildCamp : public CProtocol
{
public:
    explicit PtcC2G_ClickGuildCamp() : CProtocol(PTCC2G_CLICKGUILDCAMP_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_ClickGuildCamp()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ClickGuildCampArg m_Data;
};

#endif