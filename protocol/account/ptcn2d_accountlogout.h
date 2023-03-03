#ifndef __PTCN2D_ACCOUNTLOGOUT_H__
#define __PTCN2D_ACCOUNTLOGOUT_H__

// generate by ProtoGen at date: 2017/2/27 14:13:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2D_ACCOUNTLOGOUT_TYPE 25855

class PtcN2D_AccountLogout : public CProtocol
{
public:
    explicit PtcN2D_AccountLogout() : CProtocol(PTCN2D_ACCOUNTLOGOUT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2D_AccountLogout()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AccountLogout m_Data;
};

#endif