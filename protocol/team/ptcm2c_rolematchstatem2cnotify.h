#ifndef __PTCM2C_ROLEMATCHSTATEM2CNOTIFY_H__
#define __PTCM2C_ROLEMATCHSTATEM2CNOTIFY_H__

// generate by ProtoGen at date: 2016/8/19 14:27:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_ROLEMATCHSTATEM2CNOTIFY_TYPE 11521

class PtcM2C_RoleMatchStateM2CNotify : public CProtocol
{
public:
    explicit PtcM2C_RoleMatchStateM2CNotify() : CProtocol(PTCM2C_ROLEMATCHSTATEM2CNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_RoleMatchStateM2CNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleStateMatch m_Data;
};

#endif