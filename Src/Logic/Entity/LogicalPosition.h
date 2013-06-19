//---------------------------------------------------------------------------
// LogicalPosition.h
//---------------------------------------------------------------------------


/**
@file LogicalPosition.h

Contiene la deficion de la clase que maneja las posiciones lógicas

@see Logic::LogicalPosition

@author Jose Luis López Sánchez
*/
#pragma once
#ifndef __Logic_LogicalPosition_H
#define __Logic_LogicalPosition_H

#pragma warning(disable: 4482)

#include <string>

#include <BaseSubsystems/Math.h>

//#include "../Server.h"
/*namespace Logic
{	
	class CServer;
}*/
namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace LogicalPosition
	{					
		/**
			Ambito actual hasta que se cambie de base y/o anillo
		*/
		enum Ring : unsigned short
		{	
			LOWER_RING		=0,
			CENTRAL_RING	=1,
			UPPER_RING		=2
		};
		enum Sense : unsigned short
		{ //Quizá mejor sentido angunos positivos/negativos?	
			UNDEFINED=			0,
			RIGHT=				1,
			LEFT=				2,
			LOOKING_CENTER=		3,
			LOOKING_OUTSIDE=	4
		};
		

	}

	//Coordenadas comunes a todas las entidades del juego.
	
	/**
	Tipo copia para los anillos. Por simplicidad.
	*/
	typedef LogicalPosition::Ring Ring;
	typedef LogicalPosition::Sense Sense;

	
	/**
	Contiene el tipo de datos de una posición lógica de representación espacial dentro de nuestro juego
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author José Luis López Sánchez
	@date Diciembre, 2012
    @ingroup grupoEntidad
	*/
	class CLogicalPosition
	{					
	public:
		
		CLogicalPosition();
		~CLogicalPosition();
		
		/**
		Establece la posición de la entidad en grados.

		@param position Nueva posición.
		*/
		void setDegree(const float degree);

		/**
		Devuelve la posición de la entidad.
		en grados
		@return Posición de la entidad en el entorno.
		*/
		float getDegree() const ;
		

		/**
		Establece la altura de la entidad con respeto al plano XZ (ANILLO) sobre el que se encuentra

		@param height, nueva altura
		*/
		void setHeight(const float height);

		/**
		Devuelve la altura de la entidad.
		
		@return Height de la entidad con respecto al anillo actual.
		*/
		float getHeight() const ;

		/**
		Establece la base de la entidad. 
		NO Avisa a los componentes
		del cambio. Mas adelante vere si es necesario pero creo que no

		@param base nueva
		*/
		void setBase(const unsigned short base);

		/**
		Devuelve la base de la entidad.
		
		@return Base de la entidad en el entorno.
		*/
		unsigned short getBase() const;

		/**
		Establece el anillo de la entidad. 
		NO Avisa a los componentes
		del cambio. Mas adelante vere si es necesario pero creo que no

		@param Ring nueva
		*/
		void setRing(const LogicalPosition::Ring ring);

		/**
		Devuelve el anillo de la entidad.
		
		@return Ring de la entidad en el entorno.
		*/
		LogicalPosition::Ring getRing() const ;

		/**
		Devuelve el radio sobre el que se mueve la entidad.
		NO USAREMOS SET_RADIO, con los cambios de anillo usamos setRing()
		@return Radio de la entidad en el entorno.
		*/
		float getRadio();
		
		/**
		Establece la anchura de la entidad gráfica
		@param angularBox nuevo
		*/
		void setSense(const Logic::Sense sense) ;

		/**
		Devuelve la anchura de la entidad gráfica.
		
		@return AngularBox de la entidad en el entorno.
		*/
		Logic::Sense getSense() const ;

	private:
		Ring _ring; // Anillo
		Sense _sense; // Sentido
		unsigned short _base; // Número de base: 1-8			
		float _degrees; // Grados	
		float _height; // Altura

	}; 

} // namespace Logic

#endif // __Logic_LogicalPosition_H