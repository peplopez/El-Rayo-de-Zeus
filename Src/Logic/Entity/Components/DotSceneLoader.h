/**
@file DotSceneLoader.h

Contiene la declaración del componente que permite cargar una escena gráfica
desde un fichero de mapa.

@see Logic::CDotSceneLoader
@see Logic::IComponent

@author Emilio Santalla
*/

#ifndef __Logic_DotSceneLoader_H
#define __Logic_DotSceneLoader_H

#include "Logic/Entity/Component.h"


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CScene;
}

namespace Logic
{
	class CMessage;
}
//declaración de la clase
namespace Logic 
{

	class CDotSceneLoader : public IComponent
	{
		DEC_FACTORY(CDotSceneLoader);

	public:

		CDotSceneLoader() : 
			IComponent(GetAltTypeIdOf(CDotSceneLoader)), _scene(0), _scenarioFile("") {}


		virtual ~CDotSceneLoader();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


	protected:


		/**
		*/
		Graphics::CScene* _scene;

		/**.
		*/
		std::string _scenarioFile;


		

	}; // class CGraphics

	REG_FACTORY(CDotSceneLoader);

} // namespace Logic

#endif // __Logic_Graphics_H
