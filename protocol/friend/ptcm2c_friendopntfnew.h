#ifndef __PTCM2C_FRIENDOPNTFNEW_H__
#define __PTCM2C_FRIENDOPNTFNEW_H__

// generate by ProtoGen at date: 2016/7/28 15:53:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_FRIENDOPNTFNEW_TYPE 22609

class PtcM2C_FriendOpNtfNew : public CProtocol
{
public:
    explicit PtcM2C_FriendOpNtfNew() : CProtocol(PTCM2C_FRIENDOPNTFNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_FriendOpNtfNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FriendOpNotify m_Data;
};

#endif