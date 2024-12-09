#include "PacketLog.h"

PacketLog::PacketLog(int record_id, long timestamp, const std::string& original_packet,
                     const std::string& transformed_packet) : record_id(record_id), timestamp(timestamp),
                     original_packet(original_packet), transformed_packet(transformed_packet) {}

int PacketLog::getRecordID() const {
    return record_id;
}

long PacketLog::getTimestamp() const {
    return timestamp;
}

std::string PacketLog::getOriginalPacket() const {
    return original_packet;
}

std::string PacketLog::getTransformedPacket() const {
    return transformed_packet;
}

void PacketLog::setTransformedPacket(const std::string& transformed_Packet) {
    this->transformed_packet = transformed_Packet;
}