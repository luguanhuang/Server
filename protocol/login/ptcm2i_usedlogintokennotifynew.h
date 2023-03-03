#ifndef __PTCM2I_USEDLOGINTOKENNOTIFYNEW_H__
#define __PTCM2I_USEDLOGINTOKENNOTIFYNEW_H__

// generate by ProtoGen at date: 2016/8/4 15:18:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2I_USEDLOGINTOKENNOTIFYNEW_TYPE 2308

class PtcM2I_UsedLoginTokenNotifyNew : public CProtocol
{
public:
    explicit PtcM2I_UsedLoginTokenNotifyNew() : CProtocol(PTCM2I_USEDLOGINTOKENNOTIFYNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2I_UsedLoginTokenNotifyNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TokenNoitfyData m_Data;
};

#endif