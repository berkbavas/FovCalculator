
#include "Logger.h"

void FovCalculator::Logger::Log(LogLevel level, const std::string& message)
{
    thread_local const auto TL_THREAD_ID = mLastThreadId.fetch_add(1);

    const auto log = std::format("{:.12} [{:}] [ {:5} ] {:}", GetTimeString(), TL_THREAD_ID, GetLogLevelString(level), message);

    switch (level)
    {
        case LogLevel::TRACE:
        case LogLevel::DEBUG:

        case LogLevel::INFO:
            std::cout << log << std::endl;
            break;
        case LogLevel::WARNING:
        case LogLevel::FATAL:
            std::cerr << log << std::endl;
            break;
        default:
            break;
    }
}

void FovCalculator::Logger::SetLogLevel(LogLevel logLevel)
{
    mLogLevel = logLevel;
}

FovCalculator::LogLevel FovCalculator::Logger::GetLogLevel()
{
    return mLogLevel;
}

std::string FovCalculator::Logger::GetTimeString()
{
    const auto zone = std::chrono::current_zone();
    const auto now = std::chrono::system_clock::now();
    const auto local = std::chrono::zoned_time(zone, now);

    return std::format("{:%T}", local);
}

void FovCalculator::Logger::QtMessageOutputCallback(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    switch (type)
    {
        case QtDebugMsg:
            LOG_DEBUG("{}", msg.toStdString());
            break;
        case QtInfoMsg:
            LOG_INFO("{}", msg.toStdString());
            break;
        case QtWarningMsg:
            LOG_WARN("{}", msg.toStdString());
            break;
        case QtCriticalMsg:
            LOG_FATAL("{}", msg.toStdString());
            break;
        case QtFatalMsg:
            LOG_FATAL("{}", msg.toStdString());
            break;
    }
}

bool FovCalculator::Logger::isLogEnabledFor(LogLevel level)
{
    return mLogLevel <= level;
}

FovCalculator::LogLevel FovCalculator::Logger::mLogLevel = FovCalculator::LogLevel::ALL;

std::atomic_uint32_t FovCalculator::Logger::mLastThreadId = 0;