#pragma once

#include <iostream>
#include <memory>
#include <syslog.h>

namespace nopayloadclient {
namespace logging {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};


class ILogger {
public:
    virtual ~ILogger() {}
    virtual void log(LogLevel level, const std::string& message) = 0;

    void setLogLevel(LogLevel level) {
        log_level_ = level;
    }

    void setLogLevel(const std::string& level_string) {
        LogLevel level = stringToLogLevel(level_string);
        setLogLevel(level);
    }

protected:
    LogLevel log_level_ = LogLevel::INFO; // Default log level
    std::string mode_ = "terminal"; // one of [terminal, file, syslog]

    std::string logLevelToString(LogLevel level) {
        switch(level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    LogLevel stringToLogLevel(const std::string& level_string) {
        if (level_string == "DEBUG") return LogLevel::DEBUG;
        else if (level_string == "INFO") return LogLevel::INFO;
        else if (level_string == "WARNING") return LogLevel::WARNING;
        else if (level_string == "ERROR") return LogLevel::ERROR;
        throw std::invalid_argument("Invalid LogLevel string");
    }
};


class TerminalLogger : public ILogger {
public:
    void log(LogLevel level, const std::string& message) override {
        if (level < log_level_) return; // Don't log messages below the set log level
        std::cout << "[" + logLevelToString(level) + "] " + message << std::endl;
    }
};


class SyslogLogger : public ILogger {
private:
    bool is_open_ = false;

public:
    ~SyslogLogger() {
        if (is_open_) closelog();
    }

    void log(LogLevel level, const std::string& message) override {
        if (!is_open_) {
            openlog("nopayloadclient", LOG_PID | LOG_CONS, LOG_USER);
            is_open_ = true;
        }
        int sys_log_level;
        switch(level) {
            case LogLevel::DEBUG: sys_log_level = LOG_DEBUG; break;
            case LogLevel::INFO: sys_log_level = LOG_INFO; break;
            case LogLevel::WARNING: sys_log_level = LOG_WARNING; break;
            case LogLevel::ERROR: sys_log_level = LOG_ERR; break;
            default: sys_log_level = LOG_WARNING; break;
        }
        syslog(sys_log_level, "%s", message.c_str());
    }
};


class LoggerManager {
private:
    static std::unique_ptr<ILogger>& getLoggerInstance() {
        static std::unique_ptr<ILogger> logger = std::make_unique<TerminalLogger>();
        return logger;
    }

public:
    static void setLogger(std::unique_ptr<ILogger> newLogger) {
        getLoggerInstance() = std::move(newLogger);
    }

    static ILogger& getLogger() {
        return *getLoggerInstance();
    }
};


inline void info(const std::string& msg) {
    LoggerManager::getLogger().log(LogLevel::INFO, msg);
}
inline void warning(const std::string& msg) {
    LoggerManager::getLogger().log(LogLevel::WARNING, msg);
}
inline void error(const std::string& msg) {
    LoggerManager::getLogger().log(LogLevel::ERROR, msg);
}
inline void debug(const std::string& msg) {
    LoggerManager::getLogger().log(LogLevel::DEBUG, msg);
}

inline void setLogger(const std::string& name) {
    if (name == "terminal") {
        LoggerManager::setLogger(std::make_unique<TerminalLogger>());
    }
    else if (name == "syslog") {
        LoggerManager::setLogger(std::make_unique<SyslogLogger>());
    }
    else {
        throw std::invalid_argument("Invalid logger choice");
    }
}

inline void setLogLevel(const std::string& level_string) {
    LoggerManager::getLogger().setLogLevel(level_string);
}

} // logging namespace
} // nopayloadclient namespace
