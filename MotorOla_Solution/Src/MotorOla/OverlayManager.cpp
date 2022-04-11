#include "OverlayManager.h"


void OverlayManager::init()
{
	    man = new Ogre::OverlayManager();
		// load the fade
		mFader = 0;
		Ogre::MaterialPtr		mFaderMaterial;
		Ogre::TextureUnitState* mTexture = 0;

		
	//	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	//	Ogre::OverlayManager* overlayManager = Ogre::OverlayManager::getSingletonPtr();
		/**if (overlayManager == nullptr) {
			std::cout << "overlaymanager is null" << std::endl;
			
		}*/

		//mFader = static_cast<Ogre::PanelOverlayElement*>(
		//	overlayManager->createOverlayElement("Panel", "Fader"));
		mFader = static_cast<Ogre::PanelOverlayElement*>(
				man->createOverlayElement("Panel", "Fader"));
		mFader->setMetricsMode(Ogre::GMM_RELATIVE);
		mFader->setPosition(0, 0);
		mFader->setDimensions(0.25f, 0.25f);
		mFader->setMaterialName("BotonTest"); // Optional background material

		// Ensures that the material exists
		mFader->setUV(0, 0, 0.5, 1);

		// show the fade
		mFader->show();

		// Create an overlay, and add the panel
		Ogre::Overlay* mOverlay = 0;
		//mOverlay = overlayManager->create("FaderOverlay");
		mOverlay = man->create("FaderOverlay");
		mOverlay->add2D(mFader);
		mOverlay->show();
}


