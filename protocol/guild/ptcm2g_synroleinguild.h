#ifndef __PTCM2G_SYNROLEINGUILD_H__
#define __PTCM2G_SYNROLEINGUILD_H__

// generate by ProtoGen at date: 2016/8/29 15:41:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNROLEINGUILD_TYPE 24851

class PtcM2G_SynRoleInGuild : public CProtocol
{
public:
    explicit PtcM2G_SynRoleInGuild() : CProtocol(PTCM2G_SYNROLEINGUILD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynRoleInGuild()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynRoleInGuild m_Data;
};

#endif