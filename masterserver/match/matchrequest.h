#ifndef __MATCHREQUEST_H__
#define __MATCHREQUEST_H__

class CRole;

class MatchRequest
{
public:
	MatchRequest(CRole* pRole);

	void HandleC2MArg(UINT32 delayID, const KKSG::KMatchCommonArg& roArg);
	static void HandleM2GRes(const KKSG::KMatchCommonArgM2G& roArg, const KKSG::KMatchCommonResM2G& roRes);

	int HandleReq(const KKSG::KMatchCommonArg& roArg, const KKSG::KMatchCommonResM2G& roRes);	
	bool CheckReportLimit(const KKSG::KMatchCommonArg& roArg, KKSG::KMatchCommonRes& roRes);	

private:
	int CheckMatchCond(const KKSG::KMatchCommonArg& roArg, KKSG::KMatchCommonResM2G& tmpRes);
	CRole* m_pRole;
};

#endif