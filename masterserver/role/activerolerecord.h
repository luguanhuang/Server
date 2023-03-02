#ifndef _H_Active_Role_Record_H__
#define _H_Active_Role_Record_H__


class CActiveRoleRecord
{
	CActiveRoleRecord();
	~CActiveRoleRecord();
	DECLARE_SINGLETON(CActiveRoleRecord)

public:
	bool Init();
	void Uninit();

	void Record(UINT32 dwLastLogoutTime, UINT64 qwRoleID, const std::string& strAccount);
public:
	struct RoleActiveInfo
	{
		UINT64		m_qwRoleID;
		std::string	m_strAccount;
		RoleActiveInfo()
			:m_qwRoleID(0)
		{
		}
	};
	typedef std::multimap<UINT32, RoleActiveInfo, std::greater<UINT32> > CRoleActiveInfoMap;
	typedef std::map<UINT64, RoleActiveInfo*>	CActiveRoleList;

	typedef CRoleActiveInfoMap::iterator iterator;
public:
	iterator Begin() { return m_oInfoMap.begin();}
	iterator End()   { return m_oInfoMap.end();}

	void GetActiveRoleTopN(UINT32 dwTopN, CActiveRoleList& roRoleList);

private:
	CRoleActiveInfoMap	m_oInfoMap;
};

#endif