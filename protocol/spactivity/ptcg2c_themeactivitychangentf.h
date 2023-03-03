#ifndef __PTCG2C_THEMEACTIVITYCHANGENTF_H__
#define __PTCG2C_THEMEACTIVITYCHANGENTF_H__

// generate by ProtoGen at date: 2017/8/1 18:15:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_THEMEACTIVITYCHANGENTF_TYPE 25642

class PtcG2C_ThemeActivityChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_ThemeActivityChangeNtf() : CProtocol(PTCG2C_THEMEACTIVITYCHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ThemeActivityChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ThemeActivityChangeData m_Data;
};

#endif