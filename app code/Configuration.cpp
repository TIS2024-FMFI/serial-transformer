#include "Configuration.h"

Configuration::Configuration()
        : mode(AUTOMATIC), transformationModule(TELESCOPIC), RA(0.0), DEC(0.0),
        bypassMode(false), activityLoggingLevel(FULL), packetLogging(YES) {}

Configuration::Mode Configuration::getMode() const {
    return mode;
}

Configuration::TransformationModule Configuration::getTransformationModule() const {
    return transformationModule;
}

double Configuration::getRa() const {
    return RA;
}

double Configuration::getDEC() const {
    return DEC;
}

bool Configuration::isBypassMode() const {
    return bypassMode;
}

Configuration::LoggingLevel Configuration::getActivityLoggingLevel() const {
    return activityLoggingLevel;
}

Configuration::YesNo Configuration::getPacketLogging() const {
    return packetLogging;
}

void Configuration::setMode(Configuration::Mode Mode) {
    this->mode = Mode;
}

void Configuration::setTransformationModule(Configuration::TransformationModule module) {
    this->transformationModule = module;
}

void Configuration::setRA(double ra) {
    this->RA = ra;
}

void Configuration::setDEC(double dec) {
    this->DEC = dec;
}

void Configuration::setBypassMode(bool bypassmode) {
    this->bypassMode = bypassmode;
}

void Configuration::setActivityLoggingLevel(Configuration::LoggingLevel loggingLevel) {
    this->activityLoggingLevel = loggingLevel;
}

void Configuration::setPacketLogging(Configuration::YesNo packetlogging) {
    this->packetLogging = packetlogging;
}

bool Configuration::saveToFile(const std::string& filename) const {
    // TODO !!!
    return false;
}

bool Configuration::loadFromFile(const std::string& filename) {
    // TODO !!!
    return false;
}