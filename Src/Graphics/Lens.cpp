// "Depth of Field" demo for Ogre
// Copyright (C) 2006  Christian Lindequist Larsen
//
// This code is in the public domain. You may do whatever you want with it.
//
// Author: Ryan Booker (eyevee99)


#include "Lens.h"


using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#undef min
#undef max
#endif

template<class T>
const T& min(const T& a, const T& b) {
  return (a < b) ? a : b;
};


template<class T>
const T& max(const T& a, const T& b) {
  return (a > b) ? a : b;
};


Lens::Lens(const Real& _focalLength, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
  init(_focalLength, _fStop, _frameSize, _circleOfConfusion);
}


Lens::Lens(const Radian& _fieldOfView, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
  init(_fieldOfView, _fStop, _frameSize, _circleOfConfusion);
}


void Lens::init(const Real& _focalLength, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
  mFocalLength = _focalLength;
  mFStop = _fStop;
  mFrameSize = _frameSize;
  mCircleOfConfusion = _circleOfConfusion;
  recalculateFieldOfView();
}


void Lens::init(const Radian& _fieldOfView, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
  mFieldOfView = _fieldOfView;
  mFStop = _fStop;
  mFrameSize = _frameSize;
  mCircleOfConfusion = _circleOfConfusion;
  recalculateFocalLength();
}


void Lens::setFrameSize(const Real& _frameSize) {
  mFrameSize = _frameSize;
  recalculateFieldOfView();
}


void Lens::setFocalDistance(const Real& _focalDistance) {
  mFocalDistance = max(_focalDistance, Real(0));
}


void Lens::setFocalLength(const Real& _focalLength) {
  mFocalLength = max(_focalLength, Real(0.3));
  recalculateFieldOfView();
}


void Lens::setFieldOfView(const Radian& _fieldOfView) {
  mFieldOfView = min(_fieldOfView, Radian(2.8));
  recalculateFocalLength();
}


void Lens::setFStop(const Real& _fStop) {
  mFStop = max(_fStop, Real(0));
}


void Lens::recalculateFocalLength(void) {
  mFocalLength = (mFrameSize / Math::Tan(mFieldOfView / 2.0)) / 2.0;
}


void Lens::recalculateFieldOfView(void) {
  mFieldOfView = 2.0 * Math::ATan(mFrameSize / (2.0 * mFocalLength));
}


void Lens::recalculateHyperfocalLength(void) {
  mHyperfocalLength = (mFocalLength * mFocalLength) / (mFStop * mCircleOfConfusion) + mFocalLength;
}


void Lens::recalculateDepthOfField(Real& _nearDepth, Real& _focalDepth, Real& _farDepth) {
  // Set focalDepth to the current focalDistance
  _focalDepth = mFocalDistance;

  // Recalculate the Hyperfocal length
  recalculateHyperfocalLength();

  // Calculate the numerator of the optics equations
  Real numerator = (mFocalDistance * (mHyperfocalLength - mFocalLength));

  Real nearClear = numerator / (mHyperfocalLength + mFocalDistance - (2.0 * mFocalLength));

  // Adjust the nearDepth relative to the aperture. This is an approximation.
  _nearDepth = min(nearClear - nearClear * mFStop, (Real)0);

  if(mFocalDistance < mHyperfocalLength)
  {
    // Calculate the far clear plane
    Real farClear = numerator / (mHyperfocalLength - mFocalDistance);

    // Adjust the farDepth relative to the aperture. This is an approximation.
    _farDepth = farClear + farClear * mFStop;
  }

  // Far depth of field should be infinite
  else
    _farDepth = Math::POS_INFINITY;
}
