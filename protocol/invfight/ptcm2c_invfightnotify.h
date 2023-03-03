#ifndef __PTCM2C_INVFIGHTNOTIFY_H__
#define __PTCM2C_INVFIGHTNOTIFY_H__

// generate by ProtoGen at date: 2016/12/13 23:25:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_INVFIGHTNOTIFY_TYPE 38172

class PtcM2C_InvFightNotify : public CProtocol
{
public:
    explicit PtcM2C_InvFightNotify() : CProtocol(PTCM2C_INVFIGHTNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_InvFightNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvFightPara m_Data;
};

#endif