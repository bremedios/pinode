//
// Created by Bradley Remedios on 11/25/24.
//
#ifndef PINODE_PACKETOPS_H_
#define PINODE_PACKETOPS_H_

namespace pinode {
    enum PacketOpType : int {
        PacketOp_NONE = 0,
        PacketOp_GET_CAPS =1,
        PacketOp_CAPS=2,
        PacketOp_GET_TEMPERATURE =3,
        PacketOp_TEMPERATURE=4,
        PacketOp_GET_HUMIDITY =5,
        PacketOp_HUMIDITY=6,
        PacketOp_GET_INFO =7,
        PacketOp_INFO=8,
    }; // PacketOpType

    struct PacketGetCaps {
        uint16_t type;
        uint16_t len;
    };

    struct PacketCaps {
        uint16_t type;
        uint16_t len;
        uint32_t hasTemp :1;
        uint32_t hasHumidity :1;
        uint32_t reserved1: 30;
    };

    struct PacketGetTemperature {
        uint16_t type;
        uint16_t len;
    };

    struct PacketTemperature {
        uint16_t type;
        uint16_t len;
        int32_t temperature;
    };

    struct PacketGetHumidity {
        uint16_t type;
        uint16_t len;
    };

    struct PacketHumidity {
        uint16_t type;
        uint16_t len;
        int32_t humidity;
    };
}; // namespace pinode
#endif //PINODE_PACKETOPS_H_