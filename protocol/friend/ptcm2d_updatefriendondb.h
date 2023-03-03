#ifndef __PTCM2D_UPDATEFRIENDONDB_H__
#define __PTCM2D_UPDATEFRIENDONDB_H__

// generate by ProtoGen at date: 2016/7/30 15:19:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_UPDATEFRIENDONDB_TYPE 55555

class PtcM2D_UpdateFriendOnDB : public CProtocol
{
public:
    explicit PtcM2D_UpdateFriendOnDB() : CProtocol(PTCM2D_UPDATEFRIENDONDB_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_UpdateFriendOnDB()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateFriendOnDB m_Data;
};

#endif