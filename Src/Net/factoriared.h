//---------------------------------------------------------------------------
// factoriared.h
//---------------------------------------------------------------------------

/**
 * @file factoriared.h
 *
 * Especifica una factoria (abstracta) de objetos de red
 *
 *
 * @author Juan A. Recio-García
 * @date Octumbre, 2006
 */

#ifndef __FACTORIARED_H
#define __FACTORIARED_H

#include "servidor.h"
#include "cliente.h"

namespace Net {

/**
 * Factoría para obtener el servidor y cliente de red
 */
class CFactoriaRed {

public:
	/**
	 * Obtiene un servidor
	 */
	virtual CServidor* buildServidor()=0;

	/**
	 * Obtiene un cliente
	 */
	virtual CCliente*  buildCliente()=0;
};


} //namespace Net

#endif