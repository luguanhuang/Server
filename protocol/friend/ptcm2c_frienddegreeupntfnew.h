#ifndef __PTCM2C_FRIENDDEGREEUPNTFNEW_H__
#define __PTCM2C_FRIENDDEGREEUPNTFNEW_H__

// generate by ProtoGen at date: 2016/7/29 0:20:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_FRIENDDEGREEUPNTFNEW_TYPE 36126

class PtcM2C_FriendDegreeUpNtfNew : public CProtocol
{
public:
    explicit PtcM2C_FriendDegreeUpNtfNew() : CProtocol(PTCM2C_FRIENDDEGREEUPNTFNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_FriendDegreeUpNtfNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FriendDegreeUpNtf m_Data;
};

#endif