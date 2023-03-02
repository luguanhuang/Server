#ifndef _H_Idip_MSUpdateExp_Handler_H__
#define _H_Idip_MSUpdateExp_Handler_H__

#include "idip/idiphandler.h"


class CMSIdipUpdateExpHandler: public CIdipHandler
{
public:
	CMSIdipUpdateExpHandler() :CIdipHandler(0, IDIP_DO_UPDATE_EXP_REQ),m_nNum(0) {}
	virtual ~CMSIdipUpdateExpHandler() {}

	virtual CIdipHandler* New() { return new CMSIdipUpdateExpHandler(); }
	virtual bool ReadDataQequest();
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool ModifyMsData(CUserData& roUserData);
	virtual bool SaveDataToDB(const CUserData& roUserData);
	virtual bool OnAsyncReply(const CUserData& roUserData);
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData);

public:
	void IdipLog();
public:
	INT64 m_nNum;
	std::string szOpenId;
	std::string szMailTitle;
	std::string szMailContent;
};

#endif