//---------------------------------------------------------------------------
// Billboard.cpp
//---------------------------------------------------------------------------

/**
@file Billboard.cpp

Contiene la implementación de la clase que maneja el billboard.

@see Graphics::CBillboard

@author Pablo Terrado
@date Enero, 2013
*/

#include "Billboard.h"
#include "Scene.h"
#include "Server.h"

//#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

namespace Graphics 
{
	// TODO pasar esto al spawn y coger map scene
	//Constructor de la clase CBillboard. Donde se crea un BillboardSet (de momento con 1 sólo Billboard)
	CBillboard::CBillboard(const std::string &name, const float offset)  //le paso un string y el offset de la pos del billboard
	{
		//le paso un string
		//_bbSet = Graphics::CServer::getSingletonPtr()->getActiveScene()->createBillboard(name); 
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
			_bbSet = _scen->createBillboard(name,offset);  
		//Desde el singletonPtr del Server grafico, puedo llamar al metodo getActiveScene para
		//conseguir la escena actual. Y desde la escena puedo llamar al método createBillboard
		//para crear el BillBoardSet desde la escena le paso un string y la position
			_bbSet->setMaterialName("lifeBar"); // material por defecto
	} // CBillboard

	//--------------------------------------------------------

	void CBillboard::deactivateBillboard(const std::string &name) 
	{
		// desacoplamos el billboard de su entidad
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		_scen->deleteBillboard(name);

	} // deactivateBillboard

	//Las coordenadas se le dan al Billboard.
	void CBillboard::setCoordenadas(const float u0,const float v0,const float u1,const float v1){
		_bbSet->getBillboard(0)->setTexcoordRect(u0,v0,u1,v1);
	}

	//Las dimensiones se le dan al Billboard
	void CBillboard::setDimensions(const float width,const float height){
		//se usa la función de Ogre setDimensions sobre el billboard para darle unas dimensiones
		_bbSet->getBillboard(0)->setDimensions(width,height);

	}

	//El material se realiza sobre el BillboardSet
	void CBillboard::setMaterial(const std::string &name){
		_bbSet->setMaterialName(name); //se utiliza ogre para darle un material al billboardSet
	}

	//--------------------------------------------------------


} // namespace Graphics

