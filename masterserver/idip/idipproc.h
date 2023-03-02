#ifndef _H_Idip_Proc_H__
#define _H_Idip_Proc_H__

#include "pch.h"
#include "idip/idip.h"
#include "idip/idiphandler.h"
#include "pb/project.pb.h"


class CIdipProcess
{
	CIdipProcess();
	~CIdipProcess();
	DECLARE_SINGLETON(CIdipProcess)

public:
	bool Init();
	void Uninit();

	CIdipHandler* Process(const std::string& strJson, CIdipReply& roReply, const KKSG::GMHttpArg &roArg);

	void AddHandler(CIdipHandler* poHandler);
	bool ModifyData(UINT32 dwID, CUserData& roUserData);
	void OnAsyncReply(UINT32 dwID, const CUserData& roUserData);
	CIdipHandler* Create(INT32 nCmd);
	void Release(CIdipHandler* poHandler);
	bool KickoutHandler(UINT32 dwID);
private:
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
