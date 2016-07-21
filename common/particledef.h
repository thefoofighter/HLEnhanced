/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#if !defined( PARTICLEDEFH )
#define PARTICLEDEFH
#ifdef _WIN32
#pragma once
#endif

typedef enum {
	pt_static, 
	pt_grav,
	pt_slowgrav,
	pt_fire,
	pt_explode,
	pt_explode2,
	pt_blob,
	pt_blob2,
	pt_vox_slowgrav,
	pt_vox_grav,
	pt_clientcustom   // Must have callback function specified
} ptype_t;

using ParticleCallback = void ( * ) ( struct particle_s* particle, float frametime );
using ParticleDeathCallback = void( * )( struct particle_s* particle );

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
typedef struct particle_s
{
// driver-usable fields
	Vector		org;
	short		color;
	short		packedColor;
// drivers never touch the following fields
	struct particle_s	*next;
	Vector		vel;
	float		ramp;
	float		die;
	ptype_t		type;
	ParticleDeathCallback deathfunc;

	// for pt_clientcusttom, we'll call this function each frame
	ParticleCallback callback;
	
	// For deathfunc, etc.
	unsigned char context;
} particle_t;

#endif
