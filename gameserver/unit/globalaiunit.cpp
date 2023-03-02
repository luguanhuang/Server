#include "pch.h"
#include "unit/globalaiunit.h"
#include "entity/XGlobalAIObject.h"
#include "scene/scene.h"

GlobalAIUnit::GlobalAIUnit()
{
	m_pGlobalAIObject = NULL;
	m_uID = 0;
}

GlobalAIUnit::~GlobalAIUnit()
{

}

void GlobalAIUnit::Init(Scene* scene)
{
	m_pGlobalAIObject = new XGlobalAIObject(this);
	m_currScene = scene;
	m_pObject = m_pGlobalAIObject;
	m_pGlobalAIObject->Initilize();
	m_pGlobalAIObject->OnCreated();
}

void GlobalAIUnit::Uninit()
{
	XAIDisableEventArgs oArgs;
	GetXObject()->DispatchAction(&oArgs);	

	m_pGlobalAIObject->Uninitilize();
	delete m_pGlobalAIObject;
	m_pGlobalAIObject = NULL;
}

std::string GlobalAIUnit::GetAIString()
{
	return m_currScene->GetSceneInfo()->m_pConf->SceneAI;
}