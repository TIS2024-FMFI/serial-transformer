#ifndef PROJECTST_ACTIVITYLOGMANAGER_H
#define PROJECTST_ACTIVITYLOGMANAGER_H

#include "ActivityLog.h"
#include <vector>
#include <algorithm>
#include <functional>

class ActivityLogManager {
public:
    void addLog(const ActivityLog& log);
    void removeLog(long actionTimestamp);
    ActivityLog* getLog(long actionTimestamp);

    std::vector<ActivityLog> filterLogs(std::function<bool(const ActivityLog&)> criteria) const;
    void sortLogsByTimestamp(bool ascending = true);
    void sortLogsByActionType(bool ascending = true);

private:
    std::vector<ActivityLog> logs;
};

#endif //PROJECTST_ACTIVITYLOGMANAGER_H