/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
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
 
#include "ShipmentMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

ShipmentMgr::ShipmentMgr() {

    for (auto itr : m_shipmentStore)
        delete itr;

    m_shipmentStore.clear();
};

void ShipmentMgr::SendShipmentOfTypeResponse(WorldSession* session)
{
    WorldPacket data(SMSG_GET_SHIPMENTS_OF_TYPE_RESPONSE);
    Shipment const* shipment = 0;

    data << uint32(shipment->Container);                      // Unknown

    data << uint64(shipment->ShipmentEntry);
    data << uint32(shipment->ShipmentRecID);
    data << uint32(shipment->CreationTime);
    data << uint32(shipment->ShipmentDuration);

    session->SendPacket(&data);
}