//David Llanso Tutoria
/*

//Constructor
CBillboard(name){

	_bbset = CServer::getSingletonPtr()->getActiveScene()->createBillboard(name);
}

// Destructor 

*/
//metodos de setDimensions....activate, deactivate, setcoord...
//Dentro se utilizaria. _bbset->getBillboard(0) para acceder al primer billboard del conjunto de billboards.

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
	//Constructor de la clase CBillboard. Donde se crea un BillboardSet (de momento con 1 sólo Billboard)
	CBillboard::CBillboard(const std::string &name)
	{
		//Graphics::CServer::getSingletonPtr()->getActiveScene()->createBillboard();
		//Desde el singletonPtr del Server grafico, puedo llamar al metodo getActiveScene para
		//conseguir la escena actual. Y desde la escena puedo llamar al método createBillboard
		//para crear el BillBoardSet desde la escena
		_bbSet = Graphics::CServer::getSingletonPtr()->getActiveScene()->addBillboard(name);
		//_namebb = name;

	} // CBillboard

	//--------------------------------------------------------

	CBillboard::~CBillboard() 
	{
		// desacoplamos el billboard de su entidad
		/*_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);
		_scene->getSceneMgr()->destroySceneNode(_targetNode);
		*/

	} // ~CBillboard

	//Las coordenadas se le dan al Billboard.
	void CBillboard::setCoordenadas(const float u0,const float v0,const float u1,const float v1){
		_bbSet->getBillboard(0)->setTexcoordRect(u0,v0,u1,v1);
	}

	//Las dimensiones se le dan al Billboard
	void CBillboard::setDimensions(const float width,const float height){
		//se usa la función de Ogre setDimensions sobre el billboard para darle unas dimensiones
		_bbSet->getBillboard(0)->setDimensions(width,height);

	}

	//aqui debería de colocarse encima de la entidad.
	void CBillboard::setPosition(float,float,float){ //x,y,z
		
	}

	//El material se realiza sobre el BillboardSet
	void CBillboard::setMaterial(const std::string &name){
		_bbSet->setMaterialName(name); //se utiliza ogre para darle un material al billboardSet
	}

	//--------------------------------------------------------


} // namespace Graphics

