#include "ActivityLog.h"

ActivityLog::ActivityLog(long actionTimestamp, const std::string& actionType,
                 const std::string& actionParameters) : actionTimestamp(actionTimestamp),
                 actionType(actionType), actionParameters(actionParameters) {}

long ActivityLog::getActionTimestamp() const {
    return actionTimestamp;
}

std::string ActivityLog::getActionType() const {
    return actionType;
}

std::string ActivityLog::getActionParameters() const {
    return actionParameters;
}

void ActivityLog::setActionParameters(const std::string& actionparameters) {
    this->actionParameters = actionparameters;
}