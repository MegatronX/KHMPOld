//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2011 - Julien Fryer - julienfryer@gmail.com				//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#ifndef H_SPK_REFERENCE
#define H_SPK_REFERENCE

#define SPK_NULL_REF SPK::NullReferenceValue()

namespace SPK
{
	// Hack to allow easy null reference initialization
	class NullReferenceValue {};

	/**
	* @brief A strong reference on a SPKObject
	*
	* This class defines a smart pointer with intrusive reference counting.<br>
	* Ref objects are responsible for SPKObjects destruction. An SPKObject is destroyed as soon as no more references are pointing to it.<br>
	* <br>
	* A Ref offers the same operations (* and ->) and comparison operators as a standard pointers.<br>
	* Moreover implicit conversions exists between Ref and standard pointer.<br>
	* Implicit downcasting is also implemented. Upcasting can be performed with a call to cast<T> (equivalent to dynamic_cast<T>)<br>
	* <br>
	* In practice, An SPKObject must always be manipulated through a reference.
	*/
	template<typename T>
	class Ref
	{
	public :

		/////////////////////////////
		// Constructors/Destructor //
		/////////////////////////////

		Ref() : ptr(NULL) {}
		Ref(NullReferenceValue) : 
			ptr(NULL) {}
		Ref(const Ref& ref) : 
			ptr(ref.get()) { increment(); }
		template<typename U> Ref(U* ptr) : 
			ptr(ptr) { increment(); }
		template<typename U> Ref(const Ref<U>& ref) : 
			ptr(ref.get()) { increment(); }
		
		~Ref() { 
			decrement(); }

		//////////////////////////
		// Operator overloading //
		//////////////////////////

		Ref& operator=(NullReferenceValue) 
		{ 
			reset(); 
			return *this; 
		}

		Ref& operator=(T* ptr)
		{
			if (*this != ptr)
			{
				decrement();
				this->ptr = ptr;
				increment();
			}

			return *this;
		}

		Ref& operator=(const Ref& ref)
		{
			if (*this != ref)
			{
				decrement();
				ptr = ref.get();
				increment();
			}

			return *this;
		}

		template<typename U> Ref& operator=(const Ref<U>& ref)
		{
			if (*this != ref)
			{
				decrement();
				ptr = ref.get();
				increment();
			}

			return *this;
		}

		T& operator*() const { return *ptr; }
		T* operator->() const { return ptr; }
		T* get() const { return ptr; }

		operator bool() const { return ptr != 0; }

		void reset() { decrement(); ptr = NULL; }

		template<typename U> Ref<U> cast() const { return Ref<U>(dynamic_cast<U*>(ptr)); }

	private :

		void increment() { if (ptr != NULL) ++(ptr->nbReferences); }

		// HACK : The pointer is cast to SPKObject* to allow the Ref class to access the destructor
		void decrement() { if (ptr != NULL && --(ptr->nbReferences) == 0) SPK_DELETE(ptr); }

		T* ptr;
	};

	template<typename T,typename U> inline bool operator==(const Ref<T>& ref0,const Ref<U>& ref1) { return ref0.get() == ref1.get(); }
	template<typename T,typename U> inline bool operator==(const Ref<T>& ref,U* ptr) { return ref.get() == ptr; }
	template<typename T,typename U> inline bool operator==(T* ptr,const Ref<U>& ref) { return ref.get() == ptr; }
	template<typename T,typename U> inline bool operator<(const Ref<T>& ref0,const Ref<U>& ref1) { return ref0.get() < ref1.get(); }

	template<typename T,typename U> inline bool operator!=(const Ref<T>& ref0,const Ref<U>& ref1) { return ref0.get() != ref1.get(); }
	template<typename T,typename U> inline bool operator!=(const Ref<T>& ref,U* ptr) { return ref.get() != ptr; }
	template<typename T,typename U> inline bool operator!=(T* ptr,const Ref<U>& ref) { return ref.get() != ptr; }
}

#endif

