#include "pch.h"
#include "XPhysics.h"

INSTANCE_SINGLETON(XPhysics)

/*
 * player --> entity TRUE
 * entity --> player FALSE
 */
const bool XPhysics::_bPhysics[][XLayer_Max] = 
{
	//default  enemy  role    bigguy   none entity   all entity
	/***************************************************************/
	{  true,  true,   true,   true,    true,         true},  // XLayer_Default
	{  true,  false,  false,  true,    false,        true},  // XLayer_Enemy
	{  true,  false,  false,  true,    false,        true},  // XLayer_Role
	{  true,  true,   true,   true,    false,        true},  // XLayer_BigGuy
	{  true,  false,  false,  false,   false,        true},  // XLayer_NoneEntity
	{  true,  true,   true,   true,    true,         true},  // XLayer_AllEntity
};

XPhysicsLayer XPhysics::LayerConvertor(int scriptLayer)
{
	switch(scriptLayer)
	{
		case 10: return XLayer_Enemy;
		case 11: return XLayer_Role;
		case 13: return XLayer_BigGuy;
		case 14: return XLayer_NoneEntity;
		case 18: return XLayer_AllEntity;
		default: return XLayer_Default;
	}
}