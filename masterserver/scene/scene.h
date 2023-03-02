#ifndef _H_Scene_H__
#define _H_Scene_H__

#include "sceneconfig.h"
#include "protocol.h"


class CScene
{
public:
	CScene();
	~CScene();

	void OnCreate();
	void OnDestroy();

public:
	UINT32 GetID() const		{ return m_dwID; }
	UINT32 GetMapID() const		{ return m_dwMap; }
	UINT32 GetLine() const		{ return m_dwLineID; }
	UINT32 GetRoleNum() const	{ return m_dwRoleNum; }

	void SetID(UINT32 dwID)		{ m_dwID = dwID; }
	void SetMapID(UINT32 dwID)	{ m_dwMap = dwID; }
	void SetLine(UINT32 dwLine) { m_dwLineID = dwLine; }
	void SetRoleNum(UINT32 dwNum) { m_dwRoleNum = dwNum; }

	const SceneConf* GetConf()	{ return m_poConf; }
	void SetConf(const SceneConf* poConf) { m_poConf = poConf; }
	UINT32 GetSceneType();

	void BroadCast(CProtocol& roPtc);
private:
	UINT32	m_dwID;
	UINT32	m_dwMap;
	UINT32	m_dwLineID;
	UINT32	m_dwRoleNum;
	UINT32	m_dwObserverNum;

	const SceneConf*	m_poConf;
};

#endif
