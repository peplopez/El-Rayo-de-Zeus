// "Depth of Field" demo for Ogre
// Copyright (C) 2006  Christian Lindequist Larsen
//
// This code is in the public domain. You may do whatever you want with it.

#include "stdafx.h"
#include "DepthOfFieldEffect.h"
#include "Utils.h"
#include "CameraManager.h"
#include "StereoManager.h"

using namespace Ogre;

const int DepthOfFieldEffect::BLUR_DIVISOR = 5;
int DepthOfFieldEffect::DoF_ID = 0;

DepthOfFieldEffect::DepthOfFieldEffect(Viewport* viewport)
	: mViewport(viewport)
	, mNearDepth(10.0)
	, mFocalDepth(100.0)
	, mFarDepth(190.0)
	, mFarBlurCutoff(1.0)
{
	DoF_ID++;
	createDepthRenderTexture();
	addCompositor();
}

DepthOfFieldEffect::~DepthOfFieldEffect()
{
	removeCompositor();
	destroyDepthRenderTexture();
}

void DepthOfFieldEffect::setFocalDepths(float nearDepth, float focalDepth, float farDepth)
{
	mNearDepth = nearDepth;
	mFocalDepth = focalDepth;
	mFarDepth = farDepth;
}

void DepthOfFieldEffect::setFarBlurCutoff(float cutoff)
{
	mFarBlurCutoff = cutoff;
}

bool DepthOfFieldEffect::getEnabled() const
{
	return mCompositor->getEnabled();
}

void DepthOfFieldEffect::setEnabled(bool value)
{
	mCompositor->setEnabled(value);
}

void DepthOfFieldEffect::createDepthRenderTexture()
{
	// Create the depth render texture
	TexturePtr depthTexture = TextureManager::getSingleton().createManual(
		"DoF_Depth"  + StringConverter::toString(DoF_ID),
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D, mViewport->getActualWidth(), mViewport->getActualHeight(),
		0, PF_L8, TU_RENDERTARGET);

	// Get its render target and add a viewport to it
	RenderTexture* depthTarget = depthTexture->getBuffer()->getRenderTarget();
	Viewport* depthViewport = depthTarget->addViewport(mViewport->getCamera());

	// Register 'this' as a render target listener
	depthTarget->addListener(this);

	// Get the technique to use when rendering the depth render texture
	mDepthMaterial = MaterialManager::getSingleton().getByName("DoF_Depth");
	mDepthMaterial->load(); // needs to be loaded manually
	mDepthTechnique = mDepthMaterial->getBestTechnique();
	
	try {
		// Create a custom render queue invocation sequence for the depth render texture
		RenderQueueInvocationSequence* invocationSequence =
		Root::getSingleton().createRenderQueueInvocationSequence("DoF_Depth");

		// Add a render queue invocation to the sequence, and disable shadows for it
		RenderQueueInvocation* invocation = invocationSequence->add(RENDER_QUEUE_MAIN, "main");
		invocation->setSuppressShadows(true);
	}
	catch(Exception e)
	{
		// do nothing, RenderQueueInvocationSequence already created
	}

	// Set the render queue invocation sequence for the depth render texture viewport
	depthViewport->setRenderQueueInvocationSequenceName("DoF_Depth");
}

void DepthOfFieldEffect::destroyDepthRenderTexture()
{
	// TODO
}

void DepthOfFieldEffect::addCompositor()
{
	// Add the compositor to the viewport
	mCompositor = CompositorManager::getSingleton().addCompositor(
		mViewport, "DoF_Compositor" + StringConverter::toString(DoF_ID));

	// Register 'this' as a listener
	mCompositor->addListener(this);
}

void DepthOfFieldEffect::removeCompositor()
{
	// TODO
}

void DepthOfFieldEffect::notifyMaterialSetup(uint32 passId, MaterialPtr& material)
{
	switch (passId)
	{
	case BlurPass:
		{
			Vector3 pixelSize = Vector3(
				1.0f / (mViewport->getActualWidth() / BLUR_DIVISOR),
				1.0f / (mViewport->getActualHeight() / BLUR_DIVISOR),
				1.0);

			// Adjust fragment program parameters
			GpuProgramParametersSharedPtr fragParams =
				material->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			
			if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("pixelSize")))
				fragParams->setNamedConstant("pixelSize", pixelSize );

			break;
		}

	case OutputPass:
		{
			Vector3 pixelSizeScene = Vector3(
				1.0f / (mViewport->getActualWidth()),
				1.0f / (mViewport->getActualHeight()),
				1.0);

			Vector3 pixelSizeBlur = Vector3(
				1.0f / (mViewport->getActualWidth() / BLUR_DIVISOR),
				1.0f / (mViewport->getActualHeight() / BLUR_DIVISOR),
				1.0);

			// Adjust fragment program parameters
			GpuProgramParametersSharedPtr fragParams =
				material->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			
			if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("pixelSizeScene")))
				fragParams->setNamedConstant("pixelSizeScene", pixelSizeScene );
			
			if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("pixelSizeBlur")))
				fragParams->setNamedConstant("pixelSizeBlur", pixelSizeBlur );

			break;
		}
	}
}

void DepthOfFieldEffect::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
	Vector4 dofParams(mNearDepth, mFocalDepth, mFarDepth, mFarBlurCutoff);

	// Adjust fragment program parameters for depth pass
	GpuProgramParametersSharedPtr fragParams =
		mDepthTechnique->getPass(0)->getFragmentProgramParameters();
	
	if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("dofParams")))
		fragParams->setNamedConstant("dofParams", dofParams);

	// Add 'this' as a RenderableListener to replace the technique for all renderables
	RenderQueue* queue = evt.source->getCamera()->getSceneManager()->getRenderQueue();
	queue->setRenderableListener(this);
}

void DepthOfFieldEffect::postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
	// Reset the RenderableListener
	RenderQueue* queue = evt.source->getCamera()->getSceneManager()->getRenderQueue();
	queue->setRenderableListener(0);
}

bool DepthOfFieldEffect::renderableQueued(Renderable* rend, uint8 groupID, 
				ushort priority, Technique** ppTech, RenderQueue* pQueue)
{
	// Replace the technique of all renderables
	*ppTech = mDepthTechnique;
	return true;
}
