// NP Game Developer


#include "DataFragment/DataFragmentsContainer.h"

#include "DataFragment/DataFragment.h"

UDataFragment* UDataFragmentsContainer::FindFragmentByClass(const TArray<UDataFragment*>& Fragments, TSubclassOf<UDataFragment> FragmentClass)
{
	if (Fragments.IsEmpty()) return nullptr;
	
	for (UDataFragment* Fragment : Fragments)
	{
		if (Fragment->GetClass() == FragmentClass)
		{
			return Fragment;
		}
	}
	return nullptr;
}

UDataFragment* UDataFragmentsContainer::CreateFragmentByClass(UObject* Outer, TSubclassOf<UDataFragment> FragmentClass)
{
	checkf(
		FragmentClass->IsChildOf(UDataFragment::StaticClass()), 
		TEXT("Only subclass of UDataFragment can be used to create fragment.")
	);
	
	checkf(
		!FragmentClass->HasAnyClassFlags(CLASS_Abstract), 
		TEXT("Can't instantiate fragment object from abstract class %s"), 
		*FragmentClass->GetClass()->GetName()
	);
	
	return NewObject<UDataFragment>(Outer, FragmentClass);
}

UDataFragment* UDataFragmentsContainer::CreateFragment(TSubclassOf<UDataFragment> FragmentClass)
{
	return CreateFragmentByClass(this, FragmentClass);
}

UDataFragment* UDataFragmentsContainer::CreateFragmentAndAdded(TSubclassOf<UDataFragment> FragmentClass, FOnNewFragmentCreatedSignature OnNewFragmentCreatedDelegate)
{
	if (UDataFragment* Fragment = CreateFragmentByClass(this, FragmentClass))
	{
		if (OnNewFragmentCreatedDelegate.ExecuteIfBound(Fragment))
		{
			OnNewFragmentCreatedDelegate.Unbind();
		}
		AddFragment(Fragment);
		return Fragment;
	}
	return nullptr;
}

UDataFragment* UDataFragmentsContainer::FindFragment(TSubclassOf<UDataFragment> FragmentClass) const
{
	if (DataFragments.IsEmpty()) return nullptr;
	return FindFragmentByClass(DataFragments, FragmentClass);
}

void UDataFragmentsContainer::AddFragment(UDataFragment* Fragment)
{
	if (Fragment)
	{
		if (DataFragments.AddUnique(Fragment) > -1)
		{
			OnNewFragmentAddedDelegate.Broadcast(Fragment);
			OnDataFragmentsCountChangedDelegate.Broadcast(DataFragments);
		}
	}
}

void UDataFragmentsContainer::AddFragments(TArray<UDataFragment*> FragmentsToAdd)
{
	if (FragmentsToAdd.IsEmpty()) return;
	for (UDataFragment* Fragment : FragmentsToAdd)
	{
		AddFragment(Fragment);
	}
}

void UDataFragmentsContainer::RemoveFragmentByClass(TSubclassOf<UDataFragment> FragmentClass, bool bFirstOnly)
{
	if (DataFragments.IsEmpty()) return;
	int32 RemovedCount = 0;
	for (auto IT = DataFragments.CreateIterator(); IT; ++IT)
	{
		if (IT->GetClass() == FragmentClass)
		{
			OnRemovingFragmentDelegate.Broadcast(*IT);
			IT.RemoveCurrent();
			RemovedCount += 1;
			if (bFirstOnly) return;
		}
	}
	if (RemovedCount > 0)
	{
		OnDataFragmentsCountChangedDelegate.Broadcast(DataFragments);
	}
}
