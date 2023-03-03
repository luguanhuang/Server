#ifndef __PTCN2M_NEWROLECREATED_H__
#define __PTCN2M_NEWROLECREATED_H__

// generate by ProtoGen at date: 2016/11/4 0:47:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2M_NEWROLECREATED_TYPE 44687

class PtcN2M_NewRoleCreated : public CProtocol
{
public:
    explicit PtcN2M_NewRoleCreated() : CProtocol(PTCN2M_NEWROLECREATED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2M_NewRoleCreated()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NewRoleCreatedData m_Data;
};

#endif