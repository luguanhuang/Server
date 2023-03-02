#ifndef __SCENEUPDATEHANDLER_H__
#define __SCENEUPDATEHANDLER_H__

class Scene;
class Role;
enum UpdateHandlerType
{
	UPDATE_HANDLER_NULL,
	UPDATE_HANDLER_SPAWN,
	UPDATE_HANDLER_VS,
};
class ISceneUpdateHandler
{
public:
	virtual ~ISceneUpdateHandler() {}

	virtual UpdateHandlerType GetHandlerType(){return UPDATE_HANDLER_NULL;}

	virtual UINT32 Update(Scene* scene, float deltatime) = 0;
};

class SpawnUpdateHandler : public ISceneUpdateHandler 
{
public:
	virtual UINT32 Update(Scene* scene, float deltatime);

	virtual UpdateHandlerType GetHandlerType(){return UPDATE_HANDLER_SPAWN;}

	static SpawnUpdateHandler m_updatehandler;
};

class VsUpdateHandler : public ISceneUpdateHandler
{
public:
	virtual UINT32 Update(Scene* scene, float deltatime);

	virtual UpdateHandlerType GetHandlerType(){return UPDATE_HANDLER_VS;}

	static VsUpdateHandler m_updatehandler;
};

#endif