#ifndef __PTCK2G_BIGMELEEENDNTF_H__
#define __PTCK2G_BIGMELEEENDNTF_H__

// generate by ProtoGen at date: 2017/8/26 11:20:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2G_BIGMELEEENDNTF_TYPE 36899

class PtcK2G_BigMeleeEndNtf : public CProtocol
{
public:
    explicit PtcK2G_BigMeleeEndNtf() : CProtocol(PTCK2G_BIGMELEEENDNTF_TYPE)
    {

    }

    virtual ~PtcK2G_BigMeleeEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:

};

#endif