#ifndef __PTCD2N_MYSQLCONNECTLOST_H__
#define __PTCD2N_MYSQLCONNECTLOST_H__

// generate by ProtoGen at date: 2017/3/10 0:01:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCD2N_MYSQLCONNECTLOST_TYPE 36241

class PtcD2N_MysqlConnectLost : public CProtocol
{
public:
    explicit PtcD2N_MysqlConnectLost() : CProtocol(PTCD2N_MYSQLCONNECTLOST_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcD2N_MysqlConnectLost()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MysqlConnectLostData m_Data;
};

#endif