#ifndef PROJECTST_ACTIVITYLOG_H
#define PROJECTST_ACTIVITYLOG_H

#include <string>

class ActivityLog {
public:
    ActivityLog(long actionTimestamp, const std::string& actionType, const std::string& actionParameters = "");

    long getActionTimestamp() const;
    std::string getActionType() const;
    std::string getActionParameters() const;

    void setActionParameters(const std::string& actionparameters);

private:
    long actionTimestamp;
    std::string actionType;
    std::string actionParameters;
};

#endif //PROJECTST_ACTIVITYLOG_H