#ifndef __PTCM2G_PKUPDATESYNNTF_H__
#define __PTCM2G_PKUPDATESYNNTF_H__

// generate by ProtoGen at date: 2017/6/13 11:39:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_PKUPDATESYNNTF_TYPE 35115

class PtcM2G_PkUpdateSynNtf : public CProtocol
{
public:
    explicit PtcM2G_PkUpdateSynNtf() : CProtocol(PTCM2G_PKUPDATESYNNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_PkUpdateSynNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkUpdateSynPara m_Data;
};

#endif