#ifndef _H_Idip_Proc_H__
#define _H_Idip_Proc_H__

#include "idip/idip.h"
#include "idip/idiphandler.h"


class CIdipCmdProcess
{
 	CIdipCmdProcess();
 	~CIdipCmdProcess();
 	DECLARE_SINGLETON(CIdipCmdProcess)
 
 public:
 	bool Init();
 	void Uninit();

	CIdipHandler* Process(const std::string& strJson, CIdipReply& roReply);

	void AddHandler(CIdipHandler* poHandler);
	void OnAsyncReply(UINT32 dwID, const CUserData& roUserData);
private:
	CIdipHandler* Create(INT32 nCmd);
	void Release(CIdipHandler* poHandler);
	void Register(CIdipHandler* poHandler);

private:
	typedef std::list<CIdipHandler*>				CHandlerList;
	typedef std::pair<CIdipHandler*, CHandlerList>	CHandlerInfo;
	typedef std::map<INT32, CHandlerInfo>			CHandlerInfoMap;
	typedef std::map<INT32, CIdipHandler*>			CHandlerMap;

	UINT32			m_dwNextID;
	CHandlerInfoMap	m_oInfoMap;
	CHandlerMap		m_oHandlerMap;
};

#endif