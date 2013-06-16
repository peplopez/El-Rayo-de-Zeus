//---------------------------------------------------------------------------
// FactoryENET.h
//---------------------------------------------------------------------------

/**
 * @file FactoryENET.h
 *
 * Especifica una factoria de objetos de red implementados con ENet
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#ifndef __Net_FactoryENET_H
#define __Net_FactoryENET_H


#include "Factory.h"
#include "ServerENET.h"
#include "ClientENET.h"

namespace Net {

/**
 * Factoría para obtener el servidor y cliente de red implementados con la librería ENet
 */
class CFactoryENET: public CFactory {

public:

	CFactoryENET() {};

	CServer* buildServer();


	CClient*  buildClient();

};



CServer* CFactoryENET::buildServer()
{
	return new CServerENET();
}

CClient* CFactoryENET::buildClient()
{
	return new CClientENET();
}


} //namespace Net

#endif