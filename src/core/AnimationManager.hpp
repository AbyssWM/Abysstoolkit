#pragma once

#include <abyssutils/animation/AnimationManager.hpp>
#include <abyssutils/animation/AnimatedVariable.hpp>
#include <abysstoolkit/core/Animation.hpp>

#include <vector>

#include "AnimatedVariable.hpp"

namespace Abysstoolkit {
    class CHTAnimationManager : public Abyssutils::Animation::CAnimationManager {
      public:
        CHTAnimationManager();

        void         tick();
        virtual void scheduleTick();
        virtual void onTicked();

        using SAnimationPropertyConfig = Abyssutils::Animation::SAnimationPropertyConfig;

        SP<SAnimationPropertyConfig> configFor(const SAnimation& animation);

        template <Animable VarType>
        void createAnimation(const VarType& v, PHLANIMVAR<VarType>& pav, SP<SAnimationPropertyConfig> pConfig) {
            constexpr const eAnimatedVarType EAVTYPE = typeToeAnimatedVarType<VarType>;
            pav                                      = makeUnique<CAnimatedVariable<VarType>>();

            pav->create2(EAVTYPE, static_cast<Abyssutils::Animation::CAnimationManager*>(this), pav, v);
            pav->setConfig(pConfig);
        }

        Abyssutils::Animation::CAnimationConfigTree m_animationTree;

      private:
        std::vector<std::pair<SAnimation, SP<SAnimationPropertyConfig>>> m_dynamicConfigs;
        size_t                                                           m_nextCurveID = 0;
    };

    inline SP<CHTAnimationManager> g_animationManager;
}
