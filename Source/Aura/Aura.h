// NP Game Developer

#pragma once

#include "CoreMinimal.h"

#define  CUSTOM_DEPTH_VALUE 250.f

/*
 * Projectile collision channel
 * 
 * Look into Project Settings -> Engine -> Collision -> Object Channels
 */
/**
 * Projectile collision channel
 */
#define  ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

/**
 * Target collision channel
 */
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2

/**
 * Decal collision channel
 */
#define ECC_Decal ECollisionChannel::ECC_GameTraceChannel3

