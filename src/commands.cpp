/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "detours.h"
#include "common.h"
#include "utlstring.h"
#include "recipientfilters.h"
#include "commands.h"
#include "utils/entity.h"
#include "entity/cbaseentity.h"
#include "entity/ccsweaponbase.h"
#include "entity/ccsplayercontroller.h"
#include "entity/ccsplayerpawn.h"
#include "entity/cbasemodelentity.h"
#include "playermanager.h"
#include "ctimer.h"

#include "tier0/memdbgon.h"

extern CEntitySystem *g_pEntitySystem;
extern IVEngineServer2* g_pEngineServer2;
extern int g_targetPawn;
extern int g_targetController;

void ParseChatCommand(const char *pMessage, CCSPlayerController *pController)
{
	if (!pController)
		return;

	CCommand args;
	args.Tokenize(pMessage + 1);

	uint16 index = g_CommandList.Find(hash_32_fnv1a_const(args[0]));

	if (g_CommandList.IsValidIndex(index))
	{
		g_CommandList[index](args, pController);
	}

}

void ClientPrintAll(int hud_dest, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	char buf[256];
	V_vsnprintf(buf, sizeof(buf), msg, args);

	va_end(args);

	addresses::UTIL_ClientPrintAll(hud_dest, buf, nullptr, nullptr, nullptr, nullptr);
}

void ClientPrint(CBasePlayerController *player, int hud_dest, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	char buf[256];
	V_vsnprintf(buf, sizeof(buf), msg, args);

	va_end(args);

	addresses::ClientPrint(player, hud_dest, buf, nullptr, nullptr, nullptr, nullptr);
}

CUtlVector <CCSPlayerController*> coaches;

void print_coaches(){
	if (coaches.Count() < 1) return;
	
	ClientPrintAll(HUD_PRINTTALK, CHAT_PREFIX"Coaches list: %i", coaches.Count());
	/*FOR_EACH_VEC(coaches,i){
		ClientPrintAll(HUD_PRINTTALK, CHAT_PREFIX"Coach %i:%s", i+1, coaches[i]->GetPlayerName());
	}*/
}

CON_COMMAND_CHAT(coach, "Request slot coach")
{
	if (!player)
		return;
	
	int iPlayer = player->GetPlayerSlot();

	player->m_pInGameMoneyServices->m_iAccount = 0;

	//Check it is not existing already
	
	FOR_EACH_VEC(coaches,i){
		if(coaches[i]->GetPlayerSlot() == iPlayer){	
			ClientPrint(player, HUD_PRINTTALK, CHAT_PREFIX "Your are already a coach type \4.uncoach \1to be a player");
			return;
		}
	}

	coaches.AddToTail(player);

	ClientPrint(player, HUD_PRINTTALK, CHAT_PREFIX "Coach enabled, type \4.uncoach \1to cancel");
	print_coaches();

	
	CHandle<CCSPlayerController> hController = player->GetHandle();

	// Gotta do this on the next frame...
	new CTimer(0.0f, false, false, [hController]()
	{
		CCSPlayerController *pController = hController.Get();

		if (!pController)
			return;

		pController->m_szClan = "Coaching:";
		return;
	});
}

CON_COMMAND_CHAT(uncoach, "Undo slot coach")
{
	if (!player)
		return;
	
	int iPlayer = player->GetPlayerSlot();
	CBasePlayerController *pTarget = (CBasePlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iPlayer + 1));

	if (!pTarget)
		return;

	player->m_pInGameMoneyServices->m_iAccount = 0;

	player->m_szClan = "";

	FOR_EACH_VEC(coaches,i){
		if(coaches[i]->GetPlayerSlot() == iPlayer){
			coaches.Remove(i);
		}
	}

	ClientPrint(player, HUD_PRINTTALK, CHAT_PREFIX "You are no longer set as \4coach\1");
	print_coaches();
}

CON_COMMAND_CHAT(coach_ct, "Switch team test"){
	ClientPrint(player, HUD_PRINTTALK, "Test");
	player->ChangeTeam(CS_TEAM_CT);
}
CON_COMMAND_CHAT(coach_t, "Switch team test"){
	ClientPrint(player, HUD_PRINTTALK, "Test");
	player->ChangeTeam(CS_TEAM_T);
}
CON_COMMAND_CHAT(coach_spec, "Switch team test"){
	ClientPrint(player, HUD_PRINTTALK, "Test");
	player->ChangeTeam(CS_TEAM_SPECTATOR);
}
CON_COMMAND_CHAT(coach_test, "Switch team test"){
	ClientPrint(player, HUD_PRINTTALK, "Test");
	player->ChangeTeam(CS_TEAM_SPECTATOR);
	player->ChangeTeam(CS_TEAM_CT);
}

extern float coach_timer;
CON_COMMAND_F(c_coach_timer, "change coach killing timer",FCVAR_SPONLY | FCVAR_LINKED_CONCOMMAND){
	if (args.ArgC() > 1)
			coach_timer = atoi(args[1]);

	Message("Coach killing timer is: %f \n", coach_timer);
}