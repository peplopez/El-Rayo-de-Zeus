// "Depth of Field" demo for Ogre
// Copyright (C) 2006  Christian Lindequist Larsen
//
// This code is in the public domain. You may do whatever you want with it.
//
// Author: Ryan Booker (eyevee99)

#ifndef __Lens_H__
#define __Lens_H__

#include "OgrePrerequisites.h"

class Lens {
public:
  Lens(void) {};
  Lens(const Ogre::Real& _focalLength, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);
  Lens(const Ogre::Radian& _fieldOfView, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);
  ~Lens(void) {};

  void init(const Ogre::Real& _focalLength, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);
  void init(const Ogre::Radian& _fieldOfView, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);

  void setFrameSize(const Ogre::Real& _frameSize);
  void setFocalDistance(const Ogre::Real& _focalDistance);
  void setFocalLength(const Ogre::Real& _focalLength);
  void setFieldOfView(const Ogre::Radian& _fieldOfView);
  void setFStop(const Ogre::Real& _fStop);

  const Ogre::Real& getFrameSize(void)            { return mFrameSize; }
  const Ogre::Real& getFocalDistance(void)        { return mFocalDistance; }
  const Ogre::Real& getFocalLength(void)          { return mFocalLength; }
  const Ogre::Radian& getFieldOfView(void)        { return mFieldOfView; }
  const Ogre::Real& getFStop(void)                { return mFStop; }
  const Ogre::Real& getHyperfocalLength(void)     { return mHyperfocalLength; }

  void recalculateDepthOfField(Ogre::Real& _nearDepth, Ogre::Real& _focalDepth, Ogre::Real& _farDepth);


protected:
  // NOTE: All units must be the same, eg mm, cm or m etc
  // Primary attributes
  Ogre::Real mFrameSize;                // Film stock/sensor size, arbitrarily selected to help mimic the properties of film, eg 35mm, 3.5cm, 0.035m etc
  Ogre::Real mCircleOfConfusion;        // The area within which the depth of field is clear, it's tied to frame size, eg 0.03mm, 0.003cm, 0.0003m etc
  Ogre::Real mFocalDistance;            // The distance to the object we are focusing on
  Ogre::Real mFocalLength;              // Focal length of the lens, this directly effects field of view, we can do anything from wide angle to telephoto as we don't have the limitations of physical lenses.  Focal length is the distance from the optical centre of the lens to the film stock/sensor etc
  Ogre::Real mFStop;                    // FStop number, ie aperture, changing the aperture of a lens has an effect of depth of field, the narrower the aperture/higher the fstop number, the greater the depth of field/clearer the picture is.

  // Secondary attributes
  Ogre::Real mHyperfocalLength;         // The hyperfocal length is the point at which far depth of field is infinite, ie if mFocalDistance is >= to this value everythig will be clear to infinity
  Ogre::Radian mFieldOfView;            // Field of view of the lens, directly related to focal length


private:
  void recalculateHyperfocalLength(void);
  void recalculateFieldOfView(void);
  void recalculateFocalLength(void);
};

#endif
