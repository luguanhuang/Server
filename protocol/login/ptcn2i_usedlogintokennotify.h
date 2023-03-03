#ifndef __PTCN2I_USEDLOGINTOKENNOTIFY_H__
#define __PTCN2I_USEDLOGINTOKENNOTIFY_H__

// generate by ProtoGen at date: 2016/11/3 10:26:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2I_USEDLOGINTOKENNOTIFY_TYPE 2308

class PtcN2I_UsedLoginTokenNotify : public CProtocol
{
public:
    explicit PtcN2I_UsedLoginTokenNotify() : CProtocol(PTCN2I_USEDLOGINTOKENNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2I_UsedLoginTokenNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TokenNoitfyData m_Data;
};

#endif