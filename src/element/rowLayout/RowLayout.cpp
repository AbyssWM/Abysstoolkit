#include "RowLayout.hpp"
#include <cmath>

#include "../../layout/Positioner.hpp"
#include "../../renderer/Renderer.hpp"
#include "../../core/InternalBackend.hpp"
#include "../../window/ToolkitWindow.hpp"

#include "../Element.hpp"
#include "../LinearLayout.hpp"

using namespace Abysstoolkit;

SP<CRowLayoutElement> CRowLayoutElement::create(const SRowLayoutData& data) {
    auto p          = SP<CRowLayoutElement>(new CRowLayoutElement(data));
    p->impl->self   = p;
    p->m_impl->self = p;
    return p;
}

CRowLayoutElement::CRowLayoutElement(const SRowLayoutData& data) : IElement(), m_impl(makeUnique<SRowLayoutImpl>()) {
    m_impl->data = data;
}

void CRowLayoutElement::paint() {
    ; // no-op
}

SP<CRowLayoutBuilder> CRowLayoutElement::rebuild() {
    auto p       = SP<CRowLayoutBuilder>(new CRowLayoutBuilder());
    p->m_self    = p;
    p->m_data    = makeUnique<SRowLayoutData>(m_impl->data);
    p->m_element = m_impl->self;
    return p;
}

void CRowLayoutElement::replaceData(const SRowLayoutData& data) {
    m_impl->data = data;

    if (impl->window)
        impl->window->scheduleReposition(impl->self);
}

void CRowLayoutElement::reposition(const Abyssutils::Math::CBox& sbox, const Abyssutils::Math::Vector2D& maxSize) {
    IElement::reposition(sbox);
    LinearLayout::reposition<true>(this, impl->position, impl->children, m_impl->data.gap, [this](SP<IElement> c) { return childSize(c); });
}

Abyssutils::Math::Vector2D CRowLayoutElement::childSize(Abyssutils::Memory::CSharedPointer<IElement> child) {
    if (child->preferredSize(impl->position.size()))
        return *child->preferredSize(impl->position.size());
    else if (child->minimumSize(impl->position.size()))
        return *child->minimumSize(impl->position.size());
    return {-1, -1};
}

Abyssutils::Math::Vector2D CRowLayoutElement::size() {
    return impl->position.size();
}

std::optional<Abyssutils::Math::Vector2D> CRowLayoutElement::preferredSize(const Abyssutils::Math::Vector2D& parent) {
    auto calc = m_impl->data.size.calculate(parent);

    if (calc.x != -1 && calc.y != -1)
        return calc;

    Vector2D max;
    for (const auto& child : impl->children) {
        max.x += childSize(child).x + m_impl->data.gap;
        max.y = std::max(max.y, childSize(child).y);
    }

    if (!impl->children.empty())
        max.x -= m_impl->data.gap;

    max.x += impl->margin * 2;
    max.y += impl->margin * 2;

    max.x = std::ceil(max.x);
    max.y = std::ceil(max.y);

    if (calc.x == -1)
        calc.x = max.x;
    if (calc.y == -1)
        calc.y = max.y;

    return calc;
}

std::optional<Abyssutils::Math::Vector2D> CRowLayoutElement::minimumSize(const Abyssutils::Math::Vector2D& parent) {
    Vector2D min;
    for (const auto& child : impl->children) {
        min.y = std::max(min.y, childSize(child).y);
    }

    return min;
}

bool CRowLayoutElement::positioningDependsOnChild() {
    return m_impl->data.size.hasAuto();
}
