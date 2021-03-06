/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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
#ifndef GAME_SERVER_ENTITIES_CGRENADE_H
#define GAME_SERVER_ENTITIES_CGRENADE_H

// Contact Grenade / Timed grenade / Satchel Charge
class CGrenade : public CBaseMonster
{
public:
	DECLARE_CLASS( CGrenade, CBaseMonster );

	void Spawn( void ) override;

	static CGrenade* ShootTimed( CBaseEntity* pOwner, Vector vecStart, Vector vecVelocity, float time );
	static CGrenade* ShootContact( CBaseEntity* pOwner, Vector vecStart, Vector vecVelocity );

	void Explode( Vector vecSrc, Vector vecAim );
	void Explode( TraceResult *pTrace, int bitsDamageType );
	void EXPORT Smoke( void );

	void EXPORT BounceTouch( CBaseEntity *pOther );
	void EXPORT SlideTouch( CBaseEntity *pOther );
	void EXPORT ExplodeTouch( CBaseEntity *pOther );
	void EXPORT DangerSoundThink( void );
	void EXPORT PreDetonate( void );
	void EXPORT Detonate( void );
	void EXPORT DetonateUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void EXPORT TumbleThink( void );

	virtual void BounceSound( void );
	virtual int	BloodColor() const override { return DONT_BLEED; }
	virtual void Killed( const CTakeDamageInfo& info, GibAction gibAction ) override;

	bool m_fRegisteredSound;// whether or not this grenade has issued its DANGER sound to the world sound list yet.
};

#endif //GAME_SERVER_ENTITIES_CGRENADE_H