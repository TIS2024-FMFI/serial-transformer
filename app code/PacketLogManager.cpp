#include "PacketLogManager.h"

void PacketLogManager::addLog(const PacketLog& log) {
    logs.push_back(log);
}

void PacketLogManager::removeLog(int record_id) {
    logs.erase(std::remove_if(logs.begin(), logs.end(),
                  [record_id](const PacketLog& log) {
        return log.getRecordID() == record_id;
    }), logs.end());
}

PacketLog* PacketLogManager::getLog(int record_id) {
    auto it = std::find_if(logs.begin(), logs.end(),
                   [record_id](const PacketLog& log) {
        return log.getRecordID() == record_id;
    });
    return it != logs.end() ? &(*it) : nullptr;
}

std::vector<PacketLog> PacketLogManager::filterLogs(std::function<bool(const PacketLog&)> criteria) const {
    std::vector<PacketLog> filteredLogs;
    std::copy_if(logs.begin(), logs.end(), std::back_inserter(filteredLogs), criteria);
    return filteredLogs;
}

void PacketLogManager::sortLogsByTimestamp(bool ascending) {
    std::sort(logs.begin(), logs.end(), [ascending](const PacketLog& a, const PacketLog& b) {
        return ascending ? a.getTimestamp() < b.getTimestamp() : a.getTimestamp() > b.getTimestamp();
    });
}

void PacketLogManager::sortLogsByRecordID(bool ascending) {
    std::sort(logs.begin(), logs.end(), [ascending](const PacketLog& a, const PacketLog& b) {
        return ascending ? a.getRecordID() < b.getRecordID() : a.getRecordID() > b.getRecordID();
    });
}