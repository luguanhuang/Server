#ifndef __PTCG2M_NOTIFYSCENEPKWIN_H__
#define __PTCG2M_NOTIFYSCENEPKWIN_H__

// generate by ProtoGen at date: 2016/10/1 20:23:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_NOTIFYSCENEPKWIN_TYPE 18556

class PtcG2M_NotifyScenePkWin : public CProtocol
{
public:
    explicit PtcG2M_NotifyScenePkWin() : CProtocol(PTCG2M_NOTIFYSCENEPKWIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_NotifyScenePkWin()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyScenePkWin m_Data;
};

#endif