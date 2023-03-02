#include "pch.h"

#include "XBullet.h"
#include "XBulletCore.h"
#include "XBulletMgr.h"

INSTANCE_SINGLETON(XBulletMgr)

XBulletMgr::XBulletMgr()
{
}

XBulletMgr::~XBulletMgr()
{

}

void XBulletMgr::ShootBullet(XBullet* bullet, bool client)
{
	if(bullet)
	{
		KeyOfBullet key = KeyOfBullet(bullet->GetBulletCore()->GetFirerID(), bullet->ID(), bullet->ExtraID());

		if(client)
		{
			auto it = _client_bullets.find(key);
			if(it == _client_bullets.end())
			{
				// _client_bullets.insert(make_pair<KeyOfBullet, XBullet*>(key, bullet));lgh
				 _client_bullets[key]=bullet;//.insert(make_pair<KeyOfBullet, XBullet*>(key, bullet));
			}
			else
				delete bullet;
		}
		else
		{
			auto it = _server_bullets.find(key);
			if(it == _server_bullets.end())
			{
				// _server_bullets.insert(make_pair<KeyOfBullet, XBullet*>(key, bullet));lgh
				 _client_bullets[key]=bullet;
			}
			else
				delete bullet;
		}
	}
}

void XBulletMgr::Update(float fDeltaT)
{
	auto rit = _server_bullets.begin();

	while(rit != _server_bullets.end())
	{
		if(rit->second->IsExpired())
		{
			rit->second->Destroy();

			delete rit->second;
			_server_bullets.erase(rit++);
		}
		else
		{
			rit->second->Update(fDeltaT);
			++rit;
		}
	}

	auto mapit = _client_bullets.begin();
	while (mapit != _client_bullets.end())
	{
		if(mapit->second->IsExpired())
		{
			//for client bullet not do this...
			//mapit->second->Destroy();

			delete mapit->second;
			_client_bullets.erase(mapit++);
		}
		else
		{
			mapit->second->Update(fDeltaT);
			++mapit;
		}
	}
}

void XBulletMgr::Uninit()
{
	auto it = _server_bullets.begin();

	while (it != _server_bullets.end())
	{
		delete it->second;
		++it;
	}

	_server_bullets.clear();

	auto mapit = _client_bullets.begin();
	while (mapit != _client_bullets.end())
	{
		delete mapit->second;
		++mapit;
	}

	_client_bullets.clear();
}

void XBulletMgr::ClearBullets(UINT64 firer)
{
	auto it = _server_bullets.begin();

	while (it != _server_bullets.end())
	{
		if(it->second->GetBulletCore()->GetFirerID() == firer)
		{
			delete it->second;
			_server_bullets.erase(it++);
		}
		else
			++it;
	}

	auto mapit = _client_bullets.begin();
	while (mapit != _client_bullets.end())
	{
		if(mapit->second->GetBulletCore()->GetFirerID() == firer)
		{
			delete mapit->second;
			mapit->second = NULL;
			_client_bullets.erase(mapit++);
		}
		else
			++mapit;
	}
}
