/*
 *  Copyright (C) 2026 Nainazi
 *  Copyright (C) 2020 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NainaziStyle.h"

#include <QDialog>
#include <QFile>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

NainaziStyle::NainaziStyle()
    : BaseStyle()
{
#ifdef Q_OS_MACOS
    // Keep the pastel toolbar instead of the native macOS bar.
    m_drawNativeMacOsToolBar = false;
#endif
}

QPalette NainaziStyle::standardPalette() const
{
    auto palette = BaseStyle::standardPalette();

    palette.setColor(QPalette::Active, QPalette::Window, QRgb(0xF6F0FB));
    palette.setColor(QPalette::Inactive, QPalette::Window, QRgb(0xFBF7FD));
    palette.setColor(QPalette::Disabled, QPalette::Window, QRgb(0xEFE6F4));

    palette.setColor(QPalette::Active, QPalette::WindowText, QRgb(0x3E2A4A));
    palette.setColor(QPalette::Inactive, QPalette::WindowText, QRgb(0x4A3558));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QRgb(0xA898B4));

    palette.setColor(QPalette::Active, QPalette::Text, QRgb(0x3E2A4A));
    palette.setColor(QPalette::Inactive, QPalette::Text, QRgb(0x4A3558));
    palette.setColor(QPalette::Disabled, QPalette::Text, QRgb(0xA898B4));

    palette.setColor(QPalette::Active, QPalette::PlaceholderText, QRgb(0x8A7596));
    palette.setColor(QPalette::Inactive, QPalette::PlaceholderText, QRgb(0xA08CAF));
    palette.setColor(QPalette::Disabled, QPalette::PlaceholderText, QRgb(0xC4B6CC));

    palette.setColor(QPalette::Active, QPalette::BrightText, QRgb(0xFFF8FC));
    palette.setColor(QPalette::Inactive, QPalette::BrightText, QRgb(0xFFF8FC));
    palette.setColor(QPalette::Disabled, QPalette::BrightText, QRgb(0xF3E8F4));

    palette.setColor(QPalette::Active, QPalette::Base, QRgb(0xFFFBFE));
    palette.setColor(QPalette::Inactive, QPalette::Base, QRgb(0xFFFBFE));
    palette.setColor(QPalette::Disabled, QPalette::Base, QRgb(0xF6F0F8));

    palette.setColor(QPalette::Active, QPalette::AlternateBase, QRgb(0xFBEFF5));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QRgb(0xFBF4F8));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QRgb(0xF3E8F0));

    palette.setColor(QPalette::All, QPalette::ToolTipBase, QRgb(0x5C3D72));
    palette.setColor(QPalette::All, QPalette::ToolTipText, QRgb(0xFFF8FC));

    palette.setColor(QPalette::Active, QPalette::Button, QRgb(0xEFE2F6));
    palette.setColor(QPalette::Inactive, QPalette::Button, QRgb(0xF4EAF8));
    palette.setColor(QPalette::Disabled, QPalette::Button, QRgb(0xF6F0F8));

    palette.setColor(QPalette::Active, QPalette::ButtonText, QRgb(0x4A3558));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, QRgb(0x5C466C));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QRgb(0xA898B4));

    palette.setColor(QPalette::Active, QPalette::Highlight, QRgb(0xF3B4D0));
    palette.setColor(QPalette::Inactive, QPalette::Highlight, QRgb(0xF6D0E2));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QRgb(0xF8E4EE));

    palette.setColor(QPalette::Active, QPalette::HighlightedText, QRgb(0x4A2340));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QRgb(0x5A3050));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QRgb(0xA898B4));

    palette.setColor(QPalette::All, QPalette::Light, QRgb(0xFFFBFE));
    palette.setColor(QPalette::All, QPalette::Midlight, QRgb(0xF3E6F6));
    palette.setColor(QPalette::All, QPalette::Mid, QRgb(0xE3D0EE));
    palette.setColor(QPalette::All, QPalette::Dark, QRgb(0xC9B0D8));
    palette.setColor(QPalette::All, QPalette::Shadow, QRgb(0x8A7596));

    palette.setColor(QPalette::All, QPalette::Link, QRgb(0xA85A8C));
    palette.setColor(QPalette::Disabled, QPalette::Link, QRgb(0xC4A0B6));
    palette.setColor(QPalette::All, QPalette::LinkVisited, QRgb(0x8B5A9A));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited, QRgb(0xB8A0C0));

    return palette;
}

QString NainaziStyle::getAppStyleSheet() const
{
    QFile extStylesheetFile(QStringLiteral(":/styles/nainazi/nainazistyle.qss"));
    if (extStylesheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return extStylesheetFile.readAll();
    }
    qWarning("Failed to load 奈娜子 theme stylesheet.");
    return {};
}

void NainaziStyle::polish(QWidget* widget)
{
    if (qobject_cast<QMainWindow*>(widget) || qobject_cast<QDialog*>(widget) || qobject_cast<QMenuBar*>(widget)
        || qobject_cast<QToolBar*>(widget) || qobject_cast<QStatusBar*>(widget)) {
        auto palette = widget->palette();
        palette.setColor(QPalette::Active, QPalette::Window, QRgb(0xF3E8F8));
        palette.setColor(QPalette::Inactive, QPalette::Window, QRgb(0xF7F1FB));
        palette.setColor(QPalette::Disabled, QPalette::Window, QRgb(0xEFE6F4));
        widget->setPalette(palette);
    }
}
