#ifndef __PTCI2P_REGISTERLOGINNTF_H__
#define __PTCI2P_REGISTERLOGINNTF_H__

// generate by ProtoGen at date: 2016/7/4 15:02:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2P_REGISTERLOGINNTF_TYPE 27708

class PtcI2P_RegisterLoginNtf : public CProtocol
{
public:
    explicit PtcI2P_RegisterLoginNtf() : CProtocol(PTCI2P_REGISTERLOGINNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2P_RegisterLoginNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegisterLoginNtf m_Data;
};

#endif