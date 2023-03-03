#ifndef __PTCF2M_FMLARGEROOMCLOSEPARMNTF_H__
#define __PTCF2M_FMLARGEROOMCLOSEPARMNTF_H__

// generate by ProtoGen at date: 2016/11/30 20:40:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCF2M_FMLARGEROOMCLOSEPARMNTF_TYPE 45955

class PtcF2M_FMLargeRoomCloseParmNtf : public CProtocol
{
public:
    explicit PtcF2M_FMLargeRoomCloseParmNtf() : CProtocol(PTCF2M_FMLARGEROOMCLOSEPARMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcF2M_FMLargeRoomCloseParmNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMLargeRoomCloseParm m_Data;
};

#endif