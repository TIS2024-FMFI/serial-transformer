#ifndef PROJECTST_PACKETLOGMANAGER_H
#define PROJECTST_PACKETLOGMANAGER_H

#include "PacketLog.h"
#include <vector>
#include <algorithm>
#include <functional>

class PacketLogManager {
public:
    void addLog(const PacketLog& log);
    void removeLog(int record_id);
    PacketLog* getLog(int record_id);

    std::vector<PacketLog> filterLogs(std::function<bool(const PacketLog&)> criteria) const;
    void sortLogsByTimestamp(bool ascending = true);
    void sortLogsByRecordID(bool ascending = true);

private:
    std::vector<PacketLog> logs;
};

#endif //PROJECTST_PACKETLOGMANAGER_H