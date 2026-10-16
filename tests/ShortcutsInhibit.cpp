#include <abysstoolkit/core/Backend.hpp>
#include <abysstoolkit/window/Window.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/Text.hpp>

#include <abyssutils/memory/SharedPtr.hpp>

#include <print>
#include <string_view>

using namespace Abyssutils::Memory;
using namespace Abyssutils::Math;
using namespace Abysstoolkit;

#define SP CSharedPointer

static SP<IBackend> backend;

// focus this window, then press a keybind bound to e.g. cyclenext. with the inhibitor on, focus
// must not move. pass "off" for the negative control (focus should cycle as normal).
int main(int argc, char** argv) {
    const bool inhibit = !(argc > 1 && std::string_view{argv[1]} == "off");

    backend = IBackend::create();

    auto window =
        CWindowBuilder::begin()->preferredSize({440, 160})->appTitle("shortcuts inhibit test")->appClass("abysstoolkit-shortcuts-inhibit")->inhibitShortcuts(inhibit)->commence();

    window->m_rootElement->addChild(CRectangleBuilder::begin()->color([] { return CAbyssColor{0.1F, 0.1F, 0.1F}; })->commence());
    window->m_rootElement->addChild(CTextBuilder::begin()
                                        ->text(inhibit ? "inhibit ON: keybinds should not fire while focused" : "inhibit OFF (control): keybinds fire")
                                        ->color([] { return CAbyssColor{0.9F, 0.9F, 0.9F}; })
                                        ->commence());

    std::println("shortcuts inhibitor requested: {}", inhibit ? "ON" : "OFF (control)");

    window->open();
    backend->enterLoop();
    return 0;
}
