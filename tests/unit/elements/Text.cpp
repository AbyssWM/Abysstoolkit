#include <gtest/gtest.h>

#include <element/text/Text.hpp>
#include <abysstoolkit/core/Backend.hpp>

#include "../tricks/Tricks.hpp"

using namespace Abysstoolkit;

TEST(Element, text) {
    Tests::Tricks::createBackendSupport();

    auto text = CTextBuilder::begin()->text(R"(Hello <a href="https://github.com/AbyssWM">link</a>! Hi <a href="https://github.com/AbyssWM">link2</a>!)")->commence();

    EXPECT_EQ(text->m_impl->parsedText, "Hello <u><span foreground=\"#4eecf8ff\">link</span></u>! Hi <u><span foreground=\"#4eecf8ff\">link2</span></u>!");

    text.reset();
}