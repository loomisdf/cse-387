#ifndef ___BEHAVIOR_H___
#define ___BEHAVIOR_H___

#include "BasicIncludesAndDefines.h"

#include "VisibleObject.h" 

// Base class for objects this manipulate properties of 
// VisaulObjects over time.
class Behavior
{
public:

	// Allows VisualObject object to set the protected
	// target data member.
	friend class VisibleObject;

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);

protected:

	// Reference to the VisualObject that is being manipulated
	VisibleObject* target;

}; // end Behavior class

#endif /* ___BEHAVIOR_H___ */
