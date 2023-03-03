#ifndef __PTCM2G_ADDGUILDBOSSATTRGS_H__
#define __PTCM2G_ADDGUILDBOSSATTRGS_H__

// generate by ProtoGen at date: 2016/8/3 22:31:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_ADDGUILDBOSSATTRGS_TYPE 60439

class PtcM2G_AddGuildBossAttrGs : public CProtocol
{
public:
    explicit PtcM2G_AddGuildBossAttrGs() : CProtocol(PTCM2G_ADDGUILDBOSSATTRGS_TYPE)
    {
		
    }

    virtual ~PtcM2G_AddGuildBossAttrGs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif