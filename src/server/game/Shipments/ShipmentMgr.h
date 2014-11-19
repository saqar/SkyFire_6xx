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
 
#ifndef SHIPMENT_MGR_H
#define SHIPMENT_MGR_H

#define MAX_WORK_ORDERS 14


struct Shipment
{
    Shipment(uint32 _shipmentRecId, uint64 _shipmentEntry, uint32 _creationTime, uint32 _shipmentDuration, uint32 _shipmentId, 
        uint32 _plotInstanceId, uint32 _maxShipments, bool _success, uint32 _result, uint32 _container)
        : ShipmentRecID(_shipmentRecId), ShipmentEntry(_shipmentEntry), CreationTime(_creationTime), ShipmentDuration(_shipmentDuration),
        ShipmentID(_shipmentId), PlotInstanceID(_plotInstanceId), MaxShipments(_maxShipments), Success(_success), Result(_result), Container(_container) { }

public:
    uint32 ShipmentRecID;
    uint64 ShipmentEntry;
    uint32 CreationTime;
    uint32 ShipmentDuration;
    uint32 ShipmentID;
    uint32 PlotInstanceID;
    uint32 MaxShipments;
    bool Success;
    uint32 Result;
    uint32 Container;
};

typedef std::set<Shipment*> ShipmentSet;

class ShipmentMgr
{
public:
    ShipmentMgr();
    ~ShipmentMgr();

    void LoadFromDb();

    void SendShipmentOfTypeResponse(WorldSession* session);

private:
    ShipmentSet m_shipmentStore;

    void LoadShipmentsFromDb();
};

#endif