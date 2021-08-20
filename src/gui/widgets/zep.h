/***************************************************************************
 *   Copyright (C) 2021 PCSX-Redux authors                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/

#pragma once

#include <optional>

#include "zep/filesystem.h"
#include "zep/imgui/display_imgui.h"
#include "zep/imgui/editor_imgui.h"
#include "zep/mode_standard.h"
#include "zep/mode_vim.h"
#include "zep/tab_window.h"
#include "zep/theme.h"
#include "zep/window.h"

namespace PCSX {
namespace Widgets {

static Zep::NVec2f GetPixelScale() { return Zep::NVec2f(2.25f); }

class ZepEditor final : public Zep::IZepComponent {
  public:
    ZepEditor(const std::string &name)
        : m_editor(std::make_unique<Zep::ZepEditor_ImGui>(Zep::ZepPath(""), GetPixelScale())) {
        m_editor->RegisterCallback(this);

        m_editor->InitWithText(name, "");
        m_editor->SetGlobalMode(Zep::ZepMode_Standard::StaticName());
    }

    virtual ~ZepEditor() {}

    void Destroy() {
        m_editor->UnRegisterCallback(this);
        m_editor.reset();
    }

    bool draw();

    virtual Zep::ZepEditor &GetEditor() const override final { return *m_editor; }

    void setText(const std::string &str) {
        auto buffer = m_editor->GetMRUBuffer();
        buffer->SetText(str);
    }

    std::string getText() {
        auto buffer = m_editor->GetMRUBuffer();
        return buffer->GetBufferText(buffer->Begin(), buffer->End());
    }

    bool hasTextChanged() {
        auto currentTime = m_editor->GetMRUBuffer()->GetLastUpdateTime();
        if (m_lastUpdateTime.has_value() && (m_lastUpdateTime.value() == currentTime)) {
            return false;
        }
        m_lastUpdateTime = currentTime;
        return true;
    }

  private:
    std::unique_ptr<Zep::ZepEditor_ImGui> m_editor;
    std::optional<decltype(m_editor->GetMRUBuffer()->GetLastUpdateTime())> m_lastUpdateTime;
};

}  // namespace Widgets
}  // namespace PCSX
