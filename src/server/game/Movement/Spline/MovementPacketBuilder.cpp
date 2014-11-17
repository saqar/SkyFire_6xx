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

#include "MovementPacketBuilder.h"
#include "MoveSpline.h"
#include "WorldPacket.h"
#include "Object.h"
#include "Unit.h"
#include "Transport.h"

namespace Movement
{
    inline void operator << (ByteBuffer& b, const Vector3& v)
    {
        b << v.x << v.y << v.z;
    }

    inline void operator >> (ByteBuffer& b, Vector3& v)
    {
        b >> v.x >> v.y >> v.z;
    }

    MonsterMoveType PacketBuilder::GetMonsterMoveType(MoveSpline const& moveSpline)
    {
        switch (moveSpline.splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::Final_Target:
                return MonsterMoveFacingTarget;
            case MoveSplineFlag::Final_Angle:
                return MonsterMoveFacingAngle;
            case MoveSplineFlag::Final_Point:
                return MonsterMoveFacingPoint;
            default:
                return MonsterMoveNormal;
        }
    }

    void PacketBuilder::WriteStopMovement(Vector3 const& pos, uint32 splineId, ByteBuffer& data, Unit* unit)
    {

        ObjectGuid guid = unit->GetGUID();
        ObjectGuid transport = unit->GetTransGUID();

        data << guid;
        data << float(pos.x);
        data << float(pos.y);
        data << float(pos.z);
        data << uint32(splineId);
        data << float(0.f);//lastPoint.x);
        data << float(0.f);//lastPoint.y);
        data << float(0.f);//lastPoint.z);
        data << uint32(0);
        data << uint8(0); // AnimTier
        data << uint32(0); // TierTransStartTime
        data << int32(0);
        data << uint32(0);
        data << float(0.f);
        data << uint32(0);
        data << uint32(0);

        data << uint8(0);
        data << uint8(0);
        data << transport;
        data << uint8(unit->GetTransport() ? unit->GetTransport()->GetTransSeat() : 0);
        data << uint32(0);

        data.WriteBits(MonsterMoveFacingAngle, 2);
        data.WriteBit(0); // SplineFilters
        data << float(0.f);
        data.WriteBit(0); // Teleport
        data.WriteBits(1, 2);
        data.FlushBits();
    }

    void WriteLinearPath(Spline<int32> const& spline, ByteBuffer& data)
    {
        uint32 last_idx = spline.getPointCount() - 3;
        Vector3 const* real_path = &spline.getPoint(1);

        if (last_idx > 0)
        {
            Vector3 middle = (real_path[0] + real_path[last_idx]) / 2.f;
            Vector3 offset;
            // first and last points already appended
            for (uint32 i = 0; i < last_idx; ++i)
            {
                offset = middle - real_path[i];
                data.appendPackXYZ(offset.x, offset.y, offset.z);
            }
        }
    }

    void WriteUncompressedPath(Spline<int32> const& spline, ByteBuffer& data)
    {
        for (int i = 2; i < spline.getPointCount() - 3; i++)
            data << spline.getPoint(i).x << spline.getPoint(i).y << spline.getPoint(i).z;
    }

    void WriteUncompressedCyclicPath(Spline<int32> const& spline, ByteBuffer& data)
    {
        data << spline.getPoint(1).x << spline.getPoint(1).y << spline.getPoint(1).z; // Fake point, client will erase it from the spline after first cycle done

        for (int i = 1; i < spline.getPointCount() - 3; i++)
            data << spline.getPoint(i).x << spline.getPoint(i).y << spline.getPoint(i).z;
    }

    void PacketBuilder::WriteMonsterMove(const MoveSpline& moveSpline, WorldPacket& data, Unit* unit)
    {
        ObjectGuid guid = unit->GetGUID();
        ObjectGuid transport = unit->GetTransGUID();
        MonsterMoveType type = GetMonsterMoveType(moveSpline);
        G3D::Vector3 const& firstPoint = moveSpline.spline.getPoint(moveSpline.spline.first());

        data << guid;
        data << float(firstPoint.x);
        data << float(firstPoint.y);
        data << float(firstPoint.z);
        data << uint32(moveSpline.GetId());
        data << float(0.f);//lastPoint.x);
        data << float(0.f);//lastPoint.y);
        data << float(0.f);//lastPoint.z);

        data << uint32(0);//moveSpline.splineflags.raw());
        data << uint8(0); // AnimTier
        data << uint32(0); // TierTransStartTime
        data << int32(moveSpline.timePassed());
        data << uint32(moveSpline.Duration());
        data << float(moveSpline.vertical_acceleration);
        data << uint32(0);

        // Old 20 bits
        uint32 uncompressedSplineCount = moveSpline.splineflags & MoveSplineFlag::UncompressedPath ? moveSpline.splineflags.cyclic ? moveSpline.spline.getPointCount() - 2 : moveSpline.spline.getPointCount() - 3 : 1;
        data << uint32(uncompressedSplineCount);

        data << uint8(moveSpline.spline.mode());
        data << uint8(0);
        data << transport;
        data << uint8(unit->GetTransport() ? unit->GetTransport()->GetTransSeat() : 0);

        // Old 22 bits
        int32 compressedSplineCount = uncompressedSplineCount ? 0 : moveSpline.splineflags & MoveSplineFlag::UncompressedPath ? 0 : moveSpline.spline.getPointCount() - 3;
        data << uint32(compressedSplineCount);

        if (moveSpline.splineflags & MoveSplineFlag::UncompressedPath)
        {
            if (moveSpline.splineflags.cyclic)
                WriteUncompressedCyclicPath(moveSpline.spline, data);
            else
                WriteUncompressedPath(moveSpline.spline, data);
        }
        else
        {
            G3D::Vector3 const& point = moveSpline.spline.getPoint(moveSpline.spline.getPointCount() - 2);
            data << point.x << point.y << point.z;
        }
        
        if (compressedSplineCount)
            WriteLinearPath(moveSpline.spline, data);

        data.WriteBits(type, 2);
        data.WriteBit(0); // SplineFilters
        data.FlushBits();
        
        if (type == MonsterMoveFacingAngle || type == MonsterMoveFacingTarget)
            data << float(type == MonsterMoveFacingTarget ? unit->GetOrientation() : moveSpline.facing.angle);

        if (type == MonsterMoveFacingTarget)
            data << ObjectGuid(moveSpline.facing.target);

        if (type == MonsterMoveFacingPoint)
            data << moveSpline.facing.f.x << moveSpline.facing.f.y << moveSpline.facing.f.z;
            
        data.WriteBit(0); // Teleport
        data.WriteBits(0, 2);
        data.FlushBits();
    }

    void PacketBuilder::WriteCreateBits(MoveSpline const& moveSpline, ByteBuffer& data)
    {
    }

    void PacketBuilder::WriteCreateData(MoveSpline const& moveSpline, ByteBuffer& data)
    {
    
        Vector3 destination = moveSpline.isCyclic() ? Vector3::zero() : moveSpline.FinalDestination();

        data << moveSpline.GetId();
        data << float(destination.x);
        data << float(destination.y);
        data << float(destination.z);
        
        if (!data.WriteBit(!moveSpline.Finalized()))
            return;

        if (!moveSpline.Finalized())
        {
            MoveSplineFlag const& splineFlags = moveSpline.splineflags;
            MonsterMoveType type = GetMonsterMoveType(moveSpline);
            uint32 nodes = moveSpline.getPath().size();

            data.WriteBits(moveSpline.splineflags.raw(), 25);
            data.WriteBits(type, 2);
            data.WriteBit(moveSpline.splineflags & (MoveSplineFlag::Parabolic | MoveSplineFlag::Animation));
            data.WriteBit((moveSpline.splineflags & MoveSplineFlag::Parabolic) && moveSpline.effect_start_time < moveSpline.Duration());
            data.WriteBits(moveSpline.spline.mode(), 2);
            data.WriteBit(0); // NYI Block
            
            data << moveSpline.timePassed();
            data << moveSpline.Duration();
            data << float(1.f);                             // splineInfo.duration_mod; added in 3.1
            data << float(1.f);                             // splineInfo.duration_mod_next; added in 3.1
            data << uint32(nodes);
            
            if (type == MonsterMoveFacingAngle)
                data << float(moveSpline.facing.angle);
                
            if (type == MonsterMoveFacingTarget)
                data << float(moveSpline.facing.target);
            
            if (type == MonsterMoveFacingPoint)
                data << moveSpline.facing.f.x << moveSpline.facing.f.y << moveSpline.facing.f.z;
                
            if ((splineFlags & MoveSplineFlag::Parabolic) && moveSpline.effect_start_time < moveSpline.Duration())
                data << float(moveSpline.vertical_acceleration);   // added in 3.1
                
            if (splineFlags & (MoveSplineFlag::Parabolic | MoveSplineFlag::Animation))
                data << moveSpline.effect_start_time;       // added in 3.1

            for (uint32 i = 0; i < nodes; ++i)
            {
                data << float(moveSpline.getPath()[i].x);
                data << float(moveSpline.getPath()[i].y);
                data << float(moveSpline.getPath()[i].z);
            }

        }
    }

    void PacketBuilder::WriteFacingTargetPart(MoveSpline const& moveSpline, ByteBuffer& data)
    {
        if (GetMonsterMoveType(moveSpline) == MonsterMoveFacingTarget && !moveSpline.Finalized())
        {
            ObjectGuid facingGuid = moveSpline.facing.target;
            data << facingGuid;
        }
    }
}
