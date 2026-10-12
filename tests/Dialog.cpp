#include <abysstoolkit/core/Backend.hpp>
#include <abysstoolkit/window/Window.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/RowLayout.hpp>
#include <abysstoolkit/element/ColumnLayout.hpp>
#include <abysstoolkit/element/Text.hpp>
#include <abysstoolkit/element/Image.hpp>
#include <abysstoolkit/element/Button.hpp>
#include <abysstoolkit/element/Null.hpp>

#include <abyssutils/memory/SharedPtr.hpp>
#include <abyssutils/memory/UniquePtr.hpp>

#include <print>

using namespace Abyssutils::Memory;
using namespace Abyssutils::Math;
using namespace Abysstoolkit;

#define SP CSharedPointer
#define WP CWeakPointer
#define UP CUniquePointer

static SP<IBackend> backend;

int                 main(int argc, char** argv, char** envp) {
    backend = IBackend::create();

    //
    auto window = CWindowBuilder::begin()->preferredSize({480, 180})->minSize({480, 180})->maxSize({480, 180})->appTitle("Dialog")->appClass("abysstoolkit-dialog")->commence();

    window->m_rootElement->addChild(CRectangleBuilder::begin()->color([] { return backend->getPalette()->m_colors.background; })->commence());

    auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})->commence();
    layout->setMargin(3);

    auto layoutInner = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {0.85F, 1.F}})->commence();

    window->m_rootElement->addChild(layout);

    layout->addChild(layoutInner);
    layoutInner->setGrow(true);
    layoutInner->setPositionMode(Abysstoolkit::IElement::HT_POSITION_ABSOLUTE);
    layoutInner->setPositionFlag(Abysstoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);

    auto title = CTextBuilder::begin()->text("Hello World")->fontSize({CFontSize::HT_FONT_H2})->color([] { return backend->getPalette()->m_colors.text; })->commence();

    auto hr = CRectangleBuilder::begin() //
                  ->color([] { return CAbyssColor{backend->getPalette()->m_colors.text.darken(0.65)}; })
                  ->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_ABSOLUTE, {0.5F, 9.F}})
                  ->commence();

    hr->setMargin(4);

    auto content = CTextBuilder::begin()
                       ->text("This is an example dialog. This first line is long on purpose, so that we overflow. Amogus sussus biggus sussus.\n\nWoo!")
                       ->align(Abysstoolkit::HT_FONT_ALIGN_CENTER)
                       ->color([] { return backend->getPalette()->m_colors.text; })
                       ->commence();
    content->setPositionFlag(Abysstoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);

    auto button1 = CButtonBuilder::begin()
                       ->label("Exit")
                       ->onMainClick([w = WP<IWindow>{window}](SP<CButtonElement> el) {
                           w->close();
                           backend->destroy();
                       })
                       ->size({CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                       ->commence();

    auto button2 = CButtonBuilder::begin()
                       ->label("Do something")
                       ->onMainClick([](SP<CButtonElement> el) { std::println("Did something!"); })
                       ->size({CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                       ->commence();

    auto null2 = CNullBuilder::begin()->commence();

    auto layout2 = CRowLayoutBuilder::begin()->gap(3)->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();

    null2->setGrow(true);

    layoutInner->addChild(title);
    layoutInner->addChild(hr);
    layoutInner->addChild(content);

    layout2->addChild(null2);
    layout2->addChild(button2);
    layout2->addChild(button1);

    layout->addChild(layout2);

    window->m_events.closeRequest.listenStatic([w = WP<IWindow>{window}] {
        w->close();
        backend->destroy();
    });

    window->open();

    backend->enterLoop();

    return 0;
}