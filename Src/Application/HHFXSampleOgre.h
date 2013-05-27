//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HHFXSampleOgre_h_
#define __HHFXSampleOgre_h_

#include "BaseApplication.h"
#include "IHHFXPublic.h"

#pragma warning(disable : 4481)	// nonstandard extension used: override specifier 'override'

namespace HellHeaven
{
	// this is the infos the hhfx plugin needs to compute the particles
	class CRendererSubView
	{
	public:
		CRendererSubView()
		:	m_hhfxScene(NULL)
		,	m_viewMatrix(Ogre::Matrix4::IDENTITY)
		,	m_sceneManager(NULL)
		,	m_customMaterial("")
		,	m_usePostFX(false)
		{}
		~CRendererSubView()
		{}

		inline const Ogre::Matrix4&	viewMatrix() const		{ return m_viewMatrix; }
		inline Ogre::SceneManager&	sceneManager() const	{ assert(m_sceneManager != NULL); return *m_sceneManager; }
		inline const Ogre::String&	customMaterial() const	{ return m_customMaterial; }
		inline bool					usePostFX() const		{ return m_usePostFX; }
		inline IHHFXScene&			hhfxScene() const		{ assert(m_hhfxScene != NULL); return *m_hhfxScene; }

		inline void					setViewMatrix(const Ogre::Matrix4 &view)				{ m_viewMatrix = view; }
		inline void					setSceneManager(Ogre::SceneManager& sceneManager)		{ m_sceneManager = &sceneManager; }
		inline void					setCustomMaterial(const Ogre::String& customMaterial)	{ m_customMaterial = customMaterial; }
		inline void					setUsePostFX(bool usePostFX)							{ m_usePostFX = usePostFX; }
		inline void					setHHFXScene(IHHFXScene& HHFXScene)						{ m_hhfxScene = &HHFXScene; }

	private:
		// the scene that called the render methods
		IHHFXScene*					m_hhfxScene;
		Ogre::Matrix4				m_viewMatrix;
		Ogre::SceneManager*			m_sceneManager;
		Ogre::String				m_customMaterial;
		bool						m_usePostFX;
	};
}

// NOTE: switched createFrameListener() before createScene() in BaseApplication::setup to have ALL initialized before createScene is called
class CHHFXSampleOgre : public BaseApplication, public IHHFX::IFXListener
{
public:
	CHHFXSampleOgre();
	virtual	~CHHFXSampleOgre();

protected:
	// base application implementation
	virtual void				createScene() override;
	virtual void				destroyScene() override;

	// framelistener implementation
	virtual bool				frameStarted(const Ogre::FrameEvent& evt) override;
	virtual bool				frameRenderingQueued(const Ogre::FrameEvent& evt) override;

	// ois implementation
	virtual bool				keyPressed(const OIS::KeyEvent& evt) override;
	virtual bool				mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id) override;

	// IHHFX::IFXListener implementation
	virtual void				OnFXStarted(IHHFX *obj) override;
	virtual void				OnFXStopped(IHHFX *obj) override;

private:
	// internal helper methods
//	void						_TogglePause();
	//	bool						_IntersectGrid(Ogre::Vector3& intersectPos);
	static bool					_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);
	void						_AnimateLastSpawnedNode(float elapsedTime);
	void						_createFX(const std::string &fxName, const std::string &fxPack, const Ogre::Vector3 &pos);

	// TODO Manager singleton HHFX init, release, etc....
	void                        _loadHHFXPackage(const std::string& fxPack);
	void                        _initHHFX();
	void						_loadHHFXResources();
	void						_loadHHFXTextures();
	void						_loadHHFXCompositors();

private:
	Ogre::Plane					m_floor;

	// controls
	bool						m_simulationPaused;

	// HellHeaven's elements
	IHHFXScene*					m_hhfxScene;
	Ogre::SceneNode*			m_lastSpawnedNode;
	Ogre::Vector3				m_lastSpawnedPosition;
};

#endif
