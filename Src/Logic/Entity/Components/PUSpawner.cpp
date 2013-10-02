/**
@file PUSpawner.cpp
*/

#include "PUSpawner.h"

#include "Logic/Entity/Entity.h"
#include "Map/Entity.h"
#include "Logic/Maps/Map.h"

#include "../../Maps/EntityFactory.h"



#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "" << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CPUSpawner);
	
	//---------------------------------------------------------

	bool CPUSpawner::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;	

		if(entityInfo->hasAttribute("timeModulator"))
			_timeModulator = entityInfo->getIntAttribute("timeModulator");
	
		if(entityInfo->hasAttribute("maxTimeToSpawn"))
			_maxTimeToSpawn = entityInfo->getIntAttribute("maxTimeToSpawn");


		_timeToSpawn = _generatePseudoRandomTimeToSpawn();

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CPUSpawner::activate()
	{
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CPUSpawner::deactivate()
	{
	} // deactivate
	
	//---------------------------------------------------------

	bool CPUSpawner::accept(const CMessage *message)
	{
		return  false;
	} // accept
	
	//---------------------------------------------------------

	void CPUSpawner::process(CMessage *message)
	{

	} // process
	
	//---------------------------------------------------------

	void CPUSpawner::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		_timeSinceLastSpawn += msecs;

		if (_timeSinceLastSpawn > _timeToSpawn)
		{
			Logic::CLogicalPosition pos = _generateRandomLogicEmptyPosition();
			_spawnRandomPU(pos);
			_timeToSpawn = _generatePseudoRandomTimeToSpawn();
			_timeSinceLastSpawn = 0;
		}


		
	} // tick

	//---------------------------------------------------------

	int CPUSpawner::_generatePseudoRandomTimeToSpawn()
	{
		return ((rand() % (_maxTimeToSpawn * 1000)) + _timeModulator * 1000);
	}

	//---------------------------------------------------------

	CLogicalPosition CPUSpawner::_generateRandomLogicEmptyPosition()
	{
		CLogicalPosition pos;
		pos.setBase(_entity->getOriginBase());
		pos.setDegree(rand() % 360);
		pos.setHeight(3);
		pos.setRing(static_cast<Logic::Ring>(1 + rand() % 3));
		return pos;
	}

	//---------------------------------------------------------

	void CPUSpawner::_spawnRandomPU(const CLogicalPosition& pos)
	{
		Map::CEntity randomPUInfo;
		randomPUInfo.setType(PUARCHETYPE_STRINGS[rand() % TPUArchetype::_COUNT]);
		
		std::ostringstream eBase, eRing, eDegrees, eHeight;
		eBase << pos.getBase();
		eRing << (unsigned short) pos.getRing();
		eDegrees << (float)pos.getDegree();
		eHeight << (float)pos.getHeight();

		randomPUInfo.setAttribute("base", eBase.str());
		randomPUInfo.setAttribute("ring", eRing.str());
		randomPUInfo.setAttribute("degrees", eDegrees.str());
		randomPUInfo.setAttribute("height", eHeight.str());

		Logic::CEntity* randomPU = Logic::CEntityFactory::getSingletonPtr()->createEntity(randomPUInfo, _entity->getMap(), true);
		randomPU->activate();
	}

} // namespace Logic