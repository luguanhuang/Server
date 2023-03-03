#ifndef __PTCM2K_SERVERCROSSDATAM2W_H__
#define __PTCM2K_SERVERCROSSDATAM2W_H__

// generate by ProtoGen at date: 2017/7/6 10:58:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_SERVERCROSSDATAM2W_TYPE 7963

class PtcM2K_ServerCrossDataM2W : public CProtocol
{
public:
    explicit PtcM2K_ServerCrossDataM2W() : CProtocol(PTCM2K_SERVERCROSSDATAM2W_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_ServerCrossDataM2W()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerCrossDataSynPara m_Data;
};

#endif