
#ifndef GLOWMATERIALLISTENER_H__
#define GLOWMATERIALLISTENER_H__
 
#include <Ogre.h>
#include <OgreMaterialManager.h>
 
namespace Graphics
{
	class GlowMaterialListener : public Ogre::MaterialManager::Listener
	{
	protected:
		Ogre::MaterialPtr mBlackMat;

	public:

		GlowMaterialListener();
 
		Ogre::Technique *handleSchemeNotFound(unsigned short, const Ogre::String& schemeName, Ogre::Material*mat, unsigned short, const Ogre::Renderable*);
	};
}

 
#endif //GLOWMATERIALLISTENER_H__