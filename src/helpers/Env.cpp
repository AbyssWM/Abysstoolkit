#include "Env.hpp"

#include <cstdlib>
#include <optional>
#include <string_view>

using namespace Abysstoolkit;
using namespace Abysstoolkit::Env;

bool Abysstoolkit::Env::envEnabled(const std::string& env) {
    auto ret = getenv(env.c_str());
    if (!ret)
        return false;

    const std::string_view sv = ret;

    return !sv.empty() && sv != "0";
}

bool Abysstoolkit::Env::isTrace() {
    static bool TRACE = envEnabled("HT_TRACE");
    return TRACE;
}

std::optional<std::string_view> Hyprtoolkit::Env::getEnv(const std::string& env) {
    auto ret = getenv(env.c_str());
    if (!ret)
        return std::nullopt;

    return ret;
}
