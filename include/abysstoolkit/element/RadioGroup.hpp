#pragma once

#include "Checkbox.hpp"

#include <abyssutils/memory/SharedPtr.hpp>
#include <abyssutils/memory/WeakPtr.hpp>

#include <functional>
#include <vector>

namespace Abysstoolkit {

    // Coordinates exclusivity across a set of radio-styled CCheckboxElements.
    // Layout and sizing are left to the caller; the group only enforces that
    // toggling one radio on turns the others off.
    class CRadioGroup {
      public:
        static Abyssutils::Memory::CSharedPointer<CRadioGroup> create();
        ~CRadioGroup()                                        = default;

        void                                                  add(Abyssutils::Memory::CSharedPointer<CCheckboxElement> radio);
        Abyssutils::Memory::CSharedPointer<CCheckboxElement>   selected();
        void                                                  setSelected(Abyssutils::Memory::CSharedPointer<CCheckboxElement> radio);
        void                                                  onSelected(std::function<void(Abyssutils::Memory::CSharedPointer<CCheckboxElement>)>&&);

      private:
        CRadioGroup() = default;

        std::vector<Abyssutils::Memory::CWeakPointer<CCheckboxElement>>      m_radios;
        std::function<void(Abyssutils::Memory::CSharedPointer<CCheckboxElement>)> m_onSelected;
        Abyssutils::Memory::CWeakPointer<CRadioGroup>                        m_self;
    };
};
