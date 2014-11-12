/*
* Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Chat.h"
#include "GameEventMgr.h"
#include "Language.h"
#include "Player.h"
#include "ScriptMgr.h"

class garrison_commandscript : public CommandScript
{
public:
    garrison_commandscript() : CommandScript("garrison_commandscript") { }

    ChatCommand* GetCommands() const OVERRIDE
    {
        static ChatCommand garrisonCommandTable[] =
        {
            { "architect", rbac::RBAC_PERM_COMMAND_GARRISON_ARCHITECT, true, &HandleGarrisonArchitectCommand, "", NULL },
            { "getinfo",   rbac::RBAC_PERM_COMMAND_GARRISON_GET_INFO,  true, &HandleGarrisonGetInfoCommand,   "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "garrison", rbac::RBAC_PERM_COMMAND_GARRISON, false, NULL, "", garrisonCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }

    static bool HandleGarrisonArchitectCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string param = (char*)args;

        if (param == "open")
        {
            WorldPacket data(SMSG_GARR_OPEN_ARCHITECT);
            ObjectGuid npcGuid;

            data << npcGuid;

            handler->GetSession()->SendPacket(&data);
            return true;
        }

        return false;
    }

    static bool HandleGarrisonGetInfoCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

		
    }
};

void AddSC_garrison_commandscript()
{
    new garrison_commandscript();
}