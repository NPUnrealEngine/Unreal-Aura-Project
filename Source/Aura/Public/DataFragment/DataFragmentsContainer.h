// NP Game Developer

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataFragmentsContainer.generated.h"

class UDataFragment;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnNewFragmentCreatedSignature, UDataFragment*, Fragment);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewFragmentAddedSignature, UDataFragment*, NewFragment);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemovingFragmentSignature, UDataFragment*, RemovedFragment);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataFragmentsCountChangedSignature, const TArray<UDataFragment*>&, NewFragment);
/**
 * Contain an array of UDataFragment objects which can represent
 * unique data and have its own functionality
 * 
 * Provide basic functionalities for find, add, remove and crate
 */
UCLASS(BlueprintType, EditInlineNew)
class AURA_API UDataFragmentsContainer : public UObject
{
	GENERATED_BODY()

public: // Delegate
	/**
	 * Delegate for new fragment has been added to list
	 */
	UPROPERTY(BlueprintAssignable)
	FOnNewFragmentAddedSignature OnNewFragmentAddedDelegate;

	/**
	 * Delegate for a fragment is about to be removed from list
	 * but not yet removed
	 */
	UPROPERTY(BlueprintAssignable)
	FOnRemovingFragmentSignature OnRemovingFragmentDelegate;

	/**
	 * Delegate for the list of fragment count had changed
	 */
	UPROPERTY(BlueprintAssignable)
	FOnDataFragmentsCountChangedSignature OnDataFragmentsCountChangedDelegate;
	
public: // Helper
	/**
	 * Helper function to find specific fragment from list of fragments
	 * 
	 * @param Fragments List of fragments
	 * @param FragmentClass Specific class of fragment to find
	 * @return fragment or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType = FragmentClass))
	static UDataFragment* FindFragmentByClass(const TArray<UDataFragment*>& Fragments, TSubclassOf<UDataFragment> FragmentClass);

	/**
	 * Helper function to create a new fragment
	 * @param Outer 
	 * @param FragmentClass 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType = FragmentClass))
	static UDataFragment* CreateFragmentByClass(UObject* Outer, TSubclassOf<UDataFragment> FragmentClass);
	
public:
	/**
	 * Create a new fragment
	 * 
	 * NOTE: This will not add the new fragment to list
	 * @param FragmentClass 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType = FragmentClass))
	virtual UDataFragment* CreateFragment(TSubclassOf<UDataFragment> FragmentClass);

	/**
	 * Create a new fragment and added it to the data fragment list
	 * @param FragmentClass 
	 * @param OnNewFragmentCreatedDelegate delegate when fragment is created, use this delegate to
	 * update fragment properties after it is created
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType = FragmentClass))
	virtual UDataFragment* CreateFragmentAndAdded(TSubclassOf<UDataFragment> FragmentClass, FOnNewFragmentCreatedSignature OnNewFragmentCreatedDelegate);
	
	/**
	 * Find specific fragment by class
	 * @param FragmentClass class of fragment to find
	 * @return fragment or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType = FragmentClass))
	virtual UDataFragment* FindFragment(TSubclassOf<UDataFragment> FragmentClass) const;
	
	/**
	 * Add a fragment to data fragment list
	 * @param Fragment fragment to add
	 */
	UFUNCTION(BlueprintCallable)
	virtual void AddFragment(UDataFragment* Fragment);

	/**
	 * Add an array of fragments
	 * @param FragmentsToAdd list of fragments
	 */
	UFUNCTION(BlueprintCallable)
	virtual void AddFragments(TArray<UDataFragment*> FragmentsToAdd);

	/**
	 * Remove a fragment / fragments by class
	 * @param FragmentClass class of fragment
	 * @param bFirstOnly if true only remove first fragment  when found
	 * otherwise remove all found fragments, default to false
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RemoveFragmentByClass(TSubclassOf<UDataFragment> FragmentClass, bool bFirstOnly = false);
	
public:
	/**
	 * Get data fragments
	 * @return collection of fragments
	 */
	FORCEINLINE TArray<TObjectPtr<UDataFragment>>& GetDataFragments() { return DataFragments; };
	
protected:
	/**
	 * Array of data fragment which is subclass of UDataFragment
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UDataFragment>> DataFragments;
};
