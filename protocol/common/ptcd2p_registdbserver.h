#ifndef __PTCD2P_REGISTDBSERVER_H__
#define __PTCD2P_REGISTDBSERVER_H__

// generate by ProtoGen at date: 2016/7/4 15:01:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCD2P_REGISTDBSERVER_TYPE 45216

class PtcD2P_RegistDBServer : public CProtocol
{
public:
    explicit PtcD2P_RegistDBServer() : CProtocol(PTCD2P_REGISTDBSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcD2P_RegistDBServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif