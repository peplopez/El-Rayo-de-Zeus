//---------------------------------------------------------------------------
// FactoryENET.h
//---------------------------------------------------------------------------

/**
 * @file FactoryENET.h
 *
 * Especifica una factoria (abstracta) de objetos de red
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#ifndef __Net_Factory_H
#define __Net_Factory_H

namespace Net {

	class CServer;
	class CClient;

/**
 * Factoría para obtener el servidor y cliente de red
 */
class CFactory {

public:
	/**
	 * Obtiene un servidor
	 */
	virtual CServer* buildServer()=0;

	/**
	 * Obtiene un cliente
	 */
	virtual CClient*  buildClient()=0;
};


} //namespace Net

#endif