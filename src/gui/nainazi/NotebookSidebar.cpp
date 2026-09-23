/*
 *  Copyright (C) 2026 Nainazi
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NotebookSidebar.h"

#include "core/Database.h"
#include "core/Entry.h"
#include "core/Group.h"
#include "core/Metadata.h"
#include "gui/Icons.h"
#include "gui/group/GroupView.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

namespace
{
    QListWidgetItem* smartItem(QListWidget* list, const QString& text, const QString& icon, NotebookView view)
    {
        auto* item = new QListWidgetItem(icons()->icon(icon), text, list);
        item->setData(Qt::UserRole, static_cast<int>(view));
        item->setSizeHint({0, 36});
        return item;
    }
} // namespace

NotebookSidebar::NotebookSidebar(GroupView* groupView, QWidget* parent)
    : QWidget(parent)
    , m_groupView(groupView)
    , m_iconLabel(new QLabel(this))
    , m_nameLabel(new QLabel(this))
    , m_countLabel(new QLabel(this))
    , m_smartList(new QListWidget(this))
{
    setObjectName(QStringLiteral("notebookSidebar"));
    setMinimumWidth(220);
    setMaximumWidth(280);

    m_iconLabel->setPixmap(icons()->applicationIcon().pixmap(48, 48));
    m_iconLabel->setFixedSize(48, 48);

    auto nameFont = m_nameLabel->font();
    nameFont.setBold(true);
    nameFont.setPointSizeF(nameFont.pointSizeF() + 1);
    m_nameLabel->setFont(nameFont);
    m_nameLabel->setObjectName(QStringLiteral("notebookDbName"));
    m_countLabel->setObjectName(QStringLiteral("notebookDbCount"));
    m_countLabel->setWordWrap(true);

    m_smartList->setObjectName(QStringLiteral("notebookSmartList"));
    m_smartList->setFrameShape(QFrame::NoFrame);
    m_smartList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_smartList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_smartList->setFocusPolicy(Qt::NoFocus);
    m_smartList->setFixedHeight(36 * 4 + 8);
    smartItem(m_smartList, tr("All"), QStringLiteral("document-open"), NotebookView::All);
    smartItem(m_smartList, tr("Frequently used"), QStringLiteral("statistics"), NotebookView::Frequent);
    smartItem(m_smartList, tr("Recent"), QStringLiteral("document-open-recent"), NotebookView::Recent);
    smartItem(m_smartList, tr("Favorites"), QStringLiteral("tag"), NotebookView::Favorite);

    connect(m_smartList, &QListWidget::currentRowChanged, this, [this](int row) {
        if (m_block || row < 0) {
            return;
        }
        auto* item = m_smartList->item(row);
        if (item) {
            emit smartViewSelected(item->data(Qt::UserRole).toInt());
        }
    });
    // Adding the rows can select the first one. Leave the real group tree in charge until a click.
    m_block = true;
    m_smartList->setCurrentRow(-1);
    m_smartList->clearSelection();
    m_block = false;

    m_groupView->setParent(this);
    m_groupView->setFrameShape(QFrame::NoFrame);
    m_groupView->setIndentation(14);
    m_groupView->setAnimated(true);
    m_groupView->setIconSize({16, 16});
    m_groupView->setMinimumHeight(96);

    auto* folders = new QLabel(tr("Folders"), this);
    folders->setObjectName(QStringLiteral("notebookSection"));

    auto* statusCard = new QFrame(this);
    statusCard->setObjectName(QStringLiteral("notebookStatusCard"));
    auto* statusLayout = new QHBoxLayout(statusCard);
    statusLayout->setContentsMargins(12, 10, 12, 10);
    statusLayout->setSpacing(8);
    auto* lockIcon = new QLabel(statusCard);
    lockIcon->setPixmap(icons()->icon(QStringLiteral("object-locked")).pixmap(28, 28));
    lockIcon->setFixedSize(28, 28);
    auto* status =
        new QLabel(tr("This passbook is encrypted.\nEntries stay in a local KDBX file on this computer."), statusCard);
    status->setObjectName(QStringLiteral("notebookStatusText"));
    status->setWordWrap(true);
    statusLayout->addWidget(lockIcon, 0, Qt::AlignTop);
    statusLayout->addWidget(status, 1);

    auto* header = new QHBoxLayout();
    header->setSpacing(10);
    header->addWidget(m_iconLabel, 0, Qt::AlignTop);
    auto* titles = new QVBoxLayout();
    titles->setSpacing(2);
    titles->addWidget(m_nameLabel);
    titles->addWidget(m_countLabel);
    header->addLayout(titles, 1);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);
    layout->addLayout(header);
    layout->addWidget(m_smartList);
    layout->addWidget(folders);
    layout->addWidget(m_groupView, 1);
    layout->addWidget(statusCard);
}

void NotebookSidebar::refresh(const QSharedPointer<Database>& db)
{
    if (!db) {
        return;
    }

    QString name = db->metadata()->name();
    if (name.isEmpty()) {
        name = tr("奈娜子密码本");
    }
    m_nameLabel->setText(name);

    int count = 0;
    if (db->rootGroup()) {
        const QList<Entry*> entries = db->rootGroup()->entriesRecursive(false);
        for (const Entry* entry : entries) {
            if (entry && !entry->isRecycled()) {
                ++count;
            }
        }
    }
    m_countLabel->setText(tr("%n entries · encrypted", nullptr, count));
}

void NotebookSidebar::setSmartActive(bool active)
{
    m_block = true;
    if (!active) {
        m_smartList->clearSelection();
        m_smartList->setCurrentRow(-1);
    }
    m_block = false;

    m_groupView->setProperty("notebookSmart", active);
    m_groupView->style()->unpolish(m_groupView);
    m_groupView->style()->polish(m_groupView);
    m_groupView->viewport()->update();
}
