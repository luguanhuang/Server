#ifndef __XPHYSICS_H__
#define __XPHYSICS_H__

enum XPhysicsLayer
{
	XLayer_Default = 0,
	XLayer_Enemy = 1 << 0,
	XLayer_Role = 1 << 1,
	XLayer_BigGuy = 1 << 2,
	XLayer_NoneEntity = 1 << 3,
	XLayer_AllEntity = 1 << 4,

	XLayer_Max
};

class XPhysics
{
	XPhysics() { };
	~XPhysics() { };

	DECLARE_SINGLETON(XPhysics)

public:
	inline bool Init() { return true; }
	inline void Uninit() {};

	inline bool IsCollide(XPhysicsLayer layer1, XPhysicsLayer layer2) { return _bPhysics[layer1][layer2]; }
	XPhysicsLayer LayerConvertor(int scriptLayer);

private:
	static const bool _bPhysics[][XLayer_Max];
};

#endif	//__XPHYSICS_H__