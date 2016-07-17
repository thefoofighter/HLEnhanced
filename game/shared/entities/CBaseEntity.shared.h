#ifndef GAME_SHARED_CBASEENTITY_SHARED_H
#define GAME_SHARED_CBASEENTITY_SHARED_H

/*

Class Hierachy

CBaseEntity
	CBaseDelay
		CBaseToggle
			CBaseItem
			CBaseMonster
				CBaseCycler
				CBasePlayer
*/

#include <cstddef>

#include "archtypes.h"     // DAL
#include "SaveRestore.h"
#include "entities/NPCs/Schedule.h"

#include "entities/AnimationEvent.h"

#include "entities/DataMapping.h"

#include "entities/EHandle.h"

#include "Damage.h"

#include "ButtonSounds.h"

#include "CTakeDamageInfo.h"

class CBaseMonster;
class CSquadMonster;

/**
*	Set this bit on guns and stuff that should never respawn.
*/
#define	SF_NORESPAWN	( 1 << 30 )

/**
*	People gib if their health is <= this at the time of death.
*/
#define	GIB_HEALTH_VALUE	-30

// NOTE: tweak these values based on gameplay feedback:

/**
*	Number of 2 second intervals to take damage.
*/
#define PARALYZE_DURATION	2

/**
*	Damage to take each 2 second interval.
*/
#define PARALYZE_DAMAGE		1.0

#define NERVEGAS_DURATION	2
#define NERVEGAS_DAMAGE		5.0

#define POISON_DURATION		5
#define POISON_DAMAGE		2.0

#define RADIATION_DURATION	2
#define RADIATION_DAMAGE	1.0

#define ACID_DURATION		2
#define ACID_DAMAGE			5.0

#define SLOWBURN_DURATION	2
#define SLOWBURN_DAMAGE		1.0

#define SLOWFREEZE_DURATION	2
#define SLOWFREEZE_DAMAGE	1.0

/**
*	Tracers fire every 4 bullets
*/
#define TRACER_FREQ 4

/**
*	Max number of nodes available for a path.
*/
#define MAX_PATH_SIZE 10

//TODO: what's with all of the dllexport definitions? - Solokiller
#define EXPORT DLLEXPORT

/**
*	These are caps bits to indicate what an object's capabilities (currently used for save/restore and level transitions).
*/
enum FCapability
{
	/**
	*	Not used.
	*/
	FCAP_CUSTOMSAVE			= 0x00000001,

	/**
	*	Should transfer between transitions.
	*/
	FCAP_ACROSS_TRANSITION	= 0x00000002,

	/**
	*	Spawn after restore.
	*/
	FCAP_MUST_SPAWN			= 0x00000004,

	/**
	*	Don't save this.
	*/
	FCAP_DONT_SAVE			= 0x80000000,

	/**
	*	Can be used by the player.
	*/
	FCAP_IMPULSE_USE		= 0x00000008,

	/**
	*	Can be used by the player.
	*/
	FCAP_CONTINUOUS_USE		= 0x00000010,

	/**
	*	Can be used by the player.
	*/
	FCAP_ONOFF_USE			= 0x00000020,

	/**
	*	Player sends +/- 1 when using (currently only tracktrains).
	*/
	FCAP_DIRECTIONAL_USE	= 0x00000040,

	/**
	*	Can be used to "master" other entities (like multisource).
	*/
	FCAP_MASTER				= 0x00000080,

	/**
	*	UNDONE: This will ignore transition volumes (trigger_transition), but not the PVS!!!
	*	ALWAYS goes across transitions.
	*/
	FCAP_FORCE_TRANSITION	= 0x00000080,
};

enum USE_TYPE
{
	USE_OFF		= 0,
	USE_ON		= 1,
	USE_SET		= 2,
	USE_TOGGLE	= 3
};

/**
*	@see CBaseEntity::Classify
*/
enum Classification
{
	CLASS_NONE				= 0,
	CLASS_MACHINE			= 1,
	CLASS_PLAYER			= 2,
	CLASS_HUMAN_PASSIVE		= 3,
	CLASS_HUMAN_MILITARY	= 4,
	CLASS_ALIEN_MILITARY	= 5,
	CLASS_ALIEN_PASSIVE		= 6,
	CLASS_ALIEN_MONSTER		= 7,
	CLASS_ALIEN_PREY		= 8,
	CLASS_ALIEN_PREDATOR	= 9,
	CLASS_INSECT			= 10,
	CLASS_PLAYER_ALLY		= 11,

	/**
	*	Hornets and snarks. Launched by players.
	*/
	CLASS_PLAYER_BIOWEAPON	= 12,

	/**
	*	Hornets and snarks. Launched by the alien menace.
	*/
	CLASS_ALIEN_BIOWEAPON	= 13,

	/**
	*	Entities that want to be ignored use this.
	*	Used to be called CLASS_BARNACLE.
	*	Special because no one pays attention to it, and it eats a wide cross-section of creatures.
	*/
	CLASS_IGNORE			= 99,
};

/**
*	Possible values for entvars_t::fixangle
*
*	Solokiller
*/
enum FixAngleMode
{
	/**
	*	Do nothing
	*/
	FIXANGLE_NO			= 0,

	/**
	*	Set view angles to pev->angles
	*/
	FIXANGLE_SET		= 1,

	/**
	*	Add avelocity yaw value to view angles
	*/
	FIXANGLE_ADD_AVEL	= 2
};

/**
*	When calling Killed(), a value that governs gib behavior is expected to be one of these three values.
*/
enum GibAction
{
	/**
	*	Gib if entity was overkilled
	*/
	GIB_NORMAL	= 0,

	/**
	*	Never gib, no matter how much death damage is done ( freezing, etc )
	*/
	GIB_NEVER	= 1,

	/**
	*	Always gib ( Houndeye Shock, Barnacle Bite )
	*/
	GIB_ALWAYS	= 2,
};

typedef void ( CBaseEntity::*BASEPTR )();
typedef void ( CBaseEntity::*ENTITYFUNCPTR )( CBaseEntity *pOther );
typedef void ( CBaseEntity::*USEPTR )( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

extern void FireTargets( const char *targetName, CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

//
// Base Entity.  All entity types derive from this
//
class CBaseEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity );
	DECLARE_DATADESC_NOBASE();

	// Constructor.  Set engine to use C/C++ callback functions
	// pointers to engine data
	entvars_t *pev;		// Don't need to save/restore this pointer, the engine resets it

						// path corners
	CBaseEntity			*m_pGoalEnt;// path corner we are heading towards
	CBaseEntity			*m_pLink;// used for temporary link-list operations. 

	/*
	*	Getters and setters for entvars_t.
	*/
public:
	/**
	*	@return This entity's classname.
	*/
	const char* GetClassname() const { return STRING( pev->classname ); }

	/**
	*	@return Whether this entity's classname matches the given classname.
	*/
	bool ClassnameIs( const char* const pszClassName ) const
	{
		return FClassnameIs( this, pszClassName );
	}

	/**
	*	@copydoc ClassnameIs( const char* const pszClassName ) const
	*/
	bool ClassnameIs( const string_t iszClassName ) const
	{
		return FClassnameIs( this, STRING( iszClassName ) );
	}

	/**
	*	@return Whether this entity has a global name.
	*/
	bool HasGlobalName() const
	{
		return !!( *STRING( pev->globalname ) );
	}

	/**
	*	@return This entity's global name.
	*/
	const char* GetGlobalName() const { return STRING( pev->globalname ); }

	/**
	*	Sets this entity's global name.
	*	@param iszGlobalName Name to set.
	*/
	void SetGlobalName( const string_t iszGlobalName )
	{
		pev->globalname = iszGlobalName;
	}

	/**
	*	Sets this entity's global name.
	*	@param pszGlobalName Name to set.
	*/
	void SetGlobalName( const char* const pszGlobalName )
	{
		SetGlobalName( MAKE_STRING( pszGlobalName ) );
	}

	/**
	*	Clears this entity's global name.
	*/
	void ClearGlobalName()
	{
		pev->globalname = iStringNull;
	}

	/**
	*	@return Whether this entity has a targetname.
	*/
	bool HasTargetname() const
	{
		return !!( *STRING( pev->targetname ) );
	}

	/**
	*	@return This entity's targetname.
	*/
	const char* GetTargetname() const { return STRING( pev->targetname ); }

	/**
	*	Sets this entity's targetname.
	*	@param iszTargetName Name to set.
	*/
	void SetTargetname( const string_t iszTargetName )
	{
		pev->targetname = iszTargetName;
	}

	/**
	*	Sets this entity's targetname.
	*	@param pszTargetName Name to set.
	*/
	void SetTargetname( const char* const pszTargetName )
	{
		SetTargetname( MAKE_STRING( pszTargetName ) );
	}

	/**
	*	Clears this entity's targetname.
	*/
	void ClearTargetname()
	{
		pev->targetname = iStringNull;
	}

	/**
	*	@return Whether this entity has a target.
	*/
	bool HasTarget() const
	{
		return !!( *STRING( pev->target ) );
	}

	/**
	*	@return This entity's target.
	*/
	const char* GetTarget() const { return STRING( pev->target ); }

	/**
	*	Sets this entity's target.
	*	@param iszTarget Target to set.
	*/
	void SetTarget( const string_t iszTarget )
	{
		pev->target = iszTarget;
	}

	/**
	*	Sets this entity's target.
	*	@param pszTarget Name to set.
	*/
	void SetTarget( const char* const pszTarget )
	{
		SetTarget( MAKE_STRING( pszTarget ) );
	}

	/**
	*	Clears this entity's target.
	*/
	void ClearTarget()
	{
		pev->target = iStringNull;
	}

	/**
	*	@return The entity's absolute origin.
	*/
	const Vector& GetAbsOrigin() const { return pev->origin; }

	/**
	*	Sets the entity's absolute origin.
	*	@param vecOrigin Origin to set.
	*/
	void SetAbsOrigin( const Vector& vecOrigin )
	{
		UTIL_SetOrigin( this, vecOrigin );
	}

	/**
	*	@return The entity's old origin.
	*/
	const Vector& GetOldOrigin() const { return pev->oldorigin; }

	/**
	*	@return The entity's old origin, in mutable form.
	*/
	Vector& GetMutableOldOrigin() { return pev->oldorigin; }

	/**
	*	Sets the entity's old origin.
	*	@param vecOrigin Origin to set.
	*/
	void SetOldOrigin( const Vector& vecOrigin )
	{
		pev->oldorigin = vecOrigin;
	}

	/**
	*	@return This entity's absolute velocity.
	*/
	const Vector& GetAbsVelocity() const { return pev->velocity; }

	/**
	*	Sets the entity's absolute velocity.
	*	@param vecVelocity Velocity to set.
	*/
	void SetAbsVelocity( const Vector& vecVelocity )
	{
		pev->velocity = vecVelocity;
	}

	/**
	*	@return This entity's base velocity.
	*/
	const Vector& GetBaseVelocity() const { return pev->basevelocity; }

	/**
	*	Sets the entity's base velocity.
	*	@param vecVelocity Velocity to set.
	*/
	void SetBaseVelocity( const Vector& vecVelocity )
	{
		pev->basevelocity = vecVelocity;
	}

	/**
	*	@return This entity's move direction.
	*/
	const Vector& GetMoveDir() const { return pev->movedir; }

	/**
	*	Sets the entity's move direction.
	*	@param vecMoveDir Move direction to set.
	*/
	void SetMoveDir( const Vector& vecMoveDir )
	{
		pev->movedir = vecMoveDir;
	}

public:
	/**
	*	Called when the entity is first created. - Solokiller
	*/
	virtual void OnCreate() {}

	/**
	*	Called when the entity is destroyed. - Solokiller
	*/
	virtual void  OnDestroy() {}

	/**
	*	Called when an entity is removed at runtime. Gives entities a chance to respond to it. Not called during map change or shutdown.
	*	Call the baseclass version after handling it.
	*	Used to be non-virtual - Solokiller
	*/
	virtual void UpdateOnRemove();

	// initialization functions

	/**
	*	Called once for each keyvalue provided in the bsp file for this entity.
	*	@param pkvd Keyvalue data. Set pkvd->fHandled to true if you handled the key.
	*/
	virtual void KeyValue( KeyValueData* pkvd ) { pkvd->fHandled = false; }

	/**
	*	Called when the entity should precache its resources.
	*	Should call the baseclass implementation first.
	*/
	virtual void Precache() {}

	/**
	*	Called after all keyvalues have been passed in.
	*	Should call Precache.
	*/
	virtual void Spawn() {}

	/**
	*	Called when the server activates. Gives entities a chance to connect with eachother.
	*	Is not called if the entity is created at runtime.
	*	If the entity has the FL_DORMANT set, this will not be called.
	*/
	virtual void Activate() {}

	/**
	*	Called when the entity is being saved to a save game file.
	*	Call the baseclass implementation first, return false on failure.
	*	@param save Save data.
	*	@return true if the entity was successfully saved, false otherwise.
	*/
	virtual bool Save( CSave &save );

	/**
	*	Called when the entity is being restored from a save game file.
	*	Call the baseclass implementation first, return false on failure.
	*	@param restore Restore data.
	*	@return true if the entity was successfully restored, false otherwise.
	*/
	virtual bool Restore( CRestore &restore );

	/**
	*	Object capabilities.
	*	@return A bit vector of FCapability values.
	*	@see FCapability
	*/
	virtual int ObjectCaps() const { return FCAP_ACROSS_TRANSITION; }

	/**
	*	Setup the object->object collision box (pev->mins / pev->maxs is the object->world collision box)
	*/
	virtual void SetObjectCollisionBox();

	/**
	*	Respawns the entity. Entities that can be respawned should override this and return a new instance.
	*/
	virtual CBaseEntity* Respawn() { return nullptr; }

	/**
	*	@return This entity's edict.
	*/
	const edict_t* edict() const { return ENT( pev ); }

	/**
	*	@copydoc edict_t* edict() const
	*/
	edict_t* edict() { return ENT( pev ); }

	/**
	*	@return Offset of this entity. This is the byte offset in the edict array.
	*	DO NOT USE THIS. Use entindex instead.
	*/
	EOFFSET eoffset() const { return OFFSET( pev ); }

	/**
	*	@return The index of this entity.
	*	0 is worldspawn.
	*	[ 1, gpGlobals->maxClients ] are players.
	*	] gpGlobals->maxClients, gpGlobals->maxEntities [ are normal entities.
	*/
	int entindex() const { return ENTINDEX( edict() ); }

	// fundamental callbacks
	BASEPTR			m_pfnThink;
	ENTITYFUNCPTR	m_pfnTouch;
	USEPTR			m_pfnUse;
	ENTITYFUNCPTR	m_pfnBlocked;

	virtual void Think()
	{
		if( m_pfnThink )
			( this->*m_pfnThink )();
	}

	virtual void Touch( CBaseEntity *pOther )
	{
		if( m_pfnTouch )
			( this->*m_pfnTouch )( pOther );
	}

	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
	{
		if( m_pfnUse )
			( this->*m_pfnUse )( pActivator, pCaller, useType, value );
	}

	virtual void Blocked( CBaseEntity *pOther )
	{
		if( m_pfnBlocked )
			( this->*m_pfnBlocked )( pOther );
	}

	// Ugly code to lookup all functions to make sure they are exported when set.
#ifdef _DEBUG
	void FunctionCheck( void *pFunction, char *name )
	{
		if( pFunction && !NAME_FOR_FUNCTION( ( uint32 ) pFunction ) )
			ALERT( at_error, "No EXPORT: %s:%s (%08lx)\n", STRING( pev->classname ), name, ( uint32 ) pFunction );
	}

	BASEPTR	ThinkSet( BASEPTR func, char *name )
	{
		m_pfnThink = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnThink ) ) ) ), name );
		return func;
	}
	ENTITYFUNCPTR TouchSet( ENTITYFUNCPTR func, char *name )
	{
		m_pfnTouch = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnTouch ) ) ) ), name );
		return func;
	}
	USEPTR	UseSet( USEPTR func, char *name )
	{
		m_pfnUse = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnUse ) ) ) ), name );
		return func;
	}
	ENTITYFUNCPTR	BlockedSet( ENTITYFUNCPTR func, char *name )
	{
		m_pfnBlocked = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnBlocked ) ) ) ), name );
		return func;
	}

#endif

	// allow engine to allocate instance data
	void *operator new( size_t stAllocateBlock, entvars_t *pev )
	{
		return ( void* ) ALLOC_PRIVATE( ENT( pev ), stAllocateBlock );
	}

	// don't use this.
#if _MSC_VER >= 1200 // only build this code if MSVC++ 6.0 or higher
	void operator delete( void *pMem, entvars_t *pev )
	{
		pev->flags |= FL_KILLME;
	}
#endif

	/**
	*	Returns the CBaseEntity instance of the given edict.
	*	@param pent Edict whose instance should be returned. If this is null, uses worldspawn.
	*	@return Entity instance, or null if no instance is assigned to it.
	*/
	static CBaseEntity *Instance( edict_t *pent )
	{
		if( !pent )
			pent = ENT( 0 );
		CBaseEntity *pEnt = ( CBaseEntity * ) GET_PRIVATE( pent );
		return pEnt;
	}

	/**
	*	Returns the CBaseEntity instance of the given entvars.
	*	@param pev Entvars whose instance should be returned.
	*	@return Entity instance, or null if no instance is assigned to it.
	*/
	static CBaseEntity *Instance( entvars_t *pev ) { return Instance( ENT( pev ) ); }

	/**
	*	Returns the CBaseEntity instance of the given eoffset.
	*	@param eoffset Entity offset whose instance should be returned.
	*	@return Entity instance, or null if no instance is assigned to it.
	*/
	static CBaseEntity *Instance( EOFFSET eoffset ) { return Instance( ENT( eoffset ) ); }

	/**
	*	Creates an entity by class name.
	*	@param szName Class name of the entity. This string must continue to exist for at least as long as the map itself.
	*	@param vecOrigin Intended entity origin.
	*	@param vecAngles Intended entity angles.
	*	@param pentOwner Optional. The owner of the newly created entity.
	*	@return Newly created entity, or null if the entity could not be created.
	*/
	static CBaseEntity *Create( char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner = nullptr );

	/*
	*	Returns the type of group (i.e, "houndeye", or "human military" so that monsters with different classnames
	*	still realize that they are teammates. (overridden for monsters that form groups)
	*/
	virtual int Classify() { return CLASS_NONE; }

	/**
	*	@return This entity's blood color.
	*	@see BloodColor
	*/
	virtual int BloodColor() const { return DONT_BLEED; }

	/**
	*	Used when handling damage.
	*	This gives the entity a chance to filter damage. This should add the given damage to the global multi-damage instance if damage is not filtered.
	*	This is where blood, ricochets, and other effects should be handled.
	*	@param info Damage info.
	*	@param vecDir Direction of the attack.
	*	@param ptr Traceline that represents the attack.
	*	@see g_MultiDamage
	*/
	virtual void TraceAttack( const CTakeDamageInfo& info, Vector vecDir, TraceResult* ptr );

	/**
	*	Projects blood decals based on the given damage and traceline.
	*	@param info Damage info.
	*	@param vecDir attack direction.
	*	@param ptr Attack traceline.
	*	@see Damage
	*/
	virtual void TraceBleed( const CTakeDamageInfo& info, Vector vecDir, TraceResult* ptr );

	/**
	*	Deals damage to this entity.
	*	@param info Damage info.
	*/
	void TakeDamage( const CTakeDamageInfo& info );

	/**
	*	Deals damage to this entity.
	*	@param pInflictor The entity that is responsible for dealing the damage.
	*	@param pAttacker The entity that owns pInflictor. This may be identical to pInflictor if it directly attacked this entity.
	*	@param flDamage Amount of damage to deal.
	*	@param bitsDamageType Bit vector of damage types.
	*	@return Whether any damage was dealt.
	*/
	void TakeDamage( CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int bitsDamageType );

protected:
	/**
	*	Called when this entity is damaged.
	*	Should not be called directly, call TakeDamage.
	*	@param info Damage info.
	*/
	virtual void OnTakeDamage( const CTakeDamageInfo& info );

public:

	/**
	*	Called when the entity has been killed.
	*	@param info Damage info.
	*	@param gibAction how to handle the gibbing of this entity.
	*	@see GibAction
	*/
	virtual void Killed( const CTakeDamageInfo& info, GibAction gibAction );

	/**
	*	Gives health to this entity. Negative values take health.
	*	Used to be called TakeHealth.
	*	- Solokiller
	*	@param flHealth Amount of health to give. Negative values take health.
	*	@param bitsDamageType Damage types bit vector. @see Damage enum.
	*	@return Actual amount of health that was given/taken.
	*/
	virtual float GiveHealth( float flHealth, int bitsDamageType );

	/**
	*	@param pActivator Activator.
	*	@return Whether this entity would be triggered by the given activator.
	*/
	virtual bool IsTriggered( const CBaseEntity* const pActivator ) const { return true; }

	/**
	*	@return This entity as a CBaseMonster instance, or null if it isn't a monster.
	*/
	virtual CBaseMonster* MyMonsterPointer() { return nullptr; }

	/**
	*	@return This entity as a CSquadMonster instance, or null if it isn't a squad monster.
	*/
	virtual CSquadMonster* MySquadMonsterPointer() { return nullptr; }

	//TODO: entities that use this function should check the classname, so casting to the actual type and using it is better than a costly virtual function hack - Solokiller
	virtual float GetDelay() { return 0; }

	/**
	*	@return Whether this entity is moving.
	*/
	virtual bool IsMoving() const { return pev->velocity != g_vecZero; }

	/**
	*	Called when the entity is restored, and the entity either has a global name or was transitioned over. Resets the entity for the current level.
	*/
	virtual void OverrideReset() {}

	/**
	*	Returns the decal to project onto this entity given the damage types inflicted upon it. If this entity is alpha tested, returns -1.
	*	@param bitsDamageType
	*	@return Decal to use, or -1.
	*/
	virtual int DamageDecal( int bitsDamageType ) const;

	// This is ONLY used by the node graph to test movement through a door
	virtual void SetToggleState( int state ) {}

	/**
	*	Checks if the given entity can control this entity.
	*	@param pTest Entity to check for control.
	*	@return true if this entity can be controlled, false otherwise.
	*/
	virtual bool OnControls( const CBaseEntity* const pTest ) const { return false; }

	/**
	*	@return Whether this entity is alive.
	*/
	virtual bool IsAlive() const { return ( pev->deadflag == DEAD_NO ) && pev->health > 0; }

	/**
	*	@return Whether this is a BSP model.
	*/
	virtual bool IsBSPModel() const { return pev->solid == SOLID_BSP || pev->movetype == MOVETYPE_PUSHSTEP; }

	/**
	*	@return Whether gauss gun beams should reflect off of this entity.
	*/
	virtual bool ReflectGauss() const { return ( IsBSPModel() && !pev->takedamage ); }

	/**
	*	@return Whether this entity has the given target.
	*/
	virtual bool HasTarget( string_t targetname ) const { return FStrEq( STRING( targetname ), STRING( pev->targetname ) ); }

	/**
	*	@return Whether this entity is positioned in the world.
	*/
	virtual bool IsInWorld() const;

	/**
	*	@return Whether this is a player.
	*/
	virtual	bool IsPlayer() const { return false; }

	/**
	*	@return Whether this is a connected client. Fake clients do not qualify.
	*	TODO: this only applies to players and spectators. Move it? - Solokiller
	*/
	virtual bool IsNetClient() const { return false; }

	/**
	*	@return This entity's team name.
	*/
	virtual const char* TeamID() const { return ""; }

	/**
	*	@return The next entity that this entity should target.
	*/
	virtual CBaseEntity *GetNextTarget();

	// common member functions
	/**
	*	Think function. Removes this entity.
	*/
	void EXPORT SUB_Remove();

	/**
	*	Think function. Does nothing. Useful for when you need a think function that doesn't actually do anything.
	*/
	void EXPORT SUB_DoNothing();

	/**
	*	Think function. Sets up the entity to start fading out.
	*/
	void EXPORT SUB_StartFadeOut();

	/**
	*	Think function. Fades the entity out.
	*/
	void EXPORT SUB_FadeOut();

	/**
	*	Think function. Calls this entity's use method with USE_TOGGLE.
	*/
	void EXPORT SUB_CallUseToggle() { this->Use( this, this, USE_TOGGLE, 0 ); }

	/**
	*	Returns whether the entity should toggle, given the use type and current state.
	*	@param useType Use type.
	*	@param currentState The current entity state.
	*	@return Whether the entity should toggle.
	*/
	bool ShouldToggle( USE_TYPE useType, const bool currentState ) const;

	/**
	*	Fires a number of bullets of a given bullet type.
	*	@param cShots Number of shots to fire.
	*	@param vecSrc Bullet origin.
	*	@param vecDirShooting Bullet direction.
	*	@param vecSpread Random bullet spread to apply.
	*	@param flDistance Maximum bullet distance.
	*	@param iBulletType Bullet type to shoot. @see Bullet.
	*	@param iTracerFreq Show a tracer every this many bullets.
	*	@param iDamage Amount of damage to deal. If 0, uses skill cfg settings for the given bullet type.
	*	@param pAttacker Entity responsible for firing the bullets.
	*/
	void FireBullets( const unsigned int cShots,
					  Vector vecSrc, Vector vecDirShooting, Vector vecSpread, 
					  float flDistance, int iBulletType, 
					  int iTracerFreq = 4, int iDamage = 0, CBaseEntity* pAttacker = nullptr );

	/**
	*	Fires a number of bullets of a given bullet type.
	*	@param cShots Number of shots to fire.
	*	@param vecSrc Bullet origin.
	*	@param vecDirShooting Bullet direction.
	*	@param vecSpread Random bullet spread to apply.
	*	@param flDistance Maximum bullet distance.
	*	@param iBulletType Bullet type to shoot. @see Bullet.
	*	@param iTracerFreq Show a tracer every this many bullets.
	*	@param iDamage Amount of damage to deal. If 0, uses skill cfg settings for the given bullet type.
	*	@param pAttacker Entity responsible for firing the bullets.
	*	@param shared_rand Player specific shared random number seed.
	*	@return Bullet spread angle of the last shot for the X and Y axes.
	*/
	Vector FireBulletsPlayer( const unsigned int cShots,
							  Vector vecSrc, Vector vecDirShooting, Vector vecSpread, 
							  float flDistance, int iBulletType, 
							  int iTracerFreq = 4, int iDamage = 0, CBaseEntity* pAttacker = nullptr, int shared_rand = 0 );

	/**
	*	Triggers all of the entities named this->pev->targetname.
	*	@param pActivator Activator to pass.
	*	@param useType Use type to pass.
	*	@param value Value to pass.
	*/
	void SUB_UseTargets( CBaseEntity *pActivator, USE_TYPE useType, float value );

	/**
	*	@return Whether the bounding boxes of this and the given entity intersect.
	*/
	bool Intersects( const CBaseEntity* const pOther ) const;

	/**
	*	Makes this entity dormant. Dormant entities are not solid, don't move, don't think and have the FL_DORMANT flag set.
	*	Activate will not be called on server activation for dormant entities.
	*	@see Activate()
	*/
	void MakeDormant();

	/**
	*	@return Whether this entity is dormant.
	*/
	bool IsDormant() const;

	/**
	*	Made this virtual. Used to be non-virtual and redeclared in CBaseToggle - Solokiller
	*	@return Whether this entity is locked by its master. Only applies to entities that support having a master.
	*/
	virtual bool IsLockedByMaster() const { return false; }

	/**
	*	Made these static. No point in having member functions that don't access this. - Solokiller
	*	@return The given entity as a CBaseMonster, or null if the entity is not a monster.
	*/
	static CBaseMonster* GetMonsterPointer( entvars_t* pevMonster )
	{
		CBaseEntity* pEntity = Instance( pevMonster );
		if( pEntity )
			return pEntity->MyMonsterPointer();
		return nullptr;
	}

	/**
	*	@copydoc GetMonsterPointer( entvars_t* pevMonster )
	*/
	static CBaseMonster* GetMonsterPointer( edict_t* pentMonster )
	{
		CBaseEntity* pEntity = Instance( pentMonster );
		if( pEntity )
			return pEntity->MyMonsterPointer();
		return nullptr;
	}

	// virtual functions used by a few classes

	/**
	*	Monster maker children use this to tell the monster maker that they have died.
	*/
	virtual void DeathNotice( CBaseEntity* pChild ) {}

	/**
	*	Tries to send a monster into PRONE state.
	*	Right now only used when a barnacle snatches someone, so may have some special case stuff for that.
	*	@return Whether the entity has become prone.
	*	TODO: only used by barnacles so rename it.
	*/
	virtual bool FBecomeProne() { return false; }

	/**
	*	@return Center point of entity.
	*/
	virtual Vector Center() const { return ( pev->absmax + pev->absmin ) * 0.5; }

	/**
	*	@return Position of eyes.
	*/
	virtual Vector EyePosition() const { return pev->origin + pev->view_ofs; }

	/**
	*	@return Position of ears.
	*/
	virtual Vector EarPosition() const { return pev->origin + pev->view_ofs; }

	/**
	*	@return Position to shoot at.
	*/
	virtual Vector BodyTarget( const Vector &posSrc ) const { return Center(); }

	/**
	*	@return Entity illumination.
	*/
	virtual int Illumination() const { return GETENTITYILLUM( ENT( pev ) ); }

	/**
	*	@return Whether this entity is visible to the given entity.
	*/
	virtual	bool FVisible( const CBaseEntity *pEntity ) const;

	/**
	*	@return Whether this entity is visible from the given position.
	*/
	virtual	bool FVisible( const Vector &vecOrigin ) const;

	//TODO: find a way to get rid of this stuff. We need to be able to network arbitrary ammo counts - Solokiller
	//We use this variables to store each ammo count.
	int ammo_9mm;
	int ammo_357;
	int ammo_bolts;
	int ammo_buckshot;
	int ammo_rockets;
	int ammo_uranium;
	int ammo_hornets;
	int ammo_argrens;
};

// Ugly technique to override base member functions
// Normally it's illegal to cast a pointer to a member function of a derived class to a pointer to a 
// member function of a base class.  static_cast is a sleezy way around that problem.

#ifdef _DEBUG

#define SetThink( a ) ThinkSet( static_cast<BASEPTR>( a ), #a )
#define SetTouch( a ) TouchSet( static_cast<ENTITYFUNCPTR>( a ), #a )
#define SetUse( a ) UseSet( static_cast<USEPTR>( a ), #a )
#define SetBlocked( a ) BlockedSet( static_cast<ENTITYFUNCPTR>( a ), #a )

#else

#define SetThink( a ) m_pfnThink = static_cast<BASEPTR>( a )
#define SetTouch( a ) m_pfnTouch = static_cast<ENTITYFUNCPTR>( a )
#define SetUse( a ) m_pfnUse = static_cast<USEPTR>( a )
#define SetBlocked( a ) m_pfnBlocked = static_cast<ENTITYFUNCPTR>( a )

#endif

/**
*	Converts a entvars_t * to a class pointer
*	It will allocate the class and entity if necessary
*/
template<typename T>
T* GetClassPtr( T* a )
{
	entvars_t* pev = reinterpret_cast<entvars_t*>( a );

	// allocate entity if necessary
	if( pev == nullptr )
		pev = VARS( CREATE_ENTITY() );

	// get the private data
	a = static_cast<T*>( GET_PRIVATE( ENT( pev ) ) );

	if( a == nullptr )
	{
		// allocate private data 
		a = new( pev ) T;
		a->pev = pev;
		//Now calls OnCreate - Solokiller
		a->OnCreate();
	}

	return a;
}

#endif //GAME_SHARED_CBASEENTITY_SHARED_H