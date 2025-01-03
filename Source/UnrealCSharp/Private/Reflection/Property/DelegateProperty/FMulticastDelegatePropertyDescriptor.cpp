﻿#include "Reflection/Property/DelegateProperty/FMulticastDelegatePropertyDescriptor.h"
#include "Environment/FCSharpEnvironment.h"
#include "Reflection/Delegate/FMulticastDelegateHelper.h"

void FMulticastDelegatePropertyDescriptor::Get(void* Src, void** Dest, std::true_type) const
{
	*Dest = NewWeakRef(Src);
}

void FMulticastDelegatePropertyDescriptor::Get(void* Src, void** Dest, std::false_type) const
{
	*Dest = NewWeakRef(Src);
}

void FMulticastDelegatePropertyDescriptor::Get(void* Src, void* Dest) const
{
	*static_cast<void**>(Dest) = NewRef(Src);
}

void FMulticastDelegatePropertyDescriptor::Set(void* Src, void* Dest) const
{
	const auto SrcGarbageCollectionHandle = *static_cast<FGarbageCollectionHandle*>(Src);

	const auto SrcMulticastDelegateHelper = FCSharpEnvironment::GetEnvironment().GetDelegate<
		FMulticastDelegateHelper>(SrcGarbageCollectionHandle);

	Property->InitializeValue(Dest);

	const auto MulticastScriptDelegate = const_cast<FMulticastScriptDelegate*>(GetMulticastDelegate(Dest));

	FScriptDelegate ScriptDelegate;

	ScriptDelegate.BindUFunction(SrcMulticastDelegateHelper->GetUObject(),
	                             SrcMulticastDelegateHelper->GetFunctionName());

	MulticastScriptDelegate->Add(ScriptDelegate);
}

const FMulticastScriptDelegate* FMulticastDelegatePropertyDescriptor::GetMulticastDelegate(void* InAddress) const
{
	return Property->GetMulticastDelegate(InAddress);
}

MonoObject* FMulticastDelegatePropertyDescriptor::NewRef(void* InAddress) const
{
	auto Object = FCSharpEnvironment::GetEnvironment().GetDelegateObject<FMulticastDelegateHelper>(InAddress);

	if (Object == nullptr)
	{
		const auto MulticastDelegateHelper = new FMulticastDelegateHelper(
			const_cast<FMulticastScriptDelegate*>(GetMulticastDelegate(InAddress)),
			Property->SignatureFunction);

		Object = FCSharpEnvironment::GetEnvironment().GetDomain()->Object_New(Class);

		FCSharpEnvironment::GetEnvironment().AddDelegateReference<FMulticastDelegateHelper, false>(
			InAddress, MulticastDelegateHelper, Object);
	}

	return Object;
}

MonoObject* FMulticastDelegatePropertyDescriptor::NewWeakRef(void* InAddress) const
{
	const auto MulticastDelegateHelper = new FMulticastDelegateHelper(
		const_cast<FMulticastScriptDelegate*>(GetMulticastDelegate(InAddress)),
		Property->SignatureFunction);

	const auto Object = FCSharpEnvironment::GetEnvironment().GetDomain()->Object_New(Class);

	FCSharpEnvironment::GetEnvironment().AddDelegateReference<FMulticastDelegateHelper, false>(
		InAddress, MulticastDelegateHelper, Object);

	return Object;
}
