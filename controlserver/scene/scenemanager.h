#ifndef _H_Scene_Manager_H__
#define _H_Scene_Manager_H__

#include "scene.h"

class CRole;
class CSceneManager
{
private:
	CSceneManager();
	~CSceneManager();
	DECLARE_SINGLETON(CSceneManager)
public:
	bool Init();
	void Uninit();

	CScene* CreateScene(UINT32 dwTemplateID, UINT32 dwInstanceID, UINT32 dwLine);
	void DestroyScene(UINT32 dwSceneID);


	CScene* GetScene(UINT32 dwSceneID);
	void	GetSceneByType(UINT32 type, std::vector<CScene*>& scenes);
	UINT32	GetSceneNum() const { return m_oSceneMap.size(); }
	void	GetScene(UINT32 dwMapID, std::vector<CScene*>& scenes);


	UINT32 FindOwnerScene(UINT32 type, UINT64 uid);
	bool EnterOwnerScene(UINT32 type, UINT64 uid, UINT64 roleid);

	UINT32 NewID();

	void DestroySceneOnGsLine(UINT32 dwLine);
	static bool IsDynamicScene(UINT32 dwTemplateID);

	void RecoverFromShm();
private:
	typedef std::map<UINT32, CScene*>	CSceneMap;

public:
	CSceneMap::iterator Begin() { return m_oSceneMap.begin();}
	CSceneMap::iterator End() { return m_oSceneMap.end();}

private:
	UINT32		m_dwSceneID;
	CSceneMap	m_oSceneMap;
};

#endif
