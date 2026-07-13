#include "AuraAbilityTypes.h"

UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	
	/*
	 * Copy code from super and customized it
	 * Old way doing it
	 */
	/*uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
	}
	
	Ar.SerializeBits(&RepBits, 9);
	
	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get());
	}*/
	
	/*
	 * New way doing it
	 */
	
	// Check if parent NetSerialize successful
	if (Super::NetSerialize(Ar, Map, bOutSuccess))
	{
		// Declare a Replication Bits and use it with bitwise left shift
		// uint32 - unsign 32 bit E.g 00000000000000000000000000000000
		// bitwise left shift example
		//		- RepBits |= 1 << 0; is 00000000000000000000000000000001 = 1
		//		- RepBits |= 1 << 1; is 00000000000000000000000000000010 = 2
		//		- RepBits |= 1 << 2; is 00000000000000000000000000000100 = 4
		uint32 RepBits = 0;
		if (Ar.IsSaving())
		{
			if (bIsBlockedHit)
			{
				RepBits |= 1 << 0;
			}
			if (bIsCriticalHit)
			{
				RepBits |= 1 << 1;
			}
			if (bIsSuccessfulDebuff)
			{
				RepBits |= 1 << 2;
			}
			if (DebuffDamage > 0.f)
			{
				RepBits |= 1 << 3;
			}
			if (DebuffDuration > 0.f)
			{
				RepBits |= 1 << 4;
			}
			if (DebuffFrequency > 0.f)
			{
				RepBits |= 1 << 5;
			}
			if (DamageType.IsValid())
			{
				RepBits |= 1 << 6;
			}
			if (!DeathImpluse.IsZero())
			{
				RepBits |= 1 << 7;
			}
		}
	
		// tell archive how long is the replication bits
		Ar.SerializeBits(&RepBits, 8);
	
		// Use bitwise operator AND to see if the bit is flipped to 1 from 0
		// If it is then tell archive to serialize data
		// Note: the order has to be as same as above
		if (RepBits & (1 << 0))
		{
			Ar << bIsBlockedHit;
		}
		if (RepBits & (1 << 1))
		{
			Ar << bIsCriticalHit;
		}
		if (RepBits & (1 << 2))
		{
			Ar << bIsSuccessfulDebuff;
		}
		if (RepBits & (1 << 3))
		{
			Ar << DebuffDamage;
		}
		if (RepBits & (1 << 4))
		{
			Ar << DebuffDuration;
		}
		if (RepBits & (1 << 5))
		{
			Ar << DebuffFrequency;
		}
		if (RepBits & (1 << 6))
		{
			if (Ar.IsLoading())
			{
				if (!DamageType.IsValid())
				{
					DamageType = MakeShared<FGameplayTag>();
				}
			}
			DamageType->NetSerialize(Ar, Map, bOutSuccess);
		}
		if (RepBits & (1 << 7))
		{
			DeathImpluse.NetSerialize(Ar, Map, bOutSuccess);
		}
	
		bOutSuccess = true;
		return true;
	}
	
	bOutSuccess = false;
	return false;
}

FGameplayEffectContext* FAuraGameplayEffectContext::Duplicate() const
{
	FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		// Does a deep copy of the hit result
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}
