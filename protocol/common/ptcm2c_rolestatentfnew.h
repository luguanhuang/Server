#ifndef __PTCM2C_ROLESTATENTFNEW_H__
#define __PTCM2C_ROLESTATENTFNEW_H__

// generate by ProtoGen at date: 2016/8/3 11:52:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_ROLESTATENTFNEW_TYPE 62463

class PtcM2C_RoleStateNtfNew : public CProtocol
{
public:
    explicit PtcM2C_RoleStateNtfNew() : CProtocol(PTCM2C_ROLESTATENTFNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_RoleStateNtfNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleStateNtf m_Data;
};

#endif