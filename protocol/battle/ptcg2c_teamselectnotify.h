#ifndef __PTCG2C_TEAMSELECTNOTIFY_H__
#define __PTCG2C_TEAMSELECTNOTIFY_H__

// generate by ProtoGen at date: 2015/11/26 21:23:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TEAMSELECTNOTIFY_TYPE 25174

class PtcG2C_TeamSelectNotify : public CProtocol
{
public:
    explicit PtcG2C_TeamSelectNotify() : CProtocol(PTCG2C_TEAMSELECTNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TeamSelectNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamSelect m_Data;
};

#endif