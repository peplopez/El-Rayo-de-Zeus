// "Depth of Field" demo for Ogre
// Copyright (C) 2006  Christian Lindequist Larsen
//
// This code is in the public domain. You may do whatever you want with it.

#ifndef __DepthOfFieldEffect_H__
#define __DepthOfFieldEffect_H__

#include "OgrePrerequisites.h"
#include "OgreCompositorInstance.h"
#include "OgreRenderTargetListener.h"
#include "OgreFrameListener.h"
#include "OgreRenderQueue.h"

class DepthOfFieldEffect : public Ogre::CompositorInstance::Listener,
						   public Ogre::RenderTargetListener,
						   public Ogre::RenderQueue::RenderableListener
{
public:
	DepthOfFieldEffect(Ogre::Viewport* viewport);
	~DepthOfFieldEffect();

	float getNearDepth() const {return mNearDepth; }
	float getFocalDepth() const {return mFocalDepth; }
	float getFarDepth() const {return mFarDepth; }
	void setFocalDepths(float nearDepth, float focalDepth, float farDepth);
	float getFarBlurCutoff() const {return mFarBlurCutoff; }
	void setFarBlurCutoff(float cutoff);
	bool getEnabled() const;
	void setEnabled(bool value);

private:
	static const int BLUR_DIVISOR;
	static int DoF_ID;

	enum PassId
	{
		BlurPass,
		OutputPass
	};

	Ogre::Viewport* mViewport;
	Ogre::MaterialPtr mDepthMaterial;
	Ogre::Technique* mDepthTechnique;
	Ogre::CompositorInstance* mCompositor;
	float mNearDepth;
	float mFocalDepth;
	float mFarDepth;
	float mFarBlurCutoff;

	void createDepthRenderTexture();
	void destroyDepthRenderTexture();
	void addCompositor();
	void removeCompositor();

	// Implementation of Ogre::CompositorInstance::Listener
	virtual void notifyMaterialSetup(Ogre::uint32 passId, Ogre::MaterialPtr& material);

	// Implementation of Ogre::RenderTargetListener
	virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
	virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);

	// Implementation of Ogre::RenderQueue::RenderableListener
	virtual bool renderableQueued(Ogre::Renderable* rend, Ogre::uint8 groupID, 
				Ogre::ushort priority, Ogre::Technique** ppTech, Ogre::RenderQueue* pQueue);
};

#endif // __DepthOfFieldEffect_H__
