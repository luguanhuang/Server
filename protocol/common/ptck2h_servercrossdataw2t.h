#ifndef __PTCK2H_SERVERCROSSDATAW2T_H__
#define __PTCK2H_SERVERCROSSDATAW2T_H__

// generate by ProtoGen at date: 2017/7/8 15:33:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2H_SERVERCROSSDATAW2T_TYPE 27970

class PtcK2H_serverCrossDataW2T : public CProtocol
{
public:
    explicit PtcK2H_serverCrossDataW2T() : CProtocol(PTCK2H_SERVERCROSSDATAW2T_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2H_serverCrossDataW2T()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerCrossDataSynPara m_Data;
};

#endif