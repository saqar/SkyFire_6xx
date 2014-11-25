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

#include "MovementStructures.h"
#include "Player.h"


MovementStatusElements const MovementStatus[] =
{
    MSEGuid128,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSEPitch,
    MSESplineElevation,
    MSEForcesCount,
    MSECounter,
    MSEForces,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSEHasTransportData,
    MSEHasFallData,
    MSEHasSpline,
    MSEZeroBit,
    MSEOneBit,
    MSETransportGuid128,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSEHasTransportPrevTime,
    MSEHasTransportVehicleId,
    MSETransportPrevTime,
    MSETransportVehicleId,
    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEHasFallDirection,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,
    MSEEnd
};

MovementStatusElements const SplineMoveGuid[] =
{
    MSEGuid128,
    MSEEnd,
};

MovementStatusElements const MoveRootUnroot[] =
{
    MSEGuid128,
    MSECounter,
    MSEEnd,
};

MovementStatusElements const SplineMoveSetSpeed[] =
{
    MSEGuid128,
    MSEExtraElement,
    MSEEnd,
};

MovementStatusElements const MoveSetUnsetCanFly[] =
{
    MSEGuid128,
    MSEUintCount,
    MSEEnd,
};

MovementStatusElements const MoveSetWalkSpeed[] =
{
    MSEGuid128,
    MSEExtraElement,
    MSECounter,
    MSEEnd,
};

MovementStatusElements const MoveSetRunSpeed[] =
{
    MSEGuid128,
    MSECounter,
    MSEExtraElement,
    MSEEnd,
};

MovementStatusElements const MoveSetFlightSpeed[] =
{
    MSEGuid128,
    MSEUintCount,
    MSEExtraElement,
    MSEEnd,
};

void Movement::ExtraMovementStatusElement::ReadNextElement(ByteBuffer& packet)
{
    MovementStatusElements const element = _elements[_index++];

    switch (element)
    {
        case MSEExtraFloat:
            packet >> Data.floatData;
            break;
        case MSEExtraInt8:
            packet >> Data.byteData;
            break;
        default:
            ASSERT(PrintInvalidSequenceElement(element, __FUNCTION__));
            break;
    }
}

void Movement::ExtraMovementStatusElement::WriteNextElement(ByteBuffer& packet)
{
    MovementStatusElements const element = _elements[_index++];

    switch (element)
    {
        case MSEExtraFloat:
            packet << Data.floatData;
            break;
        case MSEExtraInt8:
            packet << Data.byteData;
            break;
        default:
            ASSERT(PrintInvalidSequenceElement(element, __FUNCTION__));
            break;
    }
}

bool Movement::PrintInvalidSequenceElement(MovementStatusElements const element, char const* function)
{
    TC_LOG_ERROR("entities.unit", "Incorrect sequence element %d detected at %s", element, function);
    return false;
}

Movement::PacketSender::PacketSender(Unit* unit, Opcodes serverControl, Opcodes playerControl, Opcodes broadcast /*= SMSG_PLAYER_MOVE*/, ExtraMovementStatusElement* extras /*= NULL*/)
    : _extraElements(extras), _unit(unit)
{
    if (unit->GetTypeId() == TYPEID_PLAYER && unit->ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER)
    {
        _selfOpcode = playerControl;
        _broadcast = broadcast;
    }
    else
    {
        _selfOpcode = NULL_OPCODE;
        _broadcast = serverControl;
    }
}

void Movement::PacketSender::Send() const
{
    bool isPlayerMovement = false;
    if (Player* player = _unit->ToPlayer())
    {
        isPlayerMovement = player->m_mover->GetTypeId() == TYPEID_PLAYER;
        if (isPlayerMovement && _selfOpcode != NULL_OPCODE)
        {
            WorldPacket data(_selfOpcode);
            _unit->WriteMovementInfo(data, _extraElements);
            player->SendDirectMessage(&data);
        }
    }

    if (_broadcast != NULL_OPCODE)
    {
        ///! Need to reset current extra element index before writing another packet
        if (_extraElements)
            _extraElements->ResetIndex();

        WorldPacket data(_broadcast);
        _unit->WriteMovementInfo(data, _extraElements);
        _unit->SendMessageToSet(&data, !isPlayerMovement);
    }
}

MovementStatusElements const* GetMovementStatusElementsSequence(Opcodes opcode)
{
    switch (opcode)
    {
        case CMSG_CAST_SPELL:
        case CMSG_MOVE_FALL_LAND:
        case CMSG_MOVE_HEARTBEAT:
        case CMSG_MOVE_JUMP:
        case CMSG_MOVE_SET_FACING:
        case CMSG_MOVE_SET_PITCH:
        case CMSG_MOVE_START_ASCEND:
        case CMSG_MOVE_START_BACKWARD:
        case CMSG_MOVE_START_DESCEND:
        case CMSG_MOVE_START_FORWARD:
        case CMSG_MOVE_START_PITCH_DOWN:
        case CMSG_MOVE_START_PITCH_UP:
        case CMSG_MOVE_START_STRAFE_LEFT:
        case CMSG_MOVE_START_STRAFE_RIGHT:
        case CMSG_MOVE_START_SWIM:
        case CMSG_MOVE_START_TURN_LEFT:
        case CMSG_MOVE_START_TURN_RIGHT:
        case CMSG_MOVE_STOP:
        case CMSG_MOVE_STOP_ASCEND:
        case CMSG_MOVE_STOP_PITCH:
        case CMSG_MOVE_STOP_STRAFE:
        case CMSG_MOVE_STOP_SWIM:
        case CMSG_MOVE_STOP_TURN:
        case SMSG_PLAYER_MOVE:
            return MovementStatus;

        case SMSG_MOVE_SET_CAN_FLY:
        case SMSG_MOVE_UNSET_CAN_FLY:
            return MoveSetUnsetCanFly;

        case SMSG_SPLINE_MOVE_SET_WALK_SPEED:
        case SMSG_SPLINE_MOVE_SET_RUN_SPEED:
        case SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED:
        case SMSG_SPLINE_MOVE_SET_SWIM_SPEED:
        case SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED:
        case SMSG_SPLINE_MOVE_SET_TURN_RATE:
        case SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED:
        case SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED:
        case SMSG_SPLINE_MOVE_SET_PITCH_RATE:
            return SplineMoveSetSpeed;

        case SMSG_MOVE_SET_WALK_SPEED:
            return MoveSetWalkSpeed;

        case SMSG_MOVE_SET_RUN_SPEED:
            return MoveSetRunSpeed;
        case SMSG_MOVE_SET_FLIGHT_SPEED:
            return MoveSetFlightSpeed;

        case SMSG_SPLINE_MOVE_SET_WALK_MODE:
        case SMSG_SPLINE_MOVE_SET_RUN_MODE:
        case SMSG_SPLINE_MOVE_GRAVITY_DISABLE:
        case SMSG_SPLINE_MOVE_GRAVITY_ENABLE:
        case SMSG_SPLINE_MOVE_SET_HOVER:
        case SMSG_SPLINE_MOVE_UNSET_HOVER:
        case SMSG_SPLINE_MOVE_START_SWIM:
        case SMSG_SPLINE_MOVE_STOP_SWIM:
        case SMSG_SPLINE_MOVE_SET_FLYING:
        case SMSG_SPLINE_MOVE_UNSET_FLYING:
        case SMSG_SPLINE_MOVE_SET_WATER_WALK:
        case SMSG_SPLINE_MOVE_SET_LAND_WALK:
        case SMSG_SPLINE_MOVE_SET_FEATHER_FALL:
        case SMSG_SPLINE_MOVE_SET_NORMAL_FALL:
        case SMSG_SPLINE_MOVE_ROOT:
        case SMSG_SPLINE_MOVE_UNROOT:
            return SplineMoveGuid;

        case SMSG_MOVE_ROOT:
        case SMSG_MOVE_UNROOT:
            return MoveRootUnroot;

        //case CMSG_DISMISS_CONTROLLED_VEHICLE:
        //case CMSG_FORCE_MOVE_ROOT_ACK:
        //case CMSG_FORCE_MOVE_UNROOT_ACK:
        //case CMSG_MOVE_CHNG_TRANSPORT:
        //case CMSG_MOVE_FALL_RESET:
        //case CMSG_MOVE_FEATHER_FALL_ACK:
        //case CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK:
        //case CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK:
        //case CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK:
        //case CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK:
        //case CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK:
        //case CMSG_MOVE_GRAVITY_DISABLE_ACK:
        //case CMSG_MOVE_GRAVITY_ENABLE_ACK:
        //case CMSG_MOVE_HOVER_ACK:
        //case CMSG_MOVE_KNOCK_BACK_ACK:
        //case CMSG_MOVE_SET_CAN_FLY_ACK:
        //case CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK:
        //case CMSG_MOVE_SET_COLLISION_HEIGHT_ACK:
        //case CMSG_MOVE_SPLINE_DONE:
        //case CMSG_MOVE_WATER_WALK_ACK:
        //case CMSG_PET_CAST_SPELL:
        //case CMSG_USE_ITEM:
        //case CMSG_MOVE_SET_RUN_MODE:
        //case CMSG_MOVE_SET_WALK_MODE:
        //case SMSG_MOVE_FEATHER_FALL:
        //case SMSG_MOVE_LAND_WALK:
        //case SMSG_MOVE_NORMAL_FALL:
        //case SMSG_MOVE_SET_COLLISION_HEIGHT:
        //case SMSG_MOVE_SET_FLIGHT_BACK_SPEED:
        //case SMSG_MOVE_SET_HOVER:
        //case SMSG_MOVE_SET_PITCH_RATE:
        //case SMSG_MOVE_SET_RUN_BACK_SPEED:
        //case SMSG_MOVE_SET_SWIM_BACK_SPEED:
        //case SMSG_MOVE_SET_SWIM_SPEED:
        //case SMSG_MOVE_SET_TURN_RATE:
        //case SMSG_MOVE_UNSET_HOVER:
        //case SMSG_MOVE_UPDATE_COLLISION_HEIGHT:
        //case SMSG_MOVE_UPDATE_FLIGHT_SPEED:
        //case SMSG_MOVE_UPDATE_KNOCK_BACK:
        //case SMSG_MOVE_UPDATE_RUN_BACK_SPEED:
        //case SMSG_MOVE_UPDATE_RUN_SPEED:
        //case SMSG_MOVE_UPDATE_SWIM_SPEED:
        //case SMSG_MOVE_UPDATE_TELEPORT:
        //case SMSG_MOVE_UPDATE_WALK_SPEED:
        //case SMSG_MOVE_WATER_WALK:
        default:
            break;
    }

    return NULL;
}