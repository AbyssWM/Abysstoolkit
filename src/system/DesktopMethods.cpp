#include "DesktopMethods.hpp"

#include <abyssutils/os/Process.hpp>

using namespace Abysstoolkit;
using namespace Abysstoolkit::DesktopMethods;

using namespace Abyssutils::OS;

std::expected<void, std::string> DesktopMethods::openLink(const std::string_view& sv) {
    CProcess proc("xdg-open", {std::string{sv}});

    if (!proc.runAsync())
        return std::unexpected("Failed to spawn process");

    return {};
}
