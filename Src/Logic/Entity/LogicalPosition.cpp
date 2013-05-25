//---------------------------------------------------------------------------
// LogicalPosition.cpp
//---------------------------------------------------------------------------


/**
@file LogicalPosition.cpp

Contiene la implementación de la clase que maneja las posiciones lógicas

@see Logic::LogicalPosition

@author Jose Luis López Sánchez
*/
#pragma warning (disable : 4244 ) //Hay una perdida de precisión que compenso con _degrees+=decimal;. Asi que el warning está asumido
#include "Logic\Entity\LogicalPosition.h"

#include "../Server.h"

namespace Logic 
{
	CLogicalPosition::CLogicalPosition() :
			_ring(LogicalPosition::CENTRAL_RING), 
			_sense(LogicalPosition::LEFT), 
			_base(0), _degrees(0),_height(0)
			{}
	
	//---------------------------------------------------------

	CLogicalPosition::~CLogicalPosition()
	{
	} // ~CEntity
	/**
		Establece la posición de la entidad en grados.

		@param position Nueva posición.
		*/
		void CLogicalPosition::setDegree(const float degree)
		{
			if (degree>=0 && degree<360)
				_degrees=degree;
			else
			{
				float decimal=degree-(int)degree;
				int grados=(int)degree%360;

				if (_degrees>360)
				{				
					_degrees=grados;
				}
				else //menor que cero
				{
					_degrees=360-(int)abs(grados);
				}
				_degrees+=decimal;
			}
			//_degrees=degree;
		}

		/**
		Devuelve la posición de la entidad.
		en grados
		@return Posición de la entidad en el entorno.
		*/
		float CLogicalPosition::getDegree() const { 
			return _degrees;
		}

		/**
		Establece la altura de la entidad con respeto al plano XZ (ANILLO) sobre el que se encuentra

		@param height, nueva altura
		*/
		void CLogicalPosition::setHeight(const float height)
		{
				_height = height;
		}

		/**
		Devuelve la altura de la entidad.
		
		@return Height de la entidad con respecto al anillo actual.
		*/
		float CLogicalPosition::getHeight() const { 
		    return	_height;
		}

		/**
		Establece la base de la entidad. 
		NO Avisa a los componentes
		del cambio. Mas adelante vere si es necesario pero creo que no

		@param base nueva
		*/
		void CLogicalPosition::setBase(const unsigned short base)  {_base=base;}

		/**
		Devuelve la base de la entidad.
		
		@return Base de la entidad en el entorno.
		*/
		unsigned short CLogicalPosition::getBase() const { return _base; }

		/**
		Establece el anillo de la entidad. 
		NO Avisa a los componentes
		del cambio. Mas adelante vere si es necesario pero creo que no

		@param Ring nueva
		*/
		void CLogicalPosition::setRing(const LogicalPosition::Ring ring)
		{	
			_ring=ring;
		}

		/**
		Devuelve el anillo de la entidad.
		
		@return Ring de la entidad en el entorno.
		*/
		LogicalPosition::Ring CLogicalPosition::getRing() const { return _ring; }

		/**
		Devuelve el radio sobre el que se mueve la entidad.
		NO USAREMOS SET_RADIO, con los cambios de anillo usamos setRing()
		@return Radio de la entidad en el entorno.
		*/
		float CLogicalPosition::getRadio()
		{
			return CServer::getSingletonPtr()->getRingRadio(_ring);
		}
		
		/**
		Establece la anchura de la entidad gráfica
		@param angularBox nuevo
		*/
		void CLogicalPosition::setSense(const Logic::Sense sense) {_sense = sense; }

		/**
		Devuelve la anchura de la entidad gráfica.
		
		@return AngularBox de la entidad en el entorno.
		*/
		Logic::Sense CLogicalPosition::getSense() const { return _sense; }
		
		
}