/**
@file PUSpawner.h
*/

#ifndef __Logic_PUSpawner_H
#define __Logic_PUSpawner_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{

	namespace PowerUp {
			enum TPUArcheType : unsigned char {
				APPLE		= 0,
				PANDORA		= 1,
				THRONE		= 2,
				_COUNT		= 3
			};		
		}		

	typedef PowerUp::TPUArcheType	TPUArchetype;

	const std::string PUARCHETYPE_STRINGS[] = {
			"PUApple","PUPandora",
			  "PUThrone"	
		};

	class CPUSpawner : public IComponent
	{
		DEC_FACTORY(CPUSpawner);
	public:


		CPUSpawner() : IComponent(GetAltTypeIdOf(CPUSpawner)), _maxTimeToSpawn(0), _timeModulator(0), _timeToSpawn(0), _timeSinceLastSpawn(0) {}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		virtual void process(CMessage *message);

		/************************
			UTILITY FUNCTIONS
		*************************/

	protected:

		int _maxTimeToSpawn;
		int _timeModulator;
		int _timeToSpawn;
		int _timeSinceLastSpawn;

		int _generatePseudoRandomTimeToSpawn();
		CLogicalPosition _generateRandomLogicEmptyPosition();
		void _spawnRandomPU(const CLogicalPosition& pos);

		

	}; // class CAltarStateSwitcher

	REG_FACTORY(CPUSpawner);

} // namespace Logic

#endif // __Logic_CPUSpawner_H */
