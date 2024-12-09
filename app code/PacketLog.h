#ifndef PROJECTST_PACKETLOG_H
#define PROJECTST_PACKETLOG_H
#include <string>

class PacketLog {
public:
    PacketLog(int record_id, long timestamp, const std::string& original_packet,
                const std::string& transformed_packet = "");

    int getRecordID() const;
    long getTimestamp() const;
    std::string getOriginalPacket() const;
    std::string getTransformedPacket() const;

    void setTransformedPacket(const std::string& transformed_packet);

private:
    int record_id;
    long timestamp;  // timestamp in milliseconds
    std::string original_packet;
    std::string transformed_packet;
};

#endif //PROJECTST_PACKETLOG_H