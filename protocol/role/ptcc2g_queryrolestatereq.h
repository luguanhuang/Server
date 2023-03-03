#ifndef __PTCC2G_QUERYROLESTATEREQ_H__
#define __PTCC2G_QUERYROLESTATEREQ_H__

// generate by ProtoGen at date: 2016/12/28 11:10:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_QUERYROLESTATEREQ_TYPE 54208

class PtcC2G_QueryRoleStateReq : public CProtocol
{
public:
    explicit PtcC2G_QueryRoleStateReq() : CProtocol(PTCC2G_QUERYROLESTATEREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_QueryRoleStateReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QueryRoleStateReq m_Data;
};

#endif