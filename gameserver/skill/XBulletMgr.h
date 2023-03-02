#ifndef __XBULLETMGR_H__
#define __XBULLETMGR_H__

class XBullet;

struct KeyOfBullet
{
	KeyOfBullet(UINT64 h, UINT64 s, UINT64 t)
		:host(h), self(s), target(t) { }

	UINT64 host;
	UINT64 self;
	UINT64 target;
};

struct CmpOfBullet
{
	bool operator()(const KeyOfBullet& l, const KeyOfBullet& r) const
	{
		if(l.host == r.host)
		{
			if(l.self == r.self)
				return l.target < r.target;
			else
				return l.self < r.self;
		}
		else
			return l.host < r.host;
	}
};

class XBulletMgr
{
private:
	XBulletMgr();
	~XBulletMgr();
	DECLARE_SINGLETON(XBulletMgr)
public:

	void ShootBullet(XBullet* bullet, bool client);
	void Update(float fDeltaT);
	bool Init(){ return true; }
	void Uninit();

	void ClearBullets(UINT64 firer);

	XBullet* GetClientBullet(const KeyOfBullet& id)
	{
		auto it = _client_bullets.find(id);
		return it != _client_bullets.end() ? it->second : 0;
	}

private:
	std::map<KeyOfBullet, XBullet*, CmpOfBullet> _server_bullets;
	std::map<KeyOfBullet, XBullet*, CmpOfBullet> _client_bullets;
};

#endif	//__XBULLETMGR_H__