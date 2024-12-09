#ifndef PROJECTST_CONFIGURATION_H
#define PROJECTST_CONFIGURATION_H

#include <string>

class Configuration {
public:
    enum Mode { AUTOMATIC, MANUAL };
    enum TransformationModule { TELESCOPIC, UNIVERSAL };
    enum LoggingLevel { FULL, ERRORS_ONLY };
    enum YesNo { YES, NO };

    Configuration();

    Mode getMode() const;
    TransformationModule getTransformationModule() const;
    double getRa() const;
    double getDEC() const;
    bool isBypassMode() const;
    LoggingLevel getActivityLoggingLevel() const;
    YesNo getPacketLogging() const;

    void setMode(Mode Mode);
    void setTransformationModule(TransformationModule module);
    void setRA(double ra);
    void setDEC(double dec);
    void setBypassMode(bool bypassmode);
    void setActivityLoggingLevel(LoggingLevel loggingLevel);
    void setPacketLogging(YesNo packetlogging);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

private:
    Mode mode;
    TransformationModule transformationModule;
    double RA;
    double DEC;
    bool bypassMode;
    LoggingLevel activityLoggingLevel;
    YesNo packetLogging;
};

#endif //PROJECTST_CONFIGURATION_H