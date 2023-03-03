#ifndef __PTCM2F_LEAVEFMLARGEROOM_H__
#define __PTCM2F_LEAVEFMLARGEROOM_H__

// generate by ProtoGen at date: 2016/11/25 17:01:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_LEAVEFMLARGEROOM_TYPE 51478

class PtcM2F_LeaveFMLargeRoom : public CProtocol
{
public:
    explicit PtcM2F_LeaveFMLargeRoom() : CProtocol(PTCM2F_LEAVEFMLARGEROOM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_LeaveFMLargeRoom()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeaveLargeRoom m_Data;
};

#endif