#ifndef __PTCK2G_SERVERCROSSDATAW2CROSSG_H__
#define __PTCK2G_SERVERCROSSDATAW2CROSSG_H__

// generate by ProtoGen at date: 2017/7/6 11:27:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2G_SERVERCROSSDATAW2CROSSG_TYPE 29551

class PtcK2G_ServerCrossDataW2CrossG : public CProtocol
{
public:
    explicit PtcK2G_ServerCrossDataW2CrossG() : CProtocol(PTCK2G_SERVERCROSSDATAW2CROSSG_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2G_ServerCrossDataW2CrossG()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerCrossDataSynPara m_Data;
};

#endif