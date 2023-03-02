#ifndef _H_Idip_Err_Desc_H__
#define _H_Idip_Err_Desc_H__

#include "idip.h"


class CIdipErrDesc
{
public:
	CIdipErrDesc();
	~CIdipErrDesc();

	const std::string& GetErrDesc(INT32 nErrCode) const;
private:
	void Bind(INT32 nErrCode, const char* pszErrDesc);
private:
	std::map<INT32, std::string>	m_oDescMap;
};

const std::string& GetIdipErrDesc(EIdipErrCode nErrCode);
UINT32 GetIdipAreaID(UINT32 AreaID);
bool GetPageNo(INT32 size, INT32 pageno, INT32 pagenum, INT32& start, INT32& end);

#endif