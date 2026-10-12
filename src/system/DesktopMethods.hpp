#pragma once

#include <string>
#include <expected>

namespace Abysstoolkit::DesktopMethods {
    std::expected<void, std::string> openLink(const std::string_view& sv);
};