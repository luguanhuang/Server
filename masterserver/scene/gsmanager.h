#ifndef _H_Gs_Manager_H__
#define _H_Gs_Manager_H__

#include "commondef.h"

class CGsInfo
{
public:
	CGsInfo()
		:m_dwLineID(0)
		,m_dwSceneNum(0)
		,m_dwRoleNum(0)
	{
	}
	~CGsInfo()
	{
	}

public:
	UINT32	m_dwLineID;
	UINT32	m_dwSceneNum;
	UINT32	m_dwRoleNum;
};


class CGsManager
{
private:
	CGsManager();
	~CGsManager();
	DECLARE_SINGLETON(CGsManager)
public:
	bool Init();
	void Uninit();

	void OnLineConnected(UINT32 dwLine);
	void OnLineClosed(UINT32 dwLine);

	CGsInfo* GetGsInfo(UINT32 dwLine);
	UINT32	 GetGsNum() const { return m_oGsMap.size(); }

	void IncGsLineRoleNum(UINT32 dwLine, UINT32 dwNum);
	void DecGsLineRoleNum(UINT32 dwLine, UINT32 dwNum);

	void	SetGsLinkTimes(UINT32 dwLine, UINT32 dwTimes);
	UINT32	GetGsLinkTimes(UINT32 dwLine) const;
private:
	typedef std::map<UINT32, CGsInfo>	CGsInfoMap;
	typedef std::map<UINT32, UINT32>	CGsLinkRecMap;		//LineID -> LinkRecord

public:
	CGsInfoMap::iterator Begin() { return m_oGsMap.begin();}
	CGsInfoMap::iterator End() { return m_oGsMap.end();}

private:
	CGsInfoMap		m_oGsMap;
	CGsLinkRecMap	m_oGsLinkRecMap;
};

#endif