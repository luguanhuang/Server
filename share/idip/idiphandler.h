#ifndef _H_Idip_Handler_H__
#define _H_Idip_Handler_H__

#include "idip.h"
#include "rapidjson/document.h"
#include "crpc.h"
#include "utildb/roledatafields.h"
#include "define/itemdef.h"
enum OperType
{
	ReadAndModify = 0,
	ReadOnly = 1,
};

class CIdipReply
{
public:
	CIdipReply(std::string& strResult)
	:m_nErrCode(IDIP_ERR_SUCCESS)
	,m_strResult(strResult)
	{
	}
	~CIdipReply()
	{
	}

public:
	EIdipErrCode	m_nErrCode;			//错误码
	std::string&	m_strResult;		//结果Json串, 在返回body中
};

std::string	PackErrResponse(EIdipErrCode nErrCode);
bool GetItemChangeList(const rapidjson::Value& roMember, std::vector<ItemDesc>& items);
class CIdipHandler
{
public:
	CIdipHandler();
	CIdipHandler(UINT32 dwID, INT32 nCmd);
	virtual ~CIdipHandler();

	virtual CIdipHandler* New() { return NULL; }

	virtual bool Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply) { return true; }
	virtual bool OnAsyncReply(const CUserData& roUserData) { return false; }
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData) { CRpc::ReplyDelayRpc(GetRpcID(), roUserData);return true; }
	virtual bool OnAsyncReplyFromOther(const CUserData& roUserData) { CRpc::ReplyDelayRpc(GetRpcID(), roUserData);return true; }
	
	//以下虚函数MS服务器实现的
	//判断由MS还是GS来处理，默认返回0，则不处理 MS上的handler需要实现该函数，gs上不需要实现
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply) {return IDIP_SERVER_NONE;}
	//向DB请求读取数据
	virtual bool ReadDataQequest() {return false;}

	//MS上修改完数据，然后向DB保存数据
	virtual bool ModifyMsData(CUserData& roUserData) { return false; }
	virtual bool SaveDataToDB(const CUserData& roUserData) {return true; }
	virtual bool SaveDataOnAsyncReply(const CUserData& roUserData) { return false; }
	virtual bool KickOutReply(){return true; }
	virtual bool DoOther(CIdipReply& roReply){return true;}

	UINT32	GetID() const	{ return m_dwID; }
	INT32	GetCmd() const	{ return m_nCmd; }
	UINT32	GetRpcID() const{ return m_dwRpcID; }
	std::string GetStrJson() const {return m_szJson; }
	UINT64	GetRoleID() const { return m_dwRoleID; }
	INT32 GetServerType() const { return m_nServerType; }
	bool IsKickAccount() const { return m_bKickAccount; }
	UINT32 GetServerID() const { return m_ServerID; }

	void	SetID(UINT32 dwID) { m_dwID = dwID; }
	void	SetCmd(INT32 nCmd) { m_nCmd = nCmd; }
	void	SetRpcID(UINT32 dwID) { m_dwRpcID = dwID; }
	void	SetJson(const std::string& strJson){ m_szJson = strJson; }
	void	SetRoleID(UINT64 dwRoleID) {m_dwRoleID = dwRoleID; }
	void	SetServerType(INT32 nType) {m_nServerType = nType; }
	void	SetKickAccount() { m_bKickAccount = true;}
	void	SetServerID(UINT32 dwID) { m_ServerID = dwID;}

private:
	UINT32	m_dwID;
	INT32	m_nCmd;
	UINT32	m_dwRpcID;
	std::string m_szJson;
	UINT64 m_dwRoleID;
	INT32 m_nServerType;
	bool m_bKickAccount;
	UINT32 m_ServerID;
};

#define IDIP_HANDLER_COMMON_DECLARE(handler_name, cmdid) \
class CMSIdip##handler_name##Handler: public CIdipHandler \
{ \
public: \
	CMSIdip##handler_name##Handler(UINT32 dwID, INT32 nCmd) :CIdipHandler(dwID, nCmd) {} \
	CMSIdip##handler_name##Handler() :CIdipHandler(0, cmdid) {} \
	virtual ~CMSIdip##handler_name##Handler() {} \
	virtual CIdipHandler* New() {return new CMSIdip##handler_name##Handler();}\
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply); \
};

#define IDIP_HANDLER_MODIFY_DECLARE(handler_name, cmdid) \
class CMSIdip##handler_name##Handler: public CIdipHandler \
{ \
public: \
	CMSIdip##handler_name##Handler(UINT32 dwID, INT32 nCmd) :CIdipHandler(dwID, nCmd),m_uItemID(0),m_nNum(0) {} \
	CMSIdip##handler_name##Handler() :CIdipHandler(0, cmdid),m_uItemID(0),m_nNum(0) {} \
	virtual ~CMSIdip##handler_name##Handler() {} \
	virtual CIdipHandler* New() {return new CMSIdip##handler_name##Handler();}\
	virtual bool ReadDataQequest();\
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply); \
	virtual bool ModifyMsData(CUserData& roUserData); \
	virtual bool SaveDataToDB(const CUserData& roUserData); \
	virtual bool OnAsyncReply(const CUserData& roUserData); \
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData); \
	virtual bool DoOther(CIdipReply& roReply); \
	virtual bool OnAsyncReplyFromOther(const CUserData& roUserData); \
public:	\
	void IdipLog(); \
private:\
	UINT32 m_uItemID;\
	INT64 m_nNum;\
	std::string szOpenId;\
	std::string szMailTitle;\
	std::string szMailContent;\
};

#define IDIP_HANDLER_READ_DECLARE(handler_name, cmdid) \
class CMSIdip##handler_name##Handler: public CIdipHandler \
{ \
public: \
	CMSIdip##handler_name##Handler(UINT32 dwID, INT32 nCmd) :CIdipHandler(dwID, nCmd) { m_id1 = 0;} \
	CMSIdip##handler_name##Handler() :CIdipHandler(0, cmdid) { m_id1 = 0;} \
	virtual ~CMSIdip##handler_name##Handler() {} \
	virtual CIdipHandler* New() {return new CMSIdip##handler_name##Handler();}\
	virtual bool ReadDataQequest();\
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply); \
	virtual bool OnAsyncReply(const CUserData& roUserData); \
public: \
	std::string szOpenId;\
	INT32 m_id1;\
};

#define IDIP_HANDLER_MODIFY_IMPLEMETION(handler_name) \
	IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(handler_name) \
	IDIP_REPLY_FROMGS_IMPLEMETION(handler_name) \
	IDIP_DO_OTHER(handler_name)

#define IDIP_HANDLER_READANDSAVE_IMPLEMETION(handler_name) \
bool CMSIdip##handler_name##Handler::ReadDataQequest()\
{\
		RpcM2D_ReadRoleData* rpc = RpcM2D_ReadRoleData::CreateRpc();\
		rpc->m_oArg.set_index(GetID());\
		rpc->m_oArg.set_param(ReadAndModify);\
		rpc->m_oArg.set_roleid(GetRoleID());\
		rpc->m_oArg.set_readtype(KKSG::ROLE_DATA_READ_IDIP);\
		DBLink::Instance()->SendTo(*rpc);\
	return true;\
}\
bool CMSIdip##handler_name##Handler::SaveDataToDB(const CUserData& roUserData)\
{\
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;\
	RpcM2D_MsSaveRoleData* rpc = RpcM2D_MsSaveRoleData::CreateRpc();\
	rpc->m_oArg.set_account(pInfo->brief().accountid());\
	rpc->m_oArg.set_roleid(pInfo->brief().roleid());\
	CRoleDataFields oFields(pInfo);\
	if(!CRoleDataSaveUtil::SaveAll(oFields, rpc->m_oArg))\
	{\
		LogError("Pack role data failed");\
		delete rpc;\
		return false;\
	}\
	rpc->m_oArg.set_param(GetID());\
	DBLink::Instance()->SendTo(*rpc);\
	return true;\
}

#define IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(handler_name) \
	IDIP_HANDLER_READANDSAVE_IMPLEMETION(handler_name) \
bool CMSIdip##handler_name##Handler::OnAsyncReply(const CUserData& roUserData)\
{\
	if(roUserData.m_pUserPtr == NULL)\
	{\
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);\
	}\
	else\
	{\
		EIdipErrCode nErrCode = IDIP_ERR_UNKNOWN;\
		if ((KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)\
		{\
			nErrCode = IDIP_ERR_SUCCESS;\
		}\
		std::string str;\
		CIdipReply oReply(str);\
		oReply.m_nErrCode = nErrCode;\
		oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);\
		CUserData data;\
		data.m_pUserPtr = &oReply;\
		CRpc::ReplyDelayRpc(GetRpcID(), data);\
		IdipLog();\
	}\
	return true;\
}

#define IDIP_HANDLER_READ_IMPLEMETION(handler_name) \
bool CMSIdip##handler_name##Handler::ReadDataQequest()\
{\
	RpcM2D_ReadRoleData* rpc = RpcM2D_ReadRoleData::CreateRpc();\
	rpc->m_oArg.set_index(GetID());\
	rpc->m_oArg.set_param(ReadOnly);\
	rpc->m_oArg.set_roleid(GetRoleID());\
	rpc->m_oArg.set_readtype(KKSG::ROLE_DATA_READ_IDIP);\
	DBLink::Instance()->SendTo(*rpc);\
	return true;\
}

//reply from gs
#define IDIP_REPLY_FROMGS_IMPLEMETION(handler_name) \
bool CMSIdip##handler_name##Handler::OnAsyncReplyFromGs(const CUserData& roUserData)\
{\
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);\
	if (roUserData.m_pUserPtr && (KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)\
	{\
		IdipLog();\
	}\
	return true;\
}

#define IDIP_DO_OTHER(handler_name) \
bool CMSIdip##handler_name##Handler::DoOther(CIdipReply& roReply)\
{\
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;\
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);\
	return true; \
}\
bool CMSIdip##handler_name##Handler::OnAsyncReplyFromOther(const CUserData& roUserData)\
{\
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);\
	return true;\
}

//gs上idip的宏定义;
#define GS_IDIP_HANDLER_COMMON_DECLARE(handler_name, cmdid) \
class CIdip##handler_name##Handler: public CIdipHandler \
{ \
public: \
	CIdip##handler_name##Handler(UINT32 dwID, INT32 nCmd) :CIdipHandler(dwID, nCmd) {} \
	CIdip##handler_name##Handler() :CIdipHandler(0, cmdid) {} \
	virtual ~CIdip##handler_name##Handler() {} \
	virtual CIdipHandler* New() {return new CIdip##handler_name##Handler();}\
	virtual bool Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);\
};

#endif
