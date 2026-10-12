#pragma once

#include "IWaylandWindow.hpp"
namespace Abysstoolkit {

    class CWaylandWindow : public IWaylandWindow {
      public:
        CWaylandWindow(const SWindowCreationData& data);
        virtual ~CWaylandWindow();

        virtual void close();
        virtual void open();

      private:
        SWindowCreationData m_creationData;

        friend class CWaylandPlatform;
        friend class CWaylandPopup;
    };
};