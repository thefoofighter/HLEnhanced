#ifndef __APIPROXY__
#define __APIPROXY__

#include "archtypes.h"     // DAL
#include "netadr.h"
#include "Sequence.h"

#ifndef _WIN32
#include "enums.h"
#endif

typedef struct cl_enginefuncs_s cl_enginefuncs_t;
typedef struct kbutton_s kbutton_t;
typedef struct cl_entity_s cl_entity_t;
typedef struct client_data_s client_data_t;
//Not a typo, both of these exist. - Solokiller
typedef struct clientdata_s clientdata_t;
typedef struct playermove_s playermove_t;
typedef struct usercmd_s usercmd_t;
typedef struct ref_params_s ref_params_t;
typedef struct mstudioevent_s mstudioevent_t;
typedef struct local_state_s local_state_t;
typedef struct entity_state_s entity_state_t;
typedef struct weapon_data_s weapon_data_t;
typedef struct netadr_s netadr_t;
typedef struct tempent_s TEMPENTITY;
typedef struct r_studio_interface_s r_studio_interface_t;
typedef struct engine_studio_api_s engine_studio_api_t;
typedef struct client_sprite_s client_sprite_t;
typedef struct cvar_s cvar_t;
typedef struct hud_player_info_s hud_player_info_t;
typedef struct client_textmessage_s client_textmessage_t;
typedef struct SCREENINFO_s SCREENINFO;
typedef struct rect_s wrect_t;
typedef struct con_nprint_s con_nprint_t;
typedef struct edict_s edict_t;
typedef struct event_args_s event_args_t;
typedef struct screenfade_s screenfade_t;
typedef struct model_s model_t;
typedef struct tagPOINT POINT;
typedef struct pmtrace_s pmtrace_t;

typedef struct triangleapi_s triangleapi_t;
typedef struct efx_api_s efx_api_t;
typedef struct event_api_s event_api_t;
typedef struct demo_api_s demo_api_t;
typedef struct net_api_s net_api_t;
typedef struct IVoiceTweak_s IVoiceTweak_t;

typedef triangleapi_t*		pTriAPI;
typedef efx_api_t*			pEfxAPI;
typedef event_api_t*		pEventAPI;
typedef demo_api_t*			pDemoAPI;
typedef net_api_t*			pNetAPI;
typedef IVoiceTweak_t*		pVoiceTweak;

#define	MAX_ALIAS_NAME	32

typedef struct cmdalias_s
{
	struct cmdalias_s	*next;
	char	name[MAX_ALIAS_NAME];
	char	*value;
} cmdalias_t;

//Now defined here. Who ever thought defining this somewhere else was a good idea? - Solokiller
using pfnUserMsgHook = int ( * )( const char *pszName, int iSize, void *pbuf );

using Callback_AddVisibleEntity = int ( * )( cl_entity_t* pEntity );
using Callback_TempEntPlaySound = void ( * )( TEMPENTITY* pTemp, float damp );
using pfnEventHook = void ( * )( event_args_t* args );

/**
*	Size of buffers used to store unique player IDs.
*/
const size_t PLAYERID_BUFFER_SIZE = 16;

// ********************************************************
// Functions exported by the client .dll
// ********************************************************

// Pointers to the exported client functions themselves
typedef struct
{
	/**
	*	Called to initialize the client library.
	*	@param pEngineFuncs Pointer to the engine functions interface;
	*	@param iVersion Interface version. Must match CLDLL_INTERFACE_VERSION.
	*	@return true on success, false otherwise. If iVersion does not match CLDLL_INTERFACE_VERSION, return false.
	*	@see CLDLL_INTERFACE_VERSION
	*/
	int				( *pInitFunc )					( cl_enginefuncs_t* pEngineFuncs, int iVersion );

	/**
	*	Called to initialize the client library. This occurs after the engine has loaded the client and has initialized all other systems.
	*/
	void			( *pHudInitFunc )				( void );

	/**
	*	Called after a connection to a server has been established.
	*/
	int				( *pHudVidInitFunc )			( void );

	/**
	*	Called to redraw the HUD.
	*	@param flCurrentTime Current game time.
	*	@param bIsIntermission Whether we're currently in intermission or not.
	*	@return Ignored. true on success, false otherwise.
	*/
	int				( *pHudRedrawFunc )				( const float flCurrentTime, const int bIsIntermission );

	/**
	*	Called every frame while running a map.
	*	@param pCLData Client data.
	*	@param flCurrentTime Current game time.
	*	@return true if client data was updated, false otherwise.
	*/
	int				( *pHudUpdateClientDataFunc )	( client_data_t* pCLData, const float flCurrentTime );

	/**
	*	Obsolete. Is never called.
	*/
	void			( *pHudResetFunc )				( void );

	/**
	*	Run client side player movement and physics code.
	*	@param ppmove Player movement data.
	*	@param server Whether this is the server or client running movement.
	*/
	void			( *pClientMove )				( playermove_t* ppmove, const qboolean server );

	/**
	*	Initializes the client side player movement code.
	*	@param ppmove Player movement data.
	*/
	void			( *pClientMoveInit )			( playermove_t* ppmove );

	/**
	*	Gets the texture type for a given texture name.
	*	Never called by the engine itself.
	*	@param pszName Texture name.
	*	@return Texture type.
	*/
	char			( *pClientTextureType )			( const char*const pszName );

	/**
	*	Called when mouse input is activated.
	*/
	void			( *pIN_ActivateMouse )			( void );

	/**
	*	Called when mouse input is deactivated.
	*/
	void			( *pIN_DeactivateMouse )		( void );

	/**
	*	Called when a mouse event has occurred.
	*	@param mstate Bit vector containing new mouse button states.
	*/
	void			( *pIN_MouseEvent )				( const int mstate );

	/**
	*	Clears all mouse button states.
	*/
	void			( *pIN_ClearStates )			( void );

	/**
	*	Called to accumulate relative mouse movement.
	*/
	void			( *pIN_Accumulate )				( void );

	/**
	*	Creates a movement command to send to the server.
	*	@param frametime Delta time between the last and current frame.
	*	@param cmd Command to fill in.
	*	@param bActive if bActive == 1 then we are 1) not playing back demos ( where our commands are ignored ) and
	*					2 ) we have finished signing on to server
	*/
	void			( *pCL_CreateMove )				( float frametime, usercmd_t* cmd, const int bActive );

	/**
	*	@return Whether the client is currently in third person mode.
	*/
	int				( *pCL_IsThirdPerson )			( void );

	/**
	*	Gets the camera offset.
	*	@param[ out ] ofs Offset.
	*/
	void			( *pCL_GetCameraOffsets )		( Vector& ofs );

	/**
	*	Finds a key by name.
	*	@param pszName Key name.
	*	@return Key, or null if it couldn't be found.
	*/
	kbutton_t*		( *pFindKey )					( const char* const pszName );

	/**
	*	Runs camera think.
	*/
	void			( *pCamThink )					( void );

	/**
	*	Calculates view data.
	*/
	void			( *pCalcRefdef )				( ref_params_t* pparams );

	/**
	*	Called when the engine has created a client side copy of an entity.
	*	@param type Entity type. @see EntityType
	*	@param ent Entity.
	*	@param pszModelName Name of the model that the entity is using. Same as ent->model->name. Is an empty string if it has no model.
	*	@return true to add it to the list of visible entities, false to filter it out.
	*/
	int				( *pAddEntity )					( int type, cl_entity_t* ent, const char* const pszModelName );

	/**
	*	Gives us a chance to add additional entities to the render this frame.
	*/
	void			( *pCreateEntities )			( void );

	/**
	*	Lets the client draw non-transparent geometry.
	*/
	void			( *pDrawNormalTriangles )		( void );

	/**
	*	Lets the client draw transparent geometry.
	*/
	void			( *pDrawTransparentTriangles )	( void );

	/**
	*	A studiomodel event has occured while advancing an entity's frame.
	*	@param event Event.
	*	@param entity Entity whose frame is being advanced.
	*/
	void			( *pStudioEvent )				( const mstudioevent_t* event, const cl_entity_t* entity );

	/**
	*	Client calls this during prediction, after it has moved the player and updated any info changed into to->
	*	time is the current client clock based on prediction
	*	cmd is the command that caused the movement, etc
	*	bRunFuncs is true if this is the first time we've predicted this command.  If so, sounds and effects should play, otherwise, they should
	*	be ignored
	*	@param from Old state.
	*	@param to New state.
	*	@param cmd Command that was ran.
	*	@param bRunFuncs Whether to play sounds and effects.
	*	@param time Current time.
	*	@param random_seed Shared random seed.
	*/
	void			( *pPostRunCmd )				( local_state_t* from, local_state_t* to, usercmd_t* cmd, const int bRunFuncs, double time, unsigned int random_seed );

	/**
	*	Called when the client shuts down. The library is freed after this call.
	*/
	void			( *pShutdown )					( void );

	/**
	*	The server sends us our origin with extra precision as part of the clientdata structure, not during the normal
	*	playerstate update in entity_state_t.  In order for these overrides to eventually get to the appropriate playerstate
	*	structure, we need to copy them into the state structure at this point.
	*	@param state Player entity state.
	*	@param client Player client state.
	*/
	void			( *pTxferLocalOverrides )		( entity_state_t* state, const clientdata_t* client );

	/**
	*	We have received entity_state_t for this player over the network.  We need to copy appropriate fields to the
	*	playerstate structure.
	*	@param dst Destination state.
	*	@param src Source state.
	*/
	void			( *pProcessPlayerState )		( entity_state_t* dst, const entity_state_t* src );

	/**
	*	Because we can predict an arbitrary number of frames before the server responds with an update, we need to be able to copy client side prediction data in
	*	from the state that the server ack'd receiving, which can be anywhere along the predicted frame path ( i.e., we could predict 20 frames into the future and the server ack's
	*	up through 10 of those frames, so we need to copy persistent client-side only state from the 10th predicted frame to the slot the server
	*	update is occupying. )
	*	@param ps Current player entity state.
	*	@param pps Current predicted player entity state.
	*	@param pcd Current client state.
	*	@param ppcd Current predicted player entity state.
	*	@param wd Current weapon data list.
	*	@param pwd Current predicted weapon data list.
	*/
	void			( *pTxferPredictionData )		( entity_state_t* ps, const entity_state_t* pps, clientdata_t* pcd, const clientdata_t* ppcd, weapon_data_t* wd, const weapon_data_t* pwd );

	/**
	*	Called by the engine while playing back a demo. The engine wants us to parse some data from the demo stream.
	*	@param size Buffer size, in bytes.
	*	@param buffer Buffer.
	*/
	void			( *pReadDemoBuffer )			( int size, unsigned char *buffer );

	/**
	*	Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
	*	size of the response_buffer, so you must zero it out if you choose not to respond.
	*	@param net_from Address of the sender.
	*	@param args Command arguments.
	*	@param response_buffer Buffer to write responses to.
	*	@param response_buffer_size Buffer size.
	*	@return true if the packet is valid, false otherwise.
	*/
	int				( *pConnectionlessPacket )		( const netadr_t* net_from, const char *args, char *response_buffer, int *response_buffer_size );

	/**
	*	Gets hull bounds for physics.
	*	@param hullnumber Hull number. @see Hull::Hull
	*	@param[ out ] mins Minimum bounds.
	*	@param[ out ] maxs Maximum bounds.
	*	@return true if the bounds were set, false otherwise.
	*/
	int				( *pGetHullBounds )				( int hullnumber, Vector& mins, Vector& maxs );

	/**
	*	Called every frame that the client library is loaded.
	*	@param flFrameTime Time between the last and current frame.
	*/
	void			( *pHudFrame )					( const double flFrameTime );

	/**
	*	Called when a key has changed state.
	*	@param bDown Whether they key is down or not.
	*	@param keynum Key number. @see KeyNum
	*	@param pszCurrentBinding Command bound to this key.
	*	@return true to allow engine to process the key, otherwise, act on it as needed.
	*/
	int				( *pKeyEvent )					( const int bDown, int keynum, const char* pszCurrentBinding );

	/**
	*	Simulation and cleanup of temporary entities.
	*	@param flFrameTime Time between the last and current frame.
	*	@param flClientTime Current client time.
	*	@param flCLGravity Client side gravity.
	*	@param ppTempEntFree List of free temporary entities.
	*	@param ppTempEntActive List of active temporary entities.
	*	@param pAddVisibleEnt Callback to add visible entities.
	*	@param pTmpPlaySound Callback to play sounds for temporary entities.
	*/
	void			( *pTempEntUpdate )				( const double flFrameTime, const double flClientTime, const double flCLGravity, 
													  TEMPENTITY** ppTempEntFree, TEMPENTITY** ppTempEntActive,
													  Callback_AddVisibleEntity pAddVisibleEnt, Callback_TempEntPlaySound pTempPlaySound );

	/**
	*	If you specify negative numbers for beam start and end point entities, then
	*	the engine will call back into this function requesting a pointer to a cl_entity_t 
	*	object that describes the entity to attach the beam onto.
	*	
	*	Indices must start at 1, not zero.
	*	@param index Entity index.
	*	@return Entity.
	*/
	cl_entity_t*	( *pGetUserEntity )				( int index );

	/**
	*	Called when a player starts or stops talking.
	*	Possibly null on old client dlls.
	*	@param entindex Player index. 1 based. Is -1 when it's the local player, -2 if the server has acknowledged the local player is talking.
	*	@param bTalking Whether the player is currently talking or not.
	*/
	void			( *pVoiceStatus )				( int entindex, qboolean bTalking );

	/**
	*	Called when a director event message was received.
	*	Should be parsed like a user message.
	*	Possibly null on old client dlls.
	*	@param iSize Size of the buffer.
	*	@param pbuf Buffer.
	*/
	void			( *pDirectorMessage )			( int iSize, void *pbuf );

	/**
	*	Export this function for the engine to use the studio renderer class to render objects.
	*	Not used by all clients.
	*	@param version Interface version. Must be STUDIO_INTERFACE_VERSION. Return false if it doesn't match.
	*	@param[ out ] ppinterface Pointer to a pointer that should contain the studio interface.
	*	@param pstudio Engine studio interface.
	*	@return true if the requested interface was available, false otherwise.
	*	@see STUDIO_INTERFACE_VERSION
	*/
	int				( *pStudioInterface )			( int version, r_studio_interface_t** ppinterface, engine_studio_api_t* pstudio );

	/**
	*	Gets the chat input position.
	*	Not used by all clients.
	*	@param x X position.
	*	@param y Y position.
	*/
	void			( *pChatInputPosition )			( int *x, int *y );

	/**
	*	Doesn't appear to be called.
	*	Not used by all clients.
	*/
	int				( *pGetPlayerTeam )				( int iplayer );

	/**
	*	This should be CreateInterfaceFn but that means including interface.h
	*	which is a C++ file and some of the client files a C only... 
	*	so we return a void * which we then do a typecast on later.
	*
	*	Never actually called, but must be provided in order for CreateInterface to be called.
	*/
	void*			( *pClientFactory )				( void );
} cldll_func_t;

/**
*	@defgroup ClientExports Client Exports
*
*	Function type declarations for client exports
*	Now declares pointers in the interface and decltypes it into the pointer type, making it easier to see parameters in IntelliSense.
*
*	@{
*/

using INITIALIZE_FUNC					= decltype( cldll_func_t::pInitFunc );
using HUD_INIT_FUNC						= decltype( cldll_func_t::pHudInitFunc );
using HUD_VIDINIT_FUNC					= decltype( cldll_func_t::pHudVidInitFunc );
using HUD_REDRAW_FUNC					= decltype( cldll_func_t::pHudRedrawFunc );
using HUD_UPDATECLIENTDATA_FUNC			= decltype( cldll_func_t::pHudUpdateClientDataFunc );
using HUD_RESET_FUNC					= decltype( cldll_func_t::pHudResetFunc );
using HUD_CLIENTMOVE_FUNC				= decltype( cldll_func_t::pClientMove );
using HUD_CLIENTMOVEINIT_FUNC			= decltype( cldll_func_t::pClientMoveInit );
using HUD_TEXTURETYPE_FUNC				= decltype( cldll_func_t::pClientTextureType );
using HUD_IN_ACTIVATEMOUSE_FUNC			= decltype( cldll_func_t::pIN_ActivateMouse );
using HUD_IN_DEACTIVATEMOUSE_FUNC		= decltype( cldll_func_t::pIN_DeactivateMouse );
using HUD_IN_MOUSEEVENT_FUNC			= decltype( cldll_func_t::pIN_MouseEvent );
using HUD_IN_CLEARSTATES_FUNC			= decltype( cldll_func_t::pIN_ClearStates );
using HUD_IN_ACCUMULATE_FUNC			= decltype( cldll_func_t::pIN_Accumulate );
using HUD_CL_CREATEMOVE_FUNC			= decltype( cldll_func_t::pCL_CreateMove );
using HUD_CL_ISTHIRDPERSON_FUNC			= decltype( cldll_func_t::pCL_IsThirdPerson );
using HUD_CL_GETCAMERAOFFSETS_FUNC		= decltype( cldll_func_t::pCL_GetCameraOffsets );
using HUD_KB_FIND_FUNC					= decltype( cldll_func_t::pFindKey );
using HUD_CAMTHINK_FUNC					= decltype( cldll_func_t::pCamThink );
using HUD_CALCREF_FUNC					= decltype( cldll_func_t::pCalcRefdef );
using HUD_ADDENTITY_FUNC				= decltype( cldll_func_t::pAddEntity );
using HUD_CREATEENTITIES_FUNC			= decltype( cldll_func_t::pCreateEntities );
using HUD_DRAWNORMALTRIS_FUNC			= decltype( cldll_func_t::pDrawNormalTriangles );
using HUD_DRAWTRANSTRIS_FUNC			= decltype( cldll_func_t::pDrawTransparentTriangles );
using HUD_STUDIOEVENT_FUNC				= decltype( cldll_func_t::pStudioEvent );
using HUD_POSTRUNCMD_FUNC				= decltype( cldll_func_t::pPostRunCmd );
using HUD_SHUTDOWN_FUNC					= decltype( cldll_func_t::pShutdown );
using HUD_TXFERLOCALOVERRIDES_FUNC		= decltype( cldll_func_t::pTxferLocalOverrides );
using HUD_PROCESSPLAYERSTATE_FUNC		= decltype( cldll_func_t::pProcessPlayerState );
using HUD_TXFERPREDICTIONDATA_FUNC		= decltype( cldll_func_t::pTxferPredictionData );
using HUD_DEMOREAD_FUNC					= decltype( cldll_func_t::pReadDemoBuffer );
using HUD_CONNECTIONLESS_FUNC			= decltype( cldll_func_t::pConnectionlessPacket );
using HUD_GETHULLBOUNDS_FUNC			= decltype( cldll_func_t::pGetHullBounds );
using HUD_FRAME_FUNC					= decltype( cldll_func_t::pHudFrame );
using HUD_KEY_EVENT_FUNC				= decltype( cldll_func_t::pKeyEvent );
using HUD_TEMPENTUPDATE_FUNC			= decltype( cldll_func_t::pTempEntUpdate );
using HUD_GETUSERENTITY_FUNC			= decltype( cldll_func_t::pGetUserEntity );
using HUD_VOICESTATUS_FUNC				= decltype( cldll_func_t::pVoiceStatus );
using HUD_DIRECTORMESSAGE_FUNC			= decltype( cldll_func_t::pDirectorMessage );
using HUD_STUDIO_INTERFACE_FUNC			= decltype( cldll_func_t::pStudioInterface );
using HUD_CHATINPUTPOSITION_FUNC		= decltype( cldll_func_t::pChatInputPosition );
using HUD_GETPLAYERTEAM					= decltype( cldll_func_t::pGetPlayerTeam );
using CLIENTFACTORY						= decltype( cldll_func_t::pClientFactory );

/** @} */

// ********************************************************
// Functions exported by the engine
// ********************************************************

// Pointers to the exported engine functions themselves
typedef struct cl_enginefuncs_s
{
	HSPRITE					( *pfnSPR_Load )							( const char *szPicName );
	int						( *pfnSPR_Frames )							( HSPRITE hPic );
	int						( *pfnSPR_Height )							( HSPRITE hPic, int frame );
	int						( *pfnSPR_Width )							( HSPRITE hPic, int frame );
	void					( *pfnSPR_Set )								( HSPRITE hPic, int r, int g, int b );
	void					( *pfnSPR_Draw )							( int frame, int x, int y, const wrect_t* prc );
	void					( *pfnSPR_DrawHoles )						( int frame, int x, int y, const wrect_t* prc );
	void					( *pfnSPR_DrawAdditive )					( int frame, int x, int y, const wrect_t* prc );
	void					( *pfnSPR_EnableScissor )					( int x, int y, int width, int height );
	void					( *pfnSPR_DisableScissor )					( void );
	client_sprite_t*		( *pfnSPR_GetList )							( char *psz, int *piCount );
	void					( *pfnFillRGBA )							( int x, int y, int width, int height, int r, int g, int b, int a );
	int						( *pfnGetScreenInfo ) 						( SCREENINFO* pscrinfo );
	void					( *pfnSetCrosshair )						( HSPRITE hspr, wrect_t rc, int r, int g, int b );
	cvar_t*					( *pfnRegisterVariable )					( char *szName, char *szValue, int flags );
	float					( *pfnGetCvarFloat )						( const char* pszName );
	const char*				( *pfnGetCvarString )						( const char* pszName );
	int						( *pfnAddCommand )							( char *cmd_name, void( *pfnEngSrc_function )( void ) );
	int						( *pfnHookUserMsg )							( char *szMsgName, pfnUserMsgHook pfn );
	int						( *pfnServerCmd )							( char *szCmdString );
	int						( *pfnClientCmd )							( char *szCmdString );
	void					( *pfnGetPlayerInfo )						( int ent_num, hud_player_info_t* pinfo );
	void					( *pfnPlaySoundByName )						( char *szSound, float volume );
	void					( *pfnPlaySoundByIndex )					( int iSound, float volume );
	void					( *pfnAngleVectors )						( const float * vecAngles, float * forward, float * right, float * up );
	client_textmessage_t*	( *pfnTextMessageGet )						( const char *pName );
	int						( *pfnDrawCharacter )						( int x, int y, int number, int r, int g, int b );
	int						( *pfnDrawConsoleString )					( int x, int y, char *string );
	void					( *pfnDrawSetTextColor )					( float r, float g, float b );
	void					( *pfnDrawConsoleStringLen )				( const char *string, int *length, int *height );
	void					( *pfnConsolePrint )						( const char *string );
	void					( *pfnCenterPrint )							( const char *string );
	int						( *GetWindowCenterX )						( void );
	int						( *GetWindowCenterY )						( void );
	void					( *GetViewAngles )							( float * );
	void					( *SetViewAngles )							( float * );
	int						( *GetMaxClients )							( void );
	void					( *Cvar_SetValue )							( char *cvar, float value );
	int						( *Cmd_Argc )								( void );
	char*					( *Cmd_Argv )								( int arg );
	void					( *Con_Printf )								( char *fmt, ... );
	void					( *Con_DPrintf )							( char *fmt, ... );
	void					( *Con_NPrintf )							( int pos, char *fmt, ... );
	void					( *Con_NXPrintf )							( con_nprint_t* info, char *fmt, ... );
	const char*				( *PhysInfo_ValueForKey )					( const char *key );
	const char*				( *ServerInfo_ValueForKey )					( const char *key );
	float					( *GetClientMaxspeed )						( void );
	int						( *CheckParm )								( char *parm, char **ppnext );
	void					( *Key_Event )								( int key, int down );
	void					( *GetMousePosition )						( int *mx, int *my );
	int						( *IsNoClipping )							( void );
	cl_entity_t*			( *GetLocalPlayer )							( void );
	cl_entity_t*			( *GetViewModel )							( void );
	cl_entity_t*			( *GetEntityByIndex )						( int idx );
	float					( *GetClientTime )							( void );
	void					( *V_CalcShake )							( void );
	void					( *V_ApplyShake )							( float *origin, float *angles, float factor );
	int						( *PM_PointContents )						( const float* point, int *truecontents );

	/**
	*	Gets the index of the water entity at the given position.
	*	@param vecPosition Position to look for the entity.
	*	@return Entity index. -1 if no water entity was found.
	*/
	int						( *PM_WaterEntity )							( float *p );
	pmtrace_t*				( *PM_TraceLine )							( float *start, float *end, int flags, int usehull, int ignore_pe );
	model_t*				( *CL_LoadModel )							( const char *modelname, int *index );
	int						( *CL_CreateVisibleEntity )					( int type, cl_entity_t* ent );
	const model_t*			( *GetSpritePointer )						( HSPRITE hSprite );
	void					( *pfnPlaySoundByNameAtLocation )			( char *szSound, float volume, float *origin );
	unsigned short			( *pfnPrecacheEvent )						( int type, const char* psz );
	void					( *pfnPlaybackEvent )						( int flags, const edict_t* pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	void					( *pfnWeaponAnim )							( int iAnim, int body );
	float					( *pfnRandomFloat )							( float flLow, float flHigh );
	int32					( *pfnRandomLong )							( int32 lLow, int32 lHigh );
	void					( *pfnHookEvent )							( char *name, pfnEventHook pEventHook );
	int						( *Con_IsVisible )							( void );
	const char*				( *pfnGetGameDirectory )					( void );
	cvar_t*					( *pfnGetCvarPointer )						( const char *szName );
	const char*				( *Key_LookupBinding )						( const char *pBinding );
	const char*				( *pfnGetLevelName )						( void );
	void					( *pfnGetScreenFade )						( screenfade_t* fade );
	void					( *pfnSetScreenFade )						( screenfade_t* fade );
	void*					( *VGui_GetPanel )							( void );
	void					( *VGui_ViewportPaintBackground )			( int extents[ 4 ] );
	byte*					( *COM_LoadFile )							( char *path, int usehunk, int *pLength );
	char*					( *COM_ParseFile )							( char *data, char *token );
	void					( *COM_FreeFile )							( void *buffer );

	triangleapi_t*		pTriAPI;
	efx_api_t*			pEfxAPI;
	event_api_t*		pEventAPI;
	demo_api_t*			pDemoAPI;
	net_api_t*			pNetAPI;
	IVoiceTweak_t*		pVoiceTweak;

	int						 ( *IsSpectateOnly )						( void );
	model_t*				 ( *LoadMapSprite )							( const char *filename );
	void					 ( *COM_AddAppDirectoryToSearchPath )		( const char *pszBaseDir, const char *appName );
	int						 ( *COM_ExpandFilename )					( const char *fileName, char *nameOutBuffer, int nameOutBufferSize );
	const char*				 ( *PlayerInfo_ValueForKey )				( int playerNum, const char *key );
	void					 ( *PlayerInfo_SetValueForKey )				( const char *key, const char *value );
	qboolean				 ( *GetPlayerUniqueID )						( int iPlayer, char playerID[ PLAYERID_BUFFER_SIZE ] );
	int						 ( *GetTrackerIDForPlayer )					( int playerSlot );
	int						 ( *GetPlayerForTrackerID )					( int trackerID );
	int						 ( *pfnServerCmdUnreliable )				( char *szCmdString );
	void					 ( *pfnGetMousePos )						( POINT* ppt );
	void					 ( *pfnSetMousePos )						( int x, int y );
	void					 ( *pfnSetMouseEnable )						( qboolean fEnable );
	cvar_t*					 ( *GetFirstCvarPtr )						( void );
	unsigned int			 ( *GetFirstCmdFunctionHandle )				( void );
	unsigned int			 ( *GetNextCmdFunctionHandle )				( unsigned int cmdhandle );
	const char *			 ( *GetCmdFunctionName )					( unsigned int cmdhandle );
	float					 ( *hudGetClientOldTime )					( void );
	float					 ( *hudGetServerGravityValue )				( void );

	model_t*				 ( *hudGetModelByIndex )					( int index );
	void					 ( *pfnSetFilterMode )						( int mode );
	void					 ( *pfnSetFilterColor )						( float r, float g, float b );
	void					 ( *pfnSetFilterBrightness )				( float brightness );
	sequenceEntry_s*		 ( *pfnSequenceGet )						( const char *fileName, const char* entryName );
	void					 ( *pfnSPR_DrawGeneric )					( int frame, int x, int y, const wrect_t* prc, int src, int dest, int w, int h );
	sentenceEntry_s*		 ( *pfnSequencePickSentence )				( const char *sentenceName, int pickMethod, int* entryPicked );
	// draw a complete string
	int						 ( *pfnDrawString )							( int x, int y, const char *str, int r, int g, int b );
	int						 ( *pfnDrawStringReverse )					( int x, int y, const char *str, int r, int g, int b );
	const char*				 ( *LocalPlayerInfo_ValueForKey )			( const char *key );
	int						 ( *pfnVGUI2DrawCharacter )					( int x, int y, int ch, unsigned int font );
	int						 ( *pfnVGUI2DrawCharacterAdd )				( int x, int y, int ch, int r, int g, int b, unsigned int font );
	unsigned int			 ( *COM_GetApproxWavePlayLength )			( const char * filename );
	void*					 ( *pfnGetCareerUI )						( void );
	void					 ( *Cvar_Set )								( char *cvar, char *value );

	int						 ( *pfnIsCareerMatch )						( void );
	void					 ( *pfnPlaySoundVoiceByName )				( char *szSound, float volume, int pitch );
	void					 ( *pfnPrimeMusicStream )					( char *szFilename, int looping );
	double					 ( *GetAbsoluteTime )						( void );
	void					 ( *pfnProcessTutorMessageDecayBuffer )		( int *buffer, int bufferLength );
	void					 ( *pfnConstructTutorMessageDecayBuffer )	( int *buffer, int bufferLength );
	void					 ( *pfnResetTutorMessageDecayData )			( void );
	void					 ( *pfnPlaySoundByNameAtPitch )				( char *szSound, float volume, int pitch );
	void					 ( *pfnFillRGBABlend )						( int x, int y, int width, int height, int r, int g, int b, int a );
	int						 ( *pfnGetAppID )							( void );
	cmdalias_t*				 ( *pfnGetAliasList )						( void );
	void					 ( *pfnVguiWrap2_GetMouseDelta )			( int *x, int *y );
} cl_enginefunc_t;

/**
*	@defgroup ClientEngineExports Client Engine Exports
*
*	Function type declarations for engine exports
*	Now declares pointers in the interface and decltypes it into the pointer type, making it easier to see parameters in IntelliSense.
*
*	@{
*/

using pfnEngSrc_pfnSPR_Load_t							= decltype( cl_enginefunc_t::pfnSPR_Load );
using pfnEngSrc_pfnSPR_Frames_t							= decltype( cl_enginefunc_t::pfnSPR_Frames );
using pfnEngSrc_pfnSPR_Height_t							= decltype( cl_enginefunc_t::pfnSPR_Height );
using pfnEngSrc_pfnSPR_Width_t							= decltype( cl_enginefunc_t::pfnSPR_Width );
using pfnEngSrc_pfnSPR_Set_t							= decltype( cl_enginefunc_t::pfnSPR_Set );
using pfnEngSrc_pfnSPR_Draw_t							= decltype( cl_enginefunc_t::pfnSPR_Draw );
using pfnEngSrc_pfnSPR_DrawHoles_t						= decltype( cl_enginefunc_t::pfnSPR_DrawHoles );
using pfnEngSrc_pfnSPR_DrawAdditive_t					= decltype( cl_enginefunc_t::pfnSPR_DrawAdditive );
using pfnEngSrc_pfnSPR_EnableScissor_t					= decltype( cl_enginefunc_t::pfnSPR_EnableScissor );
using pfnEngSrc_pfnSPR_DisableScissor_t					= decltype( cl_enginefunc_t::pfnSPR_DisableScissor );
using pfnEngSrc_pfnSPR_GetList_t						= decltype( cl_enginefunc_t::pfnSPR_GetList );
using pfnEngSrc_pfnFillRGBA_t							= decltype( cl_enginefunc_t::pfnFillRGBA );
using pfnEngSrc_pfnGetScreenInfo_t						= decltype( cl_enginefunc_t::pfnGetScreenInfo );
using pfnEngSrc_pfnSetCrosshair_t						= decltype( cl_enginefunc_t::pfnSetCrosshair );
using pfnEngSrc_pfnRegisterVariable_t					= decltype( cl_enginefunc_t::pfnRegisterVariable );
using pfnEngSrc_pfnGetCvarFloat_t						= decltype( cl_enginefunc_t::pfnGetCvarFloat );
using pfnEngSrc_pfnGetCvarString_t						= decltype( cl_enginefunc_t::pfnGetCvarString );
using pfnEngSrc_pfnAddCommand_t							= decltype( cl_enginefunc_t::pfnAddCommand );
using pfnEngSrc_pfnHookUserMsg_t						= decltype( cl_enginefunc_t::pfnHookUserMsg );
using pfnEngSrc_pfnServerCmd_t							= decltype( cl_enginefunc_t::pfnServerCmd );
using pfnEngSrc_pfnClientCmd_t							= decltype( cl_enginefunc_t::pfnClientCmd );
using pfnEngSrc_pfnGetPlayerInfo_t						= decltype( cl_enginefunc_t::pfnGetPlayerInfo );
using pfnEngSrc_pfnPlaySoundByName_t					= decltype( cl_enginefunc_t::pfnPlaySoundByName );
using pfnEngSrc_pfnPlaySoundByIndex_t					= decltype( cl_enginefunc_t::pfnPlaySoundByIndex );
using pfnEngSrc_pfnAngleVectors_t						= decltype( cl_enginefunc_t::pfnAngleVectors );
using pfnEngSrc_pfnTextMessageGet_t						= decltype( cl_enginefunc_t::pfnTextMessageGet );
using pfnEngSrc_pfnDrawCharacter_t						= decltype( cl_enginefunc_t::pfnDrawCharacter );
using pfnEngSrc_pfnDrawConsoleString_t					= decltype( cl_enginefunc_t::pfnDrawConsoleString );
using pfnEngSrc_pfnDrawSetTextColor_t					= decltype( cl_enginefunc_t::pfnDrawSetTextColor );
using pfnEngSrc_pfnDrawConsoleStringLen_t				= decltype( cl_enginefunc_t::pfnDrawConsoleStringLen );
using pfnEngSrc_pfnConsolePrint_t						= decltype( cl_enginefunc_t::pfnConsolePrint );
using pfnEngSrc_pfnCenterPrint_t						= decltype( cl_enginefunc_t::pfnCenterPrint );
using pfnEngSrc_GetWindowCenterX_t						= decltype( cl_enginefunc_t::GetWindowCenterX );
using pfnEngSrc_GetWindowCenterY_t						= decltype( cl_enginefunc_t::GetWindowCenterY );
using pfnEngSrc_GetViewAngles_t							= decltype( cl_enginefunc_t::GetViewAngles );
using pfnEngSrc_SetViewAngles_t							= decltype( cl_enginefunc_t::SetViewAngles );
using pfnEngSrc_GetMaxClients_t							= decltype( cl_enginefunc_t::GetMaxClients );
using pfnEngSrc_Cvar_SetValue_t							= decltype( cl_enginefunc_t::Cvar_SetValue );
using pfnEngSrc_Cmd_Argc_t								= decltype( cl_enginefunc_t::Cmd_Argc );
using pfnEngSrc_Cmd_Argv_t								= decltype( cl_enginefunc_t::Cmd_Argv );
using pfnEngSrc_Con_Printf_t							= decltype( cl_enginefunc_t::Con_Printf );
using pfnEngSrc_Con_DPrintf_t							= decltype( cl_enginefunc_t::Con_DPrintf );
using pfnEngSrc_Con_NPrintf_t							= decltype( cl_enginefunc_t::Con_NPrintf );
using pfnEngSrc_Con_NXPrintf_t							= decltype( cl_enginefunc_t::Con_NXPrintf );
using pfnEngSrc_PhysInfo_ValueForKey_t					= decltype( cl_enginefunc_t::PhysInfo_ValueForKey );
using pfnEngSrc_ServerInfo_ValueForKey_t				= decltype( cl_enginefunc_t::ServerInfo_ValueForKey );
using pfnEngSrc_GetClientMaxspeed_t						= decltype( cl_enginefunc_t::GetClientMaxspeed );
using pfnEngSrc_CheckParm_t								= decltype( cl_enginefunc_t::CheckParm );
using pfnEngSrc_Key_Event_t								= decltype( cl_enginefunc_t::Key_Event );
using pfnEngSrc_GetMousePosition_t						= decltype( cl_enginefunc_t::GetMousePosition );
using pfnEngSrc_IsNoClipping_t							= decltype( cl_enginefunc_t::IsNoClipping );
using pfnEngSrc_GetLocalPlayer_t						= decltype( cl_enginefunc_t::GetLocalPlayer );
using pfnEngSrc_GetViewModel_t							= decltype( cl_enginefunc_t::GetViewModel );
using pfnEngSrc_GetEntityByIndex_t						= decltype( cl_enginefunc_t::GetEntityByIndex );
using pfnEngSrc_GetClientTime_t							= decltype( cl_enginefunc_t::GetClientTime );
using pfnEngSrc_V_CalcShake_t							= decltype( cl_enginefunc_t::V_CalcShake );
using pfnEngSrc_V_ApplyShake_t							= decltype( cl_enginefunc_t::V_ApplyShake );
using pfnEngSrc_PM_PointContents_t						= decltype( cl_enginefunc_t::PM_PointContents );

using pfnEngSrc_PM_WaterEntity_t						= decltype( cl_enginefunc_t::PM_WaterEntity );
using pfnEngSrc_PM_TraceLine_t							= decltype( cl_enginefunc_t::PM_TraceLine );
using pfnEngSrc_CL_LoadModel_t							= decltype( cl_enginefunc_t::CL_LoadModel );
using pfnEngSrc_CL_CreateVisibleEntity_t				= decltype( cl_enginefunc_t::CL_CreateVisibleEntity );
using pfnEngSrc_GetSpritePointer_t						= decltype( cl_enginefunc_t::GetSpritePointer );
using pfnEngSrc_pfnPlaySoundByNameAtLocation_t			= decltype( cl_enginefunc_t::pfnPlaySoundByNameAtLocation );
using pfnEngSrc_pfnPrecacheEvent_t						= decltype( cl_enginefunc_t::pfnPrecacheEvent );
using pfnEngSrc_pfnPlaybackEvent_t						= decltype( cl_enginefunc_t::pfnPlaybackEvent );
using pfnEngSrc_pfnWeaponAnim_t							= decltype( cl_enginefunc_t::pfnWeaponAnim );
using pfnEngSrc_pfnRandomFloat_t						= decltype( cl_enginefunc_t::pfnRandomFloat );
using pfnEngSrc_pfnRandomLong_t							= decltype( cl_enginefunc_t::pfnRandomLong );
using pfnEngSrc_pfnHookEvent_t							= decltype( cl_enginefunc_t::pfnHookEvent );
using pfnEngSrc_Con_IsVisible_t							= decltype( cl_enginefunc_t::Con_IsVisible );
using pfnEngSrc_pfnGetGameDirectory_t					= decltype( cl_enginefunc_t::pfnGetGameDirectory );
using pfnEngSrc_pfnGetCvarPointer_t						= decltype( cl_enginefunc_t::pfnGetCvarPointer );
using pfnEngSrc_Key_LookupBinding_t						= decltype( cl_enginefunc_t::Key_LookupBinding );
using pfnEngSrc_pfnGetLevelName_t						= decltype( cl_enginefunc_t::pfnGetLevelName );
using pfnEngSrc_pfnGetScreenFade_t						= decltype( cl_enginefunc_t::pfnGetScreenFade );
using pfnEngSrc_pfnSetScreenFade_t						= decltype( cl_enginefunc_t::pfnSetScreenFade );
using pfnEngSrc_VGui_GetPanel_t							= decltype( cl_enginefunc_t::VGui_GetPanel );
using pfnEngSrc_VGui_ViewportPaintBackground_t			= decltype( cl_enginefunc_t::VGui_ViewportPaintBackground );
using pfnEngSrc_COM_LoadFile_t							= decltype( cl_enginefunc_t::COM_LoadFile );
using pfnEngSrc_COM_ParseFile_t							= decltype( cl_enginefunc_t::COM_ParseFile );
using pfnEngSrc_COM_FreeFile_t							= decltype( cl_enginefunc_t::COM_FreeFile );

using pfnEngSrc_IsSpectateOnly_t						= decltype( cl_enginefunc_t::IsSpectateOnly );
using pfnEngSrc_LoadMapSprite_t							= decltype( cl_enginefunc_t::LoadMapSprite );
using pfnEngSrc_COM_AddAppDirectoryToSearchPath_t		= decltype( cl_enginefunc_t::COM_AddAppDirectoryToSearchPath );
using pfnEngSrc_COM_ExpandFilename_t					= decltype( cl_enginefunc_t::COM_ExpandFilename );
using pfnEngSrc_PlayerInfo_ValueForKey_t				= decltype( cl_enginefunc_t::PlayerInfo_ValueForKey );
using pfnEngSrc_PlayerInfo_SetValueForKey_t				= decltype( cl_enginefunc_t::PlayerInfo_SetValueForKey );
using pfnEngSrc_GetPlayerUniqueID_t						= decltype( cl_enginefunc_t::GetPlayerUniqueID );
using pfnEngSrc_GetTrackerIDForPlayer_t					= decltype( cl_enginefunc_t::GetTrackerIDForPlayer );
using pfnEngSrc_GetPlayerForTrackerID_t					= decltype( cl_enginefunc_t::GetPlayerForTrackerID );
using pfnEngSrc_pfnServerCmdUnreliable_t				= decltype( cl_enginefunc_t::pfnServerCmdUnreliable );
using pfnEngSrc_GetMousePos_t							= decltype( cl_enginefunc_t::pfnGetMousePos );
using pfnEngSrc_SetMousePos_t							= decltype( cl_enginefunc_t::pfnSetMousePos );
using pfnEngSrc_SetMouseEnable_t						= decltype( cl_enginefunc_t::pfnSetMouseEnable );
using pfnEngSrc_GetFirstCVarPtr_t						= decltype( cl_enginefunc_t::GetFirstCvarPtr );
using pfnEngSrc_GetFirstCmdFunctionHandle_t				= decltype( cl_enginefunc_t::GetFirstCmdFunctionHandle );
using pfnEngSrc_GetNextCmdFunctionHandle_t				= decltype( cl_enginefunc_t::GetNextCmdFunctionHandle );
using pfnEngSrc_GetCmdFunctionName_t					= decltype( cl_enginefunc_t::GetCmdFunctionName );
using pfnEngSrc_GetClientOldTime_t						= decltype( cl_enginefunc_t::hudGetClientOldTime );
using pfnEngSrc_GetServerGravityValue_t					= decltype( cl_enginefunc_t::hudGetServerGravityValue );

using pfnEngSrc_GetModelByIndex_t						= decltype( cl_enginefunc_t::hudGetModelByIndex );
using pfnEngSrc_pfnSetFilterMode_t						= decltype( cl_enginefunc_t::pfnSetFilterMode );
using pfnEngSrc_pfnSetFilterColor_t						= decltype( cl_enginefunc_t::pfnSetFilterColor );
using pfnEngSrc_pfnSetFilterBrightness_t				= decltype( cl_enginefunc_t::pfnSetFilterBrightness );
using pfnEngSrc_pfnSequenceGet_t						= decltype( cl_enginefunc_t::pfnSequenceGet );
using pfnEngSrc_pfnSPR_DrawGeneric_t					= decltype( cl_enginefunc_t::pfnSPR_DrawGeneric );
using pfnEngSrc_pfnSequencePickSentence_t				= decltype( cl_enginefunc_t::pfnSequencePickSentence );
using pfnEngSrc_pfnDrawString_t							= decltype( cl_enginefunc_t::pfnDrawString );
using pfnEngSrc_pfnDrawStringReverse_t					= decltype( cl_enginefunc_t::pfnDrawStringReverse );
using pfnEngSrc_LocalPlayerInfo_ValueForKey_t			= decltype( cl_enginefunc_t::LocalPlayerInfo_ValueForKey );
using pfnEngSrc_pfnVGUI2DrawCharacter_t					= decltype( cl_enginefunc_t::pfnVGUI2DrawCharacter );
using pfnEngSrc_pfnVGUI2DrawCharacterAdd_t				= decltype( cl_enginefunc_t::pfnVGUI2DrawCharacterAdd );
using pfnEngSrc_COM_GetApproxWavePlayLength				= decltype( cl_enginefunc_t::COM_GetApproxWavePlayLength );
using pfnEngSrc_pfnGetCareerUI_t						= decltype( cl_enginefunc_t::pfnGetCareerUI );
using pfnEngSrc_Cvar_Set_t								= decltype( cl_enginefunc_t::Cvar_Set );
using pfnEngSrc_pfnIsPlayingCareerMatch_t				= decltype( cl_enginefunc_t::pfnIsCareerMatch );
using pfnEngSrc_pfnPlaySoundVoiceByName_t				= decltype( cl_enginefunc_t::pfnPlaySoundVoiceByName );
using pfnEngSrc_pfnPrimeMusicStream_t					= decltype( cl_enginefunc_t::pfnPrimeMusicStream );
using pfnEngSrc_GetAbsoluteTime_t						= decltype( cl_enginefunc_t::GetAbsoluteTime );
using pfnEngSrc_pfnProcessTutorMessageDecayBuffer_t		= decltype( cl_enginefunc_t::pfnProcessTutorMessageDecayBuffer );
using pfnEngSrc_pfnConstructTutorMessageDecayBuffer_t	= decltype( cl_enginefunc_t::pfnConstructTutorMessageDecayBuffer );
using pfnEngSrc_pfnResetTutorMessageDecayData_t			= decltype( cl_enginefunc_t::pfnResetTutorMessageDecayData );
using pfnEngSrc_pfnPlaySoundByNameAtPitch_t				= decltype( cl_enginefunc_t::pfnPlaySoundByNameAtPitch );
using pfnEngSrc_pfnFillRGBABlend_t						= decltype( cl_enginefunc_t::pfnFillRGBABlend );
using pfnEngSrc_pfnGetAppID_t							= decltype( cl_enginefunc_t::pfnGetAppID );
using pfnEngSrc_pfnGetAliases_t							= decltype( cl_enginefunc_t::pfnGetAliasList );
using pfnEngSrc_pfnVguiWrap2_GetMouseDelta_t			= decltype( cl_enginefunc_t::pfnVguiWrap2_GetMouseDelta );

/** @} */

#endif // __APIPROXY__
