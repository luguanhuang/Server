#ifndef __PTCG2C_TEAMSELECTBOXNOTIFY_H__
#define __PTCG2C_TEAMSELECTBOXNOTIFY_H__

// generate by ProtoGen at date: 2015/11/26 21:23:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TEAMSELECTBOXNOTIFY_TYPE 64728

class PtcG2C_TeamSelectBoxNotify : public CProtocol
{
public:
    explicit PtcG2C_TeamSelectBoxNotify() : CProtocol(PTCG2C_TEAMSELECTBOXNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TeamSelectBoxNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamAllSelectBox m_Data;
};

#endif