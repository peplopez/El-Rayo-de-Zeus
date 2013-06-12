//---------------------------------------------------------------------------
// ParticleSystem.cpp
//---------------------------------------------------------------------------

/**
@file ParticleSystem.cpp

Contiene la implementación de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author FRS
@date Mayo, 2013
*/

#include "ParticleSystem.h"


#include <OgreCommon.h>
#include <OgreMovableObject.h>
#include <OgreSceneManager.h>

#include "Camera.h"
#include "Scene.h"

#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::PARTICLE_SYSTEM>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Graphics 
{		
	
	void CParticleSystem::start() const
	{		
		if(_movObj->IsFXActive() )
			_movObj->RunFX();	
	}

	void CParticleSystem::stop() const
	{
		_movObj->StopFX();
		// TODO comprobar que se llama al stopped para liberar MO
	}
		


	/**********************
		HHFX LISTENER
	***********************/

	// TODO revisar
	void CParticleSystem::OnFXStarted(IHHFX* obj)
	{
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");

		// create a light under the ElectricOrb effect
		// UNDONE FRS
		// if ( strstr(obj->GetPath(), "ElectricOrb.hfx") )
		//{
		//	IHHFXOgre* fx = static_cast<IHHFXOgre*>(obj);
		//	const Vector3& fxPosition = fx->getParentSceneNode()->getPosition();

		//	Ogre::Light* pointLight = getSceneMgr()->createLight("pointLight" + Ogre::StringConverter::toString((unsigned int)(obj)));
		//	pointLight->setType(Ogre::Light::LT_POINT);
		//	pointLight->setPosition(fxPosition + Vector3::UNIT_Y * 0.8f);
		//	pointLight->setDiffuseColour(0.1f, 0.1f, 1.0f);
		//	pointLight->setSpecularColour(0.8f, 0.8f, 1.0f);
		//}
	}

	// called when an effect stopped by itself or when the hhfx scene is cleared
	void CParticleSystem::OnFXStopped(IHHFX* obj)
	{		
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");
		
		_node->detachObject(_movObj);
		getSceneMgr()->destroyMovableObject(_movObj); 
		_movObj = 0;

		// destroy the light created under ElectricOrb
		// UNDONE FRS
		//if (strstr(obj->GetPath(), "ElectricOrb.hfx") != NULL)
		//	getSceneMgr()->destroyLight("pointLight" + Ogre::StringConverter::toString((unsigned int)(obj)));

	}


	/**********************
		SCENE ELEMENT
	***********************/

	bool CParticleSystem::load()
	{
		assert( getSceneMgr()->hasSceneNode( _parentName + "_node") );

		try{		
			_hhfxScene = _scene->getHHFXScene();

			// effect's params
			Ogre::NameValuePairList params;
				params["pack"] =  _hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetPack();
				params["fx"] = _hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetEffects()[ enumSelected ];
				params["run"] = "yes";

			// spawn a new effect at this location
			Ogre::MovableObject	*mo = getSceneMgr()->createMovableObject("HHFX", &params);
			assert(mo && "Error al crear ParticleSystem");	

			_movObj = static_cast<IHHFXOgre*>(mo);
				_movObj->SetFXListener(this);
			// set this class to listen to the ps, to be notified when it is destroyed, 
			// so we may remove the scene node we created, if needed.

		    // create a node to attach the effect
			_node = getSceneMgr()->getSceneNode( _parentName + "_node")
								 ->createChildSceneNode(_relativePos); // TODO FRS añadir algún tipo de orientación inicial?
				_node->attachObject(_movObj);

			_loaded = true;
			
		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;		
	} // load
	
	//--------------------------------------------------------
		
	void CParticleSystem::unload()
	{	
		CSceneElement::unload();	

		if(_movObj)	{		
			getSceneMgr()->destroyMovableObject(_movObj); 
			_movObj = 0;
		}
	} // unload

	
	
	/**********************
		GET's & SET's
	***********************/

	bool CParticleSystem::isVisible() const {
		assert(_loaded && "Imprescindible haber cargado el sistema de particulas en escena primero");
		return _movObj->isVisible();
	}

	
} // namespace Graphics

