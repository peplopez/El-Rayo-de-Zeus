//---------------------------------------------------------------------------
// ParticleSystem.h
//---------------------------------------------------------------------------

/**
@file ParticleSystem.h

Contiene la declaración de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author FRS
@date Mayo, 2013
*/

#ifndef __Graphics_ParticleSystem_H
#define __Graphics_ParticleSystem_H

#include <HHFX/IHHFXPublic.h>

#include "SceneElement.h"


namespace Graphics 
{
	/**
	Clase de ParticleSysteme.
	
	@ingroup graphicsGroup

	@author FRS
	@date Mayo, 2013
	*/
	class CParticleSystem : public CSceneElement, public IHHFX::IFXListener
	{
	public:

		/**
		Constructor de la clase.		
		*/		
		CParticleSystem(const std::string& hfx, const std::string& parentName, bool isLooped = false,
			const Vector3& relativePos = Vector3::ZERO, 
			const Vector3& lightDiffuse = Vector3::ZERO, const Vector3& lightSpecular = Vector3::ZERO);


		/*****************
			GET's & SET'S
		*******************/

		bool isVisible() const;
		void start(); // TODO Si solo tenemos start -> renombrar a RUN
		void stop(); 	


	private:		
		
		std::string _parentName;
		Vector3 _relativePos;	// Pos relativa respecto al padre (particulas son hijas de otra entidad gráfica)
		

		//-------- SceneElement implementation --------------
		bool				load(); 
		void				unload();	
		void				tick(float secs);
	


		/*********************
			HELL HEAVENS FX		
		**********************/				
		
		IHHFXOgre*				_movObj;	// Sistema de particulas (movable object)		
		IHHFXScene*				_hhfxScene; // HHFX Scene
		std::string				_hfx;
		Ogre::NameValuePairList _hhfxParams; // effect's params

		//------------------ FX LIGHT --------------------------------
		Ogre::Light*			_fxLight; // luz asociada al fx (si corresponde)
		Vector3					_lightDiffuse;
		Vector3					_lightSpecular;
		
		//----------------- LOOPING -----------------------
		bool					_isLooped;
		bool					_isLooping; // _isLooped + started (!stopped)
		bool					_loopRestart;	
	
		//--------- IHHFX::IFXListener implementation -------------------
		void				OnFXStarted(IHHFX *obj);
		void				OnFXStopped(IHHFX *obj);


	//	static bool					_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);

	}; // class CParticleSystem

} // namespace Graphics

#endif // __Graphics_ParticleSystem_H
