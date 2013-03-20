/**
@file Light.h

Contiene la declaración del componente que controla la representación
de una luz.

@see Logic::CLight
@see Logic::IComponent

@author Emilio Santalla Comellas
@date Marzo, 2013
*/
#ifndef __Logic_Light_H
#define __Logic_Light_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CLight;
	class CScene;
};

namespace Logic
{
	class CMessage;
};

//declaración de la clase
namespace Logic 
{

	class CLight : public IComponent
	{
		DEC_FACTORY(CLight);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CLight() : IComponent(GetAltTypeIdOf(CLight)), _graphicalLight(0) {}

		CLight(altTypeId id) : IComponent(id), _graphicalLight(0) {}


		virtual ~CLight();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const CMessage *message);

		virtual void process(CMessage *message);

	protected:

		/**	Luz gráfica.*/
		Graphics::CLight* _graphicalLight;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		Graphics::CScene* _scene;

		/**
		*/
		std::string _type;

	    /**
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		*/
		virtual Graphics::CLight* createGraphicalLight(const Map::CEntity *entityInfo);
		

	}; // class CLight

	REG_FACTORY(CLight);

} // namespace Logic

#endif // __Logic_Light_H