#ifndef __HellHeaven_RendererSubView_H
#define __HellHeaven_RendererSubView_H

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

#endif