#pragma once
#define ME_INLINE __forceinline
#include <crtdbg.h>


typedef void (*FreeMethod)();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  The Delegate Class definition.
//  based on: https://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/
// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Delegate
{
private:

	void* _instance;						// instance if class pointer
	void (*_member)(void*);					// member function.

public:
	typedef void* InstancePtr;
	typedef void (*InternalFunction)(InstancePtr);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  turns a free function into our internal function stub
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <void (*Function)()>
	static ME_INLINE void FunctionStub(InstancePtr)
	{
		// we don't need the instance pointer because we're dealing with free functions
		return (Function)();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	// turns a member function into our internal function stub
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class C, void (C::* Function)()>
	static ME_INLINE void ClassMethodStub(InstancePtr instance)
	{
		// cast the instance pointer back into the original class instance
		return (static_cast<C*>(instance)->*Function)();
	}

public:
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Simple constructor.
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Delegate(void) : _instance(nullptr), _member(nullptr)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Binds a free function
	//  usage:  Delegate delegate;   delegate.Bind<&FreeFunction>(); delegate.Invoke();
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <void (*Function)()>
	void Bind(void)
	{
		_instance = nullptr;
		_member = &FunctionStub<Function>;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Binds a class method
	//  usage: Class c; Delegate delegate;   delegate.Bind<Class, &Class::MemberFunction>(&c);  delegate.Invoke();
	// </summary>
	// <param name="instance">  The class instance.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class C, void (C::* Function)()>
	void Bind(C* instance)
	{
		_instance = instance;
		_member   = &ClassMethodStub<C, Function>;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Invoke and execute the delegate.
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Invoke() const
	{
		_ASSERT_EXPR(_member != nullptr, "Cannot invoke unbound delegate. Call Bind() first.");
	    _member(_instance);
	}

};
