#ifndef _Role_Generator_H__
#define _Role_Generator_H__


class CRoleIDGenerator
{
public:
	CRoleIDGenerator();
	~CRoleIDGenerator();

	bool Init();
	void UnInit();

	UINT64 NewRoleID(UINT32 serverID);
private:
	UINT32	m_dwTime;
	UINT16	m_wIndex;
	UINT32	m_wMaxIndex;
};


#endif