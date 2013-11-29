#ifndef adskRefCounted_h
#define adskRefCounted_h

#include <adskCommon.h>

// ****************************************************************************
/*!
	\class adsk::RefCounted
 	\brief Class implementing standard reference counting

	Simple abstract reference counting class, derive from it and implement
	the destructor, copy constructor, and assignment operator. Maintains a
	reference count and indicates deletion is safe when the count drops to
	zero.
*/

namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};

	class METADATA_EXPORT RefCounted
	{
		public:
			// Note the 'const' trick to allow objects inheriting from this class
			// to maintain const members even though the ref counts are changing.
			void	ref			() const { ++const_cast<RefCounted*>(this)->fRefCount; }
			void	unref		() const { if( --const_cast<RefCounted*>(this)->fRefCount == 0 ) delete this; }
			int		refCount	() const { return fRefCount; }
			bool	isShared	() const { return fRefCount > 1; }

			//-----------------------------------------------------------
			// Debugging support
			static	bool Debug	( const RefCounted* me, Debug::Print& request );
			static	bool Debug	( const RefCounted* me, Debug::Footprint& request );

		protected:
			RefCounted	()						: fRefCount(0) {}
			RefCounted	(const RefCounted&) : fRefCount(0) { }
			RefCounted&	operator=	(const RefCounted&)
									{ fRefCount = 0; return *this; }

			virtual ~RefCounted		();

		private:
			int		fRefCount;	//! Number of references
	};

} // namespace adsk

//=========================================================
//-
// Copyright  2012  Autodesk,  Inc.  All  rights  reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // adskRefCounted_h
