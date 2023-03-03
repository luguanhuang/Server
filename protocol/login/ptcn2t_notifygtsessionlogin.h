#ifndef __PTCN2T_NOTIFYGTSESSIONLOGIN_H__
#define __PTCN2T_NOTIFYGTSESSIONLOGIN_H__

// generate by ProtoGen at date: 2016/11/28 14:10:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2T_NOTIFYGTSESSIONLOGIN_TYPE 65202

class PtcN2T_NotifyGtSessionLogin : public CProtocol
{
public:
    explicit PtcN2T_NotifyGtSessionLogin() : CProtocol(PTCN2T_NOTIFYGTSESSIONLOGIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2T_NotifyGtSessionLogin()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotfiyGtSessionLogin m_Data;
};

#endif