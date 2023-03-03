#ifndef __PTCK2M_PKMATCHINWORLDNTF_H__
#define __PTCK2M_PKMATCHINWORLDNTF_H__

// generate by ProtoGen at date: 2017/2/2 19:46:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_PKMATCHINWORLDNTF_TYPE 29347

class PtcK2M_PkMatchInWorldNtf : public CProtocol
{
public:
    explicit PtcK2M_PkMatchInWorldNtf() : CProtocol(PTCK2M_PKMATCHINWORLDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_PkMatchInWorldNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkMatchToWorldPara m_Data;
};

#endif