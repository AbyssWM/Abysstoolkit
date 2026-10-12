#include "Logger.hpp"

using namespace Abysstoolkit;

static Abyssutils::CLI::eLogLevel levelToHU(eLogLevel l) {
    switch (l) {
        case Abysstoolkit::HT_LOG_DEBUG: return Abyssutils::CLI::LOG_DEBUG;
        case Abysstoolkit::HT_LOG_ERROR: return Abyssutils::CLI::LOG_ERR;
        case Abysstoolkit::HT_LOG_WARNING: return Abyssutils::CLI::LOG_WARN;
        case Abysstoolkit::HT_LOG_CRITICAL: return Abyssutils::CLI::LOG_CRIT;
        case Abysstoolkit::HT_LOG_TRACE: return Abyssutils::CLI::LOG_TRACE;
    }
    return Abyssutils::CLI::LOG_DEBUG;
}

CLogger::CLogger() {
    const auto IS_TRACE = Env::isTrace();
    m_logger.setLogLevel(IS_TRACE ? Abyssutils::CLI::LOG_TRACE : Abyssutils::CLI::LOG_DEBUG);
}

void CLogger::updateLogLevel() {
    const auto IS_TRACE = Env::isTrace();
    if (m_loggerConnection && IS_TRACE)
        m_loggerConnection->setLogLevel(Abyssutils::CLI::LOG_TRACE);
}

void CLogger::log(eLogLevel level, const std::string& str) {
    static const bool IS_QUIET = Env::envEnabled("HT_QUIET");
    if (IS_QUIET)
        return;

    if (m_logFn) {
        m_logFn(level, str);
        return;
    }

    if (m_loggerConnection) {
        m_loggerConnection->log(levelToHU(level), str);
        return;
    }

    m_logger.log(levelToHU(level), str);
}
