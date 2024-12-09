#include "ActivityLogManager.h"

void ActivityLogManager::addLog(const ActivityLog& log) {
    logs.push_back(log);
}

void ActivityLogManager::removeLog(long actionTimestamp) {
    logs.erase(std::remove_if(logs.begin(), logs.end(),
                  [actionTimestamp](const ActivityLog& log) {
        return log.getActionTimestamp() == actionTimestamp;
    }), logs.end());
}

ActivityLog* ActivityLogManager::getLog(long actionTimestamp) {
    auto it = std::find_if(logs.begin(), logs.end(),
                   [actionTimestamp](const ActivityLog& log) {
        return log.getActionTimestamp() == actionTimestamp;
    });
    return it != logs.end() ? &(*it) : nullptr;
}

std::vector<ActivityLog> ActivityLogManager::filterLogs(std::function<bool(const ActivityLog&)> criteria) const {
    std::vector<ActivityLog> filteredLogs;
    std::copy_if(logs.begin(), logs.end(), std::back_inserter(filteredLogs), criteria);
    return filteredLogs;
}

void ActivityLogManager::sortLogsByTimestamp(bool ascending) {
    std::sort(logs.begin(), logs.end(),
              [ascending](const ActivityLog& a, const ActivityLog& b) {
        return ascending ? a.getActionTimestamp() < b.getActionTimestamp() :
                            a.getActionTimestamp() > b.getActionTimestamp();
    });
}

void ActivityLogManager::sortLogsByActionType(bool ascending) {
    std::sort(logs.begin(), logs.end(),
              [ascending](const ActivityLog& a, const ActivityLog& b) {
        return ascending ? a.getActionType() < b.getActionType() : a.getActionType() > b.getActionType();
    });
}