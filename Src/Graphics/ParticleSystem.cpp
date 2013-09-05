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
#include "Server.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::PARTICLE_SYSTEM>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Graphics 
{	
	/*******************
		CONSTRUCTORES
	*******************/

	CParticleSystem::CParticleSystem(
		const std::string& hfx, const std::string& parentName, const Vector3& relativePos, 
		bool isLooped, const std::string& compoName, int compoMsec,
		const Vector3& lightColorDiff, const Vector3& lightColorSpec) :
			_hfx(hfx), _parentName(parentName), _relativePos(relativePos), 
			_fxLight(0), _lightColorDiff(lightColorDiff), _lightColorSpec(lightColorSpec), 
			_isLooped(isLooped), _isLooping(false), _loopRestart(false),
			_compoName(compoName), _compoMsec(compoMsec), _compoTimer(0), _compoIsLoaded(false),
			_movObj(0), _hhfxScene(0)
	{
		_type = _isLooped || _compoMsec? TGraphicalType::DYNAMIC : TGraphicalType::NONE;

		_hhfxParams["pack"] =  Graphics::CServer::getSingletonPtr()->getHHFXBase()->GetHHFXPackExplorer().GetPack();
		_hhfxParams["fx"]	=  Graphics::CServer::getSingletonPtr()->getHFXLongName(hfx);
		_hhfxParams["run"]	=  "yes";	// Por defecto: ejecución inmediata al crear el MO
	}

	

	/**************
		FLUJO
	****************/

	void CParticleSystem::start()
	{	
		if(!_movObj) {
			// spawn a new effect at this location
			Ogre::MovableObject	*mo = getSceneMgr()->createMovableObject("HHFX", &_hhfxParams);
				assert(mo && "Error al crear ParticleSystem");	

			// set this class to listen to the ps, to be notified when it is destroyed.
			_movObj = static_cast<IHHFXOgre*>(mo);
				_movObj->SetFXListener(this);
				_node->attachObject(_movObj);

			// Adding compositors for post fx -> Sólo en el primer start: 
			if( _compoName.length() && !_compoIsLoaded) { // en el load no se puede porque la scena no está activa todavía
				_scene->compositorAdd(_compoName);
				_compoIsLoaded = true;
			}

			_isLooping = _isLooped;
		}
	}

	//--------------------------------------------------------

	// UNDONE FRS: No se puede usar el método StopFX => 
	// Inconsistencias y pescaillas que se muerden la cola con evento StoppedFX
	void CParticleSystem::stop()
	{
		if(_movObj) 
			_isLooping = false;				
	}


	/**********************
		HHFX LISTENER
	***********************/

	// Comienzo de la emisión de partículas
	void CParticleSystem::OnFXStarted(IHHFX* obj) 
	{
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");

		if(_fxLight) 
			_fxLight->setVisible(true);

		if( _compoName.length() ) {
			CServer::getSingletonPtr()->compositorEnable(_compoName);
			_compoTimer = _compoMsec / 1000.0f;
		}
	}

	//--------------------------------------------------------
	// Fin de la emisión de partículas.
	// Called when an effect stopped by itself or when the hhfx scene is cleared
	void CParticleSystem::OnFXStopped(IHHFX* obj)
	{		
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");
			
		getSceneMgr()->destroyMovableObject(_movObj); 		
		_movObj = 0;

		if(_isLooping) 
			_loopRestart = true;
		else if (_fxLight)
			_fxLight->setVisible(false); // destroy the light created under ElectricOrb		
		
	}

	


	/**********************
		SCENE ELEMENT
	***********************/

	bool CParticleSystem::load()
	{
		// Orientacion initial rotada 180 hacia -X (cara frontal de nuestras entidades)
		static const Quaternion ORIENTATION(Ogre::Radian( Math::PI ), Vector3::UNIT_Y);

		assert( getSceneMgr()->hasSceneNode( _parentName + "_node") );

		try{		
			_hhfxScene = _scene->getHHFXScene();

			// Sin nodo padre -> relativePos = absolutePos (hijo de root)
			if( _parentName == "root" || !getSceneMgr()->hasSceneNode( _parentName + "_node") )
				_node = getSceneMgr()->getRootSceneNode()
					->createChildSceneNode(_relativePos, ORIENTATION);

			else // con nodo padre
				_node = getSceneMgr()->getSceneNode( _parentName + "_node") 
					->createChildSceneNode(_relativePos, ORIENTATION);

			/* Quito la creación de luces dinámicas porque le está dando problemas a PeP de rendimiento
			// create a light if defined
			if ( _lightColorDiff + _lightColorSpec != Vector3::ZERO)  {			
				_fxLight = getSceneMgr()->createLight();
				_fxLight->setVisible(false);
				_fxLight->setType(Ogre::Light::LT_POINT);	
				_fxLight->setDiffuseColour( _lightColorDiff .x, _lightColorDiff .y, _lightColorDiff .z);
				_fxLight->setSpecularColour(_lightColorSpec .x, _lightColorSpec .y, _lightColorSpec .z);
				_fxLight->setAttenuation(160, 1.0, 0.027, 0.0028);		
				_fxLight->setPosition(Vector3::UNIT_Y * 0.8f); 	// Segun HHFX sample para centrar la luz en la esfera (si worldScale = 1)
				_node->attachObject(_fxLight);					
			}
			*/
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

		if (_fxLight) {
			getSceneMgr()->destroyLight(_fxLight);
			_fxLight = 0;
		}
		if(_movObj)	{		
			getSceneMgr()->destroyMovableObject(_movObj); 
			_movObj = 0;
		}
	} // unload

	//--------------------------------------------------------
	
	void CParticleSystem::tick(float secs)
	{
		if(_compoTimer > 0 && (_compoTimer -= secs) <= 0) 
			CServer::getSingletonPtr()->compositorDisable(_compoName);

		if(_loopRestart){			
			start();
			_loopRestart = false;
		}
	} // ticks

	


	
	
	/**********************
		GET's & SET's
	***********************/

	bool CParticleSystem::isVisible() const {
		assert(_loaded && "Imprescindible haber cargado el sistema de particulas en escena primero");
		return _movObj->isVisible();
	}

	
} // namespace Graphics

