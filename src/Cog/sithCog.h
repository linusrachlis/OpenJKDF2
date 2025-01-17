#ifndef _SITHCOG_H
#define _SITHCOG_H

#include "types.h"
#include "globals.h"
#include "Cog/sithCogExec.h"
#include "Cog/sithCogScript.h"

#define jkCog_RegisterVerbs_ADDR (0x40A110)
#define jkCog_Startup_ADDR (0x40A0C0)
#define sithCogFunction_Startup_ADDR (0x00505400)
#define sithCogFunctionThing_Startup_ADDR (0x005014E0)
#define sithCogFunctionAI_Startup_ADDR (0x00500B00)
#define sithCogFunctionSound_Startup_ADDR (0x004FF060)
#define sithCogFunctionPlayer_Startup_ADDR (0x004E0780)
#define sithCogFunctionSector_Startup_ADDR (0x004FE680)
#define sithCogFunctionSurface_Startup_ADDR (0x004FFB50)

#define sithCog_Startup_ADDR (0x004DE070)
#define sithCog_Shutdown_ADDR (0x004DE590)
#define sithCog_Open_ADDR (0x004DE5D0)
#define sithCog_Close_ADDR (0x004DE7E0)
#define sithCog_Free_ADDR (0x004DE820)
#define sithCog_FreeEntry_ADDR (0x004DE9B0)
#define sithCog_Free2_ADDR (0x004DEA20)
#define sithCog_HandleThingTimerPulse_ADDR (0x004DEA60)
#define sithCog_SendSimpleMessageToAll_ADDR (0x004DEAD0)
#define sithCog_SendMessageToAll_ADDR (0x004DEB00)
#define sithCog_SendMessage_ADDR (0x004DEBE0)
#define sithCog_SendMessageEx_ADDR (0x004DEDC0)
#define sithCog_InitScripts_ADDR (0x004DEFF0)
#define sithCog_InitCogs_ADDR (0x004DF080)
#define sithCog_Load_ADDR (0x004DF110)
#define sithCog_LoadEntry_ADDR (0x004DF410)
#define sithCog_ThingsSectorsRegSymbolIdk_ADDR (0x004DF610)
#define sithCog_LoadCogscript_ADDR (0x004DF7D0)
#define sithCog_GetByIdx_ADDR (0x004DF930)
#define sithCog_ThingFromSymbolidk_ADDR (0x004DF980)
#define sithCog_Thingidk_ADDR (0x004DFA00)
#define sithCog_Sectoridk_ADDR (0x004DFA60)
#define sithCog_SendMessageFromThing_ADDR (0x004DFAC0)
#define sithCog_SendMessageFromThingEx_ADDR (0x004DFAF0)
#define sithCog_SendMessageFromSector_ADDR (0x004DFD60)
#define sithCog_SendMessageFromSectorEx_ADDR (0x004DFD90)
#define sithCog_SendMessageFromSurface_ADDR (0x004DFED0)
#define sithCog_SendMessageFromSurfaceEx_ADDR (0x004DFF00)
#define sithCogScript_Load_ADDR (0x004E0040)
#define sithCogScript_LoadEntry_ADDR (0x004E0240)
#define sithCogScript_Tick_ADDR (0x004E0300)
#define sithCogScript_TickAll_ADDR (0x004E0400)
#define sithCogScript_DevCmdCogStatus_ADDR (0x004E0480)
#define sithCogScript_RegisterMessageSymbol_ADDR (0x004E0600)
#define sithCogScript_TimerTick_ADDR (0x004E0640)
#define sithCogScript_RegisterGlobalMessage_ADDR (0x004E06C0)
#define sithCogScript_RegisterVerb_ADDR (0x004E0700)

#define COG_SHOULD_SYNC(ctx) (sithComm_multiplayerFlags && !(ctx->flags & SITH_COG_NO_SYNC) && ctx->trigId != SITH_MESSAGE_STARTUP && ctx->trigId != SITH_MESSAGE_SHUTDOWN)


static int (*_sithCog_Load)(sithWorld *world, int a2) = (void*)sithCog_Load_ADDR;
//static int (*sithCogScript_Load)(sithWorld *world, int a2) = (void*)sithCogScript_Load_ADDR;
//static void (*sithCogScript_RegisterVerb)(void* a, intptr_t func, char* cmd) = (void*)0x4E0700;
//static void (__cdecl *sithCog_SendMessage)(sithCog *a1, int msgid, int senderType, int senderIndex, int sourceType, int sourceIndex, int linkId) = (void*)0x4DEBE0;
//static float (__cdecl *sithCog_SendMessageEx)(sithCog *a1, SITH_MESSAGE message, int senderType, int senderIndex, int sourceType, int sourceIndex, int linkId, float param0, float param1, float param2, float param3) = (void*)0x4DEDC0;
//static void (*sithCog_HandleThingTimerPulse)(sithThing *a1) = (void*)sithCog_HandleThingTimerPulse_ADDR;
//static int (*sithCog_ThingsSectorsRegSymbolIdk)(sithCog *a1, sithCogReference *a2, sithCogSymbol *a3) = (void*)sithCog_ThingsSectorsRegSymbolIdk_ADDR;
static sithCog* (*_sithCog_LoadCogscript)(const char *fpath) = (void*)sithCog_LoadCogscript_ADDR;

int sithCog_Startup();
void sithCog_Shutdown();
int sithCog_Open();
void sithCog_Close();
int sithCog_Load(sithWorld *world, int a2);
sithCog* sithCog_LoadCogscript(const char *fpath);
int sithCog_LoadEntry(sithCogSymbol *cogSymbol, sithCogReference *cogIdk, char *val);
int sithCog_ThingsSectorsRegSymbolIdk(sithCog *cog, sithCogReference *idk, sithCogSymbol *symbol);
void sithCog_HandleThingTimerPulse(sithThing *thing);

void sithCogFunction_Startup(void* a1);
void sithCogThing_Startup(void* a1);
void sithCogFunctionSound_Startup(void* a1);
void sithCogFunctionSector_Startup(void* a1);
void sithCogSurface_Startup(void* a1);

void sithCog_SendMessageFromThing(sithThing *a1, sithThing *a2, int msg);
float sithCog_SendMessageFromThingEx(sithThing *sender, sithThing *receiver, SITH_MESSAGE message, float param0, float param1, float param2, float param3);
void sithCog_SendMessageFromSurface(sithSurface *surface, sithThing *thing, int msg);
double sithCog_SendMessageFromSurfaceEx(sithSurface *sender, sithThing *thing, SITH_MESSAGE msg, float a4, float a5, float a6, float a7);
void sithCog_SendMessageFromSector(sithSector *sector, sithThing *thing, int message);
float sithCog_SendMessageFromSectorEx(sithSector *a1, sithThing *sourceType, SITH_MESSAGE message, float param0, float param1, float param2, float param3);
void sithCog_SendSimpleMessageToAll(int a1, int a2, int a3, int a4, int a5);
void sithCog_SendMessageToAll(int cmdid, int senderType, int senderIdx, int sourceType, int sourceIdx, float arg0, float arg1, float arg2, float arg3);
void sithCog_SendMessage(sithCog *cog, int msgid, int senderType, int senderIndex, int sourceType, int sourceIndex, int linkId);
float sithCog_SendMessageEx(sithCog *cog, int message, int senderType, int senderIndex, int sourceType, int sourceIndex, int linkId, float param0, float param1, float param2, float param3);
void sithCog_Free(sithWorld *world);

static int (*_sithCog_Open)() = (void*)sithCog_Open_ADDR;
//static double (*sithCog_SendMessageFromSurfaceEx)(sithSurface *a1, sithThing *a2, int a3, float a4, float a5, float a6, float a7) = (void*)sithCog_SendMessageFromSurfaceEx_ADDR;
static float (*_sithCog_SendMessageFromThingEx)(sithThing *sender, sithThing *receiver, SITH_MESSAGE message, float param0, float param1, float param2, float param3) = (void*)sithCog_SendMessageFromThingEx_ADDR;
//static void (*sithCog_SendMessageFromSectorEx)(sithSector *a1, sithThing *sourceType, SITH_MESSAGE message, float param0, float param1, float param2, float param3) = (void*)sithCog_SendMessageFromSectorEx_ADDR;
//static void (*sithCog_SendMessageToAll)(int cmdid, int senderType, int senderIdx, int sourceType, int sourceIdx, float arg0, float arg1, float arg2, float arg3) = (void*)sithCog_SendMessageToAll_ADDR;
//static void (*sithCog_Free)(sithWorld* world) = (void*)sithCog_Free_ADDR;
//static void (*sithCogScript_Tick)(sithCog* cog) = (void*)sithCogScript_Tick_ADDR;

int sithCogScript_Load(sithWorld *lvl, int a2);
sithCogScript* sithCogScript_LoadEntry(const char *pFpath, int unk);
void sithCogScript_RegisterVerb(sithCogSymboltable *a1, cogSymbolFunc_t a2, char *a3);
void sithCogScript_RegisterMessageSymbol(sithCogSymboltable *a1, int a2, const char *a3);
void sithCogScript_RegisterGlobalMessage(sithCogSymboltable *a1, const char *a2, int a3);
void sithCogScript_TickAll();
void sithCogScript_Tick(sithCog *cog);
int sithCogScript_TimerTick(int deltaMs, sithEventInfo *info);
void sithCogScript_DevCmdCogStatus(stdDebugConsoleCmd *cmd, char *extra);
sithCog* sithCog_GetByIdx(int idx);

#endif // _SITHCOG_H
