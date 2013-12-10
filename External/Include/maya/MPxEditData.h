#ifndef _MPxEditData
#define _MPxEditData
//-
// ===========================================================================
// Copyright 2013 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MPxEditData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


// ****************************************************************************
// CLASS DECLARATION (MPxEditData)

//! \ingroup OpenMaya MPx
//! \brief Base class used to associate user-defined data with edits
/*!
  
MPxEditData is a pure virtual base class, used to derive custom
data objects which can be associated with individual MEdit objects.

A small set of comparison methods need to be defined to support query operations
and use of associative data structures.  The base class defines no other 
data or behavior.  

Derived classes are free to add data members and methods as required.

<h2>Use in Python</h2>

<h3>Python Accessor Methods _setValue and _getValue</h3>

It is expected that classes derived from MPxEditData will define one or more data
members to store the object's value and this data will be used by the virtual 
methods performIsEqual and performIsLessThan for comparison. 

In Python, derived class objects passed to the virtual comparison method 
implementations will only be visible as MPxEditData base class objects, not the actual
derived class objects.

For this reason, Python classes derived from MPxEditData have two additional methods, 
\b _setValue and \b _getValue that provide a mechanism for the derived class to
access its custom data value using the base class interface. 

Since these interfaces are defined directly in Python, the data value can be any valid Python type.     
\code
def _setValue(data):
   # Called by the class to store the data value of the object
   # The data value can be of any valid Python type 

def _getValue():
   # Called by the class to retrieve the data value that was
   # previously set using _setValue
\endcode

<h3>Ownership of the MPxEditData Object</h3>

MPxEditData objects associated with an MEdit must be dynamically allocated.  
Ownership of the editData will be assumed by Maya and subsequent management 
of the data object, including its eventual deletion will be handled along with the 
edit it is associated with.  For this reason, an editData object should only be 
associated with a single edit, the same object should not be shared or associated 
with other edits. If multiple edits have editData of the same value, each edit must 
have its own unique copy. 

In Python, the OpenMayaMPx asMPxPtr method is used to transfer ownership of a dynamically allocated
object to Maya. The Python example below demonstrates how this could be implemented.


<h3>Python Example</h3>

\code

# Example MPxEditData class implemented in Python

import Maya.OpenMayaMPx as OpenMayaMPx

class testEditDataType(OpenMayaMPx.MPxEditData):
    """
    Simple tag class that is intended to store a string value
    """
    _tagValue = ''
    def __init__(self, tagValue):
        OpenMayaMPx.MPxEditData.__init__(self)
        # Python derived classes need to use _setValue() here
        # and then _getValue() in performIsEqual/performIsLessThan methods
        # (because python does not have a dynamic_cast like C++ to use
        # on variable 'other' to get access to the derived class data)
        self._tagValue = tagValue
        self._setValue(self._tagValue)

    def performIsEqual(self, other):
        # Note: use of _getValue is not actually necessary for accessing
        # data members of 'self'. It is however necessary for 'other' 
        # since 'other' is only available here as a base class MPxEditData object.
        thisValue = self._getValue()
        otherValue = other._getValue()
        return bool(thisValue == otherValue)
 
    def performIsLessThan(self, other):
        thisValue = self._getValue()
        otherValue = other._getValue()
        return bool(thisValue < otherValue)

    @staticmethod
    def creator(tagValue):
        """
        Create a tag with the given value.
        This creator method correctly returns pointer whose ownership
        has been transferred to maya. 
        """ 
        return OpenMayaMPx.asMPxPtr(testEditDataType(tagValue))

\endcode

*/

class FND_EXPORT MPxEditData 
{
public:

	MPxEditData();
	virtual 		~MPxEditData();

	bool	isEqual(const MPxEditData *other) const;	
	bool	isLessThan(const MPxEditData *other) const;	

	static const char* className();	

protected:

	// Derived classes must implement these comparison methods
	virtual	bool performIsEqual(const MPxEditData *other) const = 0;	
	virtual	bool performIsLessThan(const MPxEditData *other) const = 0;	

    // INTERNAL SCRIPT USE ONLY
    // Do not access _dataValue directly, scripts should use accessors _setValue and _getValue	
public:
	void* _dataValue;  

private:

};


#endif /* __cplusplus */
#endif /* _MPxEditData */
