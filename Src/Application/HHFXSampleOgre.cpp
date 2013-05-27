//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "HHFXSampleOgre.h"

#include <OgreCompositorManager.h>
#include <OgreMeshManager.h>
#include <OgreCompositorInstance.h>


using namespace Ogre;
//using namespace OgreBites;

//-------------------------------------------------------------------------------------
// WARNING : if != 1.0, This scale must be taken into account when setting and getting arbitrary particle attributes !

const float	kWorldFxScale = 1.0f;
const int FX_SELECTED = 0;

//-------------------------------------------------------------------------------------

CHHFXSampleOgre::CHHFXSampleOgre()
:	
//m_selectMenuPack(NULL)
//,	m_selectMenuFx(NULL)
//,	m_labelTotalParticles(NULL)
//,	m_buttonPauseSimulation(NULL)
//,	m_buttonClearScene(NULL)
//,	m_sliderSimulationSpeed(NULL)
//,	m_checkBoxShowGrid(NULL)
//,	m_checkBoxAnimate(NULL)
//,
m_simulationPaused(false)
,	m_hhfxScene(NULL)
,	m_selectedFx(-1)
,	m_lastSpawnedNode(NULL)
,	m_lastSpawnedPosition(Vector3::ZERO)
{
}

//-------------------------------------------------------------------------------------

CHHFXSampleOgre::~CHHFXSampleOgre()
{
}

//-------------------------------------------------------------------------------------

//void CHHFXSampleOgre::_TogglePause()
//{
//	static Real simulationTime = 1.0f;
//
//	// pause
//	m_simulationPaused = !m_simulationPaused;
//	if (m_simulationPaused)
//	{
//		m_buttonPauseSimulation->setCaption("Space: Run  ");
//		simulationTime = m_sliderSimulationSpeed->getValue();
//		m_sliderSimulationSpeed->setValue(0.0f, true);
//	}
//	// undo pause
//	else
//	{
//		m_buttonPauseSimulation->setCaption("Space: Pause");
//		m_sliderSimulationSpeed->setValue(simulationTime, true);
//	}
//}

//-------------------------------------------------------------------------------------

//bool CHHFXSampleOgre::_IntersectGrid(Vector3& intersectPos)
//{
//	// build a ray from the camera to the mouse
//	Vector2 mousePos(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
//	Vector2 mousePosScreen(	mousePos.x / mCamera->getViewport()->getActualWidth(),
//							mousePos.y / mCamera->getViewport()->getActualHeight());
//
//	Ray mouseRay = mCamera->getCameraToViewportRay(mousePosScreen.x, mousePosScreen.y);
//
//	std::pair<bool, Real> result = mouseRay.intersects(m_floor);
//
//	// plane intersected
//	if (result.first == true)
//	{
//		intersectPos = mouseRay.getPoint(result.second);
//
//		// stay on our "visible" plane
//		result.first = false;
//		if (intersectPos.x > -100 && intersectPos.x < 100 && intersectPos.z > -100 && intersectPos.z < 100)
//			result.first = true;
//	}
//	return result.first;
//}

//-------------------------------------------------------------------------------------
// simplistic collision with our ground-plane (200 units-wide)

bool	CHHFXSampleOgre::_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport)
{
	CHHFXSampleOgre	*self = (CHHFXSampleOgre*)arg;
	Ray	traceRay(start, direction);

	std::pair<bool, Real> result = traceRay.intersects(Ogre::Plane( Vector3(0,0,1) , Vector3(0,0,0) ) );
	//std::pair<bool, Real> result = traceRay.intersects(self->m_floor);

	// plane intersected
	if (result.first == true)
	{
		contactReport.m_Point = traceRay.getPoint(result.second);

		// stay on our "visible" plane
		result.first = false;
		if (contactReport.m_Point.x > -100 && contactReport.m_Point.x < 100 &&
			contactReport.m_Point.z > -100 && contactReport.m_Point.z < 100)
		{
			result.first = true;

			contactReport.m_Time = result.second;
			contactReport.m_Normal = Vector3(0,0,1);
			//contactReport.m_Normal = self->m_floor.normal;
		}
	}
	return result.first;
}

//-------------------------------------------------------------------------------------

void CHHFXSampleOgre::createScene()
{
	/////////////////////////////////////////////////////////////////////////////////////////
	// preload the distortion texture with hardware gamma correction
	/////////////////////////////////////////////////////////////////////////////////////////

	LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] ---------- preload texture hardware gamma ----------");

	// ok then, load all the textures in the HellHeaven resource group
	String resourceGroup("HellHeaven");

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	ResourceGroupManager::getSingleton().loadResourceGroup(resourceGroup);

	StringVectorPtr strVectorPtr = ResourceGroupManager::getSingleton().listResourceNames(resourceGroup);

	for (unsigned int i = 0; i < strVectorPtr->size(); ++i)
	{
		String texName = strVectorPtr->at(i);

		try
		{
			LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] Trying to load " + texName + " as texture...");

			// should load all the textures with gamma correction 
			// except the ones used for distortion post effect
			if (!texName.compare("ParticleDeformBlur_01.dds") ||
				!texName.compare("RainDeform_01.dds") ||
				!texName.compare("RainDropsDeform_01.dds"))
			{
				TexturePtr pTex = TextureManager::getSingleton().load(
																		texName, resourceGroup,
																		TEX_TYPE_2D, MIP_DEFAULT,
																		1.0f, false, PF_UNKNOWN, false);
			}
			else
			{
				// gamma corrected
				TexturePtr pTex = TextureManager::getSingleton().load(
																		texName, resourceGroup,
																		TEX_TYPE_2D, MIP_DEFAULT,
																		1.0f, false, PF_A8R8G8B8, true);
			}
		}
		catch (Ogre::Exception e) // texture not loaded because it surely is not one
		{
			continue;
		}
	}

	LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] ---------- done ----------");

	/////////////////////////////////////////////////////////////////////////////////////////
	// hud
	/////////////////////////////////////////////////////////////////////////////////////////

	//// label particles number display
	//m_labelTotalParticles = mTrayMgr->createLabel(TL_TOP, "labelTotalParticles", "particles: 0", 175);

	//// some controls
	//m_selectMenuFx = mTrayMgr->createLongSelectMenu(TL_BOTTOM, "selectMenuFx", "Fx  ", 180, 15);
	//m_sliderSimulationSpeed = mTrayMgr->createThickSlider(TL_BOTTOM, "SimulationSpeed", "Simulation speed", 250, 85, 0, 5, 100);
	//m_sliderSimulationSpeed->setValue(1.0f, false);

	//m_buttonClearScene = mTrayMgr->createButton(TL_BOTTOM, "buttonClearScene", "C: Clear scene", 140);
	//m_buttonPauseSimulation = mTrayMgr->createButton(TL_BOTTOM, "buttonPauseSimulation", "Space: Pause", 140);

	//m_checkBoxShowGrid = mTrayMgr->createCheckBox(TL_BOTTOMRIGHT, "showGrid", "Show grid", 185);
	//m_checkBoxShowGrid->setChecked(true, false);

	//m_checkBoxAnimate = mTrayMgr->createCheckBox(TL_BOTTOMRIGHT, "Animate", "Animate", 185);
	//m_checkBoxAnimate->setChecked(true, false);

	//// background color
	//mTrayMgr->createLongSlider(TL_BOTTOMRIGHT, "BkgB", "Back ", 60, 85, 0, 1, 100)->setValue(0.0f);

	/////////////////////////////////////////////////////////////////////////////////////////
	// light / camera / flag
	/////////////////////////////////////////////////////////////////////////////////////////

	// set the default visibility flag for all the movable objects, because we will use posts effects that 
	// needs to filter objects for rendering
	MovableObject::setDefaultVisibilityFlags(1);

	// setup some basic lighting for our scene
	mSceneMgr->setAmbientLight(ColourValue(0.2f, 0.2f, 0.2f));

	// set our camera to orbit around the origin and show cursor
	//mCameraMan->setStyle(CS_ORBIT);
	//mCameraMan->setYawPitchDist(Degree(45), Degree(27), 45);
	mCamera->setNearClipDistance(0.01f);
	//mTrayMgr->showCursor();
	//mCameraMan->setStyle(CS_FREELOOK);
	//mCameraMan->setTopSpeed(40);

	/////////////////////////////////////////////////////////////////////////////////////////
	// create the plane and the pointer
	/////////////////////////////////////////////////////////////////////////////////////////

	// create a plane
	{
		Entity* ent;

		m_floor.normal = Vector3::UNIT_Y;
		m_floor.d = 0;
		MeshManager::getSingleton().createPlane("FloorPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, m_floor, 200, 200, 100, 100, true, 1, 20, 20, Vector3::UNIT_Z);
		ent = mSceneMgr->createEntity("floor", "FloorPlane");
		ent->setMaterialName("HHFXSample/Grid");

		mSceneMgr->getRootSceneNode()->createChildSceneNode("nGrid")->attachObject(ent);
	}

	// create the visible spawn entity
	{
		SceneNode* spawn = mSceneMgr->getRootSceneNode()->createChildSceneNode("nSpawn");
		Entity* ent = mSceneMgr->createEntity("eSpawn", "sphere.mesh");

		ent->setMaterialName("HHFXSample/Spawn");
		spawn->setScale(Vector3::UNIT_SCALE * 0.002f);
		spawn->attachObject(ent);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// add the compositor for post fx
	/////////////////////////////////////////////////////////////////////////////////////////

	// adding compositor for post fx
	CompositorInstance*	comp = CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "HellHeavenOgre/Compositor/Distortion");

	if (!comp)
		LogManager::getSingleton().logMessage(LML_CRITICAL, "[HHFX ERROR] Cannot load compositor Distortion !");
	else
		comp->setEnabled(true);

	/////////////////////////////////////////////////////////////////////////////////////////
	// hellheaven
	/////////////////////////////////////////////////////////////////////////////////////////

	// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
	MovableObject	*fx = mSceneMgr->createMovableObject("HHFX");

	if (fx != NULL)
	{
		OgreAssert(dynamic_cast<IHHFXOgre*>(fx) != NULL, "object should be of type \"HHFX\"");
		IHHFXOgre	*hhFx = static_cast<IHHFXOgre*>(fx);
		m_hhfxScene = &hhFx->GetHHFXScene();

		// we got the hh scene, destroy the effect
		mSceneMgr->destroyMovableObject(fx);
	}
	OgreAssert(m_hhfxScene != NULL, "failed creating HHFx Scene !");

	// load a pack
	m_selectedPack = "../Packs/SampleParticlesOgre";

	bool hhfxPackLoaded = m_hhfxScene->GetHHFXBase().LoadPack(m_selectedPack.c_str(), true);
	OgreAssert(hhfxPackLoaded, "hhfx pack did not load correctly or contains no effects !");

	// bind the collision callback
	m_hhfxScene->SetCollisionCallback(this, &_IntersectScene);
	m_hhfxScene->SetWorldScale(kWorldFxScale);

	// get all the effects from this pack and add them to the fx menu, then select the first effect
	const std::vector<std::string>& fxNames = m_hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetNames();
	//for (unsigned int i = 0; i < fxNames.size(); ++i)
	//	m_selectMenuFx->addItem(fxNames[i]);
	//m_selectMenuFx->selectItem(0, true);
}

//-------------------------------------------------------------------------------------

void CHHFXSampleOgre::destroyScene()
{
	// clear the scene before shutting down ogre since the hhfx ogre implementation holds some Ogre objects.
	m_hhfxScene->Clear();

	// remove our compositor
//	CompositorManager::getSingleton().removeCompositor(mCamera->getViewport(), "HellHeavenOgre/Compositor/Distortion");

	// call the base class destroy scene method
	BaseApplication::destroyScene();
}

//-------------------------------------------------------------------------------------

bool CHHFXSampleOgre::frameStarted(const Ogre::FrameEvent& evt)
{
	static float elapsedTime = 0.0f; // update at first frame
	static float minPosY = 1.0f;
	Vector3	camPos = mCamera->getPosition();

	elapsedTime += evt.timeSinceLastFrame;

	// do not go through the floor
	if (camPos.y < minPosY)
		mCamera->setPosition(camPos.x, minPosY, camPos.z);

	// manually move the last spawned Fx if necessary:
	//if (m_checkBoxAnimate != NULL && m_checkBoxAnimate->isChecked())
	//	_AnimateLastSpawnedNode(elapsedTime);

	// update the hhfx scene
	m_hhfxScene->Update(evt.timeSinceLastFrame);
	return true;
}

//-------------------------------------------------------------------------------------

void CHHFXSampleOgre::_AnimateLastSpawnedNode(float elapsedTime)
{
	if (m_lastSpawnedNode != NULL)
	{
		// simple circular animation
		const float	kRadius = 1.0f;
		m_lastSpawnedNode->setPosition(m_lastSpawnedPosition + Vector3(kRadius * sinf(elapsedTime), 0, kRadius * cosf(elapsedTime)));
	}
}

//-------------------------------------------------------------------------------------

bool CHHFXSampleOgre::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	static float elapsedTime = 1.0f; // update at first frame
	elapsedTime += evt.timeSinceLastFrame;

	// setting render params for the particles renderers
	HellHeaven::CRendererSubView view;

	Matrix4 worldTransforms;

	const Vector3& camPos = mCamera->getPosition();
	const Quaternion& camOri = mCamera->getOrientation();

	worldTransforms.makeTransform(camPos, Vector3::UNIT_SCALE, camOri);
	worldTransforms = worldTransforms.transpose();

	view.setHHFXScene(*m_hhfxScene);
	view.setViewMatrix(worldTransforms);
	view.setSceneManager(*mSceneMgr);

	m_hhfxScene->Render(view, camPos);

	// update hud once per second with some stats
	if (elapsedTime >= 1.0f)
	{
		// update the label with total particle count
		//m_labelTotalParticles->setCaption(String("particles: ") + StringConverter::toString(m_hhfxScene->GetParticleCount()));
		elapsedTime = 0.0f;
	}

	return BaseApplication::frameRenderingQueued(evt);
}

//-------------------------------------------------------------------------------------

//bool CHHFXSampleOgre::keyPressed(const OIS::KeyEvent& evt)
//{
//	//switch (evt.key)
//	//{
//	//// pause the simulation time
//	//case OIS::KC_SPACE:
//	//	//_TogglePause();
//	//	break;
//
//	//// clear all the effects in the scene
//	//case OIS::KC_C:
//	//	m_hhfxScene->Clear();
//	//	break;
//
//	//// take a screenshot
//	//case OIS::KC_SYSRQ:
//	//	mWindow->writeContentsToTimestampedFile("HHFXOgrePlugin_", ".png");
//	//	break;
//
//	//// quit
//	//case OIS::KC_ESCAPE:
//	//	mShutDown = true; 
//	//	break;
//	//}
//
//	//mCameraMan->injectKeyDown(evt);
//	return true;
//}

//-------------------------------------------------------------------------------------

bool CHHFXSampleOgre::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//if (mTrayMgr->injectMouseDown(evt, id))
	//	return true;

//	mCameraMan->injectMouseDown(evt, id);
	switch (id)
	{
	case OIS::MB_Left:
		{
			Vector3 intersectPos(0,0,0);

			//// we want your effect to spawn on the floor
			//if (!_IntersectGrid(intersectPos))
			//	break;

			// set the red sphere at the spawn position
			mSceneMgr->getSceneNode("nSpawn")->setPosition(intersectPos);

			// effect's params
			NameValuePairList params;
			m_selectedFx = FX_SELECTED;

			params["pack"] = m_selectedPack.c_str();
			params["fx"] = m_hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetEffects()[m_selectedFx];
			params["run"] = "yes";

			// spawn a new effect at this location
			MovableObject	*fx = mSceneMgr->createMovableObject("HHFX", &params);
			if (fx != NULL)
			{
				IHHFXOgre	*hhfx = static_cast<IHHFXOgre*>(fx);

				// Here, check if the effect is still active after we've run it.
				// It might not be active anymore for fire-and-forget effects, such as an instantaneous explosion.
				// For such effects, don't bother creating a parent scene node.
				if (hhfx->IsFXActive())
				{
					// create a node to attach the effect
					SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

					node->attachObject(hhfx);
					node->setPosition(intersectPos);

					// set this class to listen to the fx, to be notified when it is destroyed, so we may remove the scene node we created:
					hhfx->SetFXListener(this);

					// set the camera's orientation to the node
					//Quaternion orientation;
					//Quaternion camOri = mCamera->getOrientation();

					//orientation.FromAngleAxis(Radian(camOri.getYaw()), Vector3::UNIT_Y);
					//node->setOrientation(orientation);

					// set the last spawned node to this one. Used in the demo to move the last spawned Fx around.
					m_lastSpawnedNode = node;
					m_lastSpawnedPosition = intersectPos;
				}
			}
			break;
		}

	//case OIS::MB_Right:
	//	mTrayMgr->hideCursor();
	//	break;
	}

	return true;
}

//-------------------------------------------------------------------------------------

//bool CHHFXSampleOgre::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
//{
//	//if (mTrayMgr->injectMouseUp(evt, id))
//	//	return true;
//
//	//mCameraMan->injectMouseUp(evt, id);
//	//switch (id)
//	//{
//	//case OIS::MB_Right:
//	//	mTrayMgr->showCursor();
//	//	break;
//	//}
//
//	return true;
//}

//-------------------------------------------------------------------------------------

//bool CHHFXSampleOgre::mouseMoved(const OIS::MouseEvent& evt)
//{
//	// camera
//	if (evt.state.buttonDown(OIS::MB_Right))
//	{
//		mCameraMan->injectMouseMove(evt);
//
//		// restore the mouse position to the center of the screen
//		OIS::MouseState& mouseState = const_cast<OIS::MouseState&>(mMouse->getMouseState());
//
//		mouseState.X.abs = mCamera->getViewport()->getActualWidth() / 2;
//		mouseState.Y.abs = mCamera->getViewport()->getActualHeight() / 2;
//		return true;
//	}
//
//	if (mTrayMgr->injectMouseMove(evt))
//		return true;
//
//	return true;
//}

//-------------------------------------------------------------------------------------

//void CHHFXSampleOgre::checkBoxToggled(CheckBox* box)
//{
//	//if (box == m_checkBoxShowGrid)
//	//	mSceneMgr->getSceneNode("nGrid")->setVisible(box->isChecked());
//}
//
////-------------------------------------------------------------------------------------
//
//void CHHFXSampleOgre::sliderMoved(Slider* slider)
//{
//	//if (slider == m_sliderSimulationSpeed)
//	//	m_hhfxScene->SetSimulationSpeed(m_sliderSimulationSpeed->getValue());
//	//else if (slider->getName() == "BkgB")
//	//{
//	//	ColourValue cVal = mCamera->getViewport()->getBackgroundColour();
//
//	//	cVal.b = slider->getValue();
//	//	mCamera->getViewport()->setBackgroundColour(cVal);
//	//}
//}
//
////-------------------------------------------------------------------------------------
//
//void CHHFXSampleOgre::itemSelected(SelectMenu* menu)
//{
//	//if (menu == m_selectMenuFx)
//	//	m_selectedFx = menu->getSelectionIndex();
//}
//
////-------------------------------------------------------------------------------------
//
//void CHHFXSampleOgre::buttonHit(Button* button)
//{
//	//if (button == m_buttonClearScene)
//	//	m_hhfxScene->Clear();
//	//else if (button == m_buttonPauseSimulation)
//	//	_TogglePause();
//}

//-------------------------------------------------------------------------------------

void CHHFXSampleOgre::OnFXStarted(IHHFX* obj)
{
	// create a light under the ElectricOrb effect
	if (strstr(obj->GetPath(), "ElectricOrb.hfx") != NULL)
	{
		IHHFXOgre* fx = static_cast<IHHFXOgre*>(obj);
		const Vector3& fxPosition = fx->getParentSceneNode()->getPosition();

		Light* pointLight = mSceneMgr->createLight("pointLight" + StringConverter::toString((unsigned int)(obj)));
		pointLight->setType(Light::LT_POINT);
		pointLight->setPosition(fxPosition + Vector3::UNIT_Y * 0.8f);
		pointLight->setDiffuseColour(0.1f, 0.1f, 1.0f);
		pointLight->setSpecularColour(0.8f, 0.8f, 1.0f);
	}
}

//-------------------------------------------------------------------------------------

// called when an effect stopped by itself or when the hhfx scene is cleared
void CHHFXSampleOgre::OnFXStopped(IHHFX* obj)
{
	// destroy the light created under ElectricOrb
	if (strstr(obj->GetPath(), "ElectricOrb.hfx") != NULL)
		mSceneMgr->destroyLight("pointLight" + StringConverter::toString((unsigned int)(obj)));

	// destroy the created node's effect parent and the effect
	IHHFXOgre	*fx = static_cast<IHHFXOgre*>(obj);
	SceneNode	*parentNode = fx->getParentSceneNode();

	if (parentNode == m_lastSpawnedNode)
	{
		m_lastSpawnedNode = NULL;
		m_lastSpawnedPosition = Vector3::ZERO;
	}

	if (parentNode != NULL)
		mSceneMgr->destroySceneNode(parentNode);
	mSceneMgr->destroyMovableObject(fx);
}

//-------------------------------------------------------------------------------------
