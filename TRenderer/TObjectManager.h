#ifndef TObjectManager_H
#define TObjectManager_H

#include "T.h"
#include "TPointLight.h"
#include <string>
#include <unordered_map>

class TObjectManager
{
public:
	virtual ~TObjectManager() {};

	virtual TPointLight* AddPointLight(TVector3 position, TVector3 color, float range, float intencity, std::string key) = 0;
	virtual TPointLight* GetPointLight(std::string key) = 0;
	virtual bool RemovePointLight(std::string key) = 0;
	virtual void FlushPointLights() = 0;

};

#endif
