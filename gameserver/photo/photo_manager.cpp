#include "pch.h"
#include "photo_manager.h"

INSTANCE_SINGLETON(PhotoManager);

PhotoManager::PhotoManager()
{

}

PhotoManager::~PhotoManager()
{

}

bool PhotoManager::Init()
{
	if (!photograph_effect_.LoadFile("table/PhotographEffect.txt"))
	{
		SSWarn<<"load file table/PhotographEffect.txt failed"<<END;
		return false;
	}

	return true;
}

void PhotoManager::Uninit()
{

}


