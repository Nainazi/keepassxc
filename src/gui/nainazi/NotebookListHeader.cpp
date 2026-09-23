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

#include "NotebookListHeader.h"

#include "gui/Icons.h"
#include "gui/SearchWidget.h"
#include "gui/entry/EntryModel.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

NotebookListHeader::NotebookListHeader(QWidget* parent)
    : QWidget(parent)
    , m_summary(new QLabel(this))
    , m_sort(new QComboBox(this))
    , m_searchHost(new QWidget(this))
{
    setObjectName(QStringLiteral("notebookListHeader"));

    auto* createButton = new QPushButton(tr("New entry"), this);
    createButton->setObjectName(QStringLiteral("notebookNewEntry"));
    createButton->setIcon(icons()->icon(QStringLiteral("entry-new")));
    createButton->setCursor(Qt::PointingHandCursor);
    connect(createButton, &QPushButton::clicked, this, &NotebookListHeader::newEntryRequested);

    auto* importButton = makeGhost(tr("Import"));
    importButton->setIcon(icons()->icon(QStringLiteral("document-import")));
    importButton->setToolTip(tr("Import entries from another password file"));
    connect(importButton, &QPushButton::clicked, this, &NotebookListHeader::importRequested);

    auto* exportButton = makeGhost(tr("Export"));
    exportButton->setIcon(icons()->icon(QStringLiteral("document-export")));
    exportButton->setToolTip(tr("Save a copy of this passbook as a KDBX file"));
    connect(exportButton, &QPushButton::clicked, this, &NotebookListHeader::exportRequested);

    auto* moreButton = makeGhost(tr("More"));
    auto* moreMenu = new QMenu(moreButton);
    moreMenu->addAction(tr("New folder"), this, &NotebookListHeader::newGroupRequested);
    moreMenu->addAction(tr("Lock passbook"), this, &NotebookListHeader::lockRequested);
    moreMenu->addSeparator();
    moreMenu->addAction(tr("Password generator"), this, &NotebookListHeader::generatorRequested);
    moreMenu->addAction(tr("Settings"), this, &NotebookListHeader::settingsRequested);
    moreMenu->addAction(tr("Passbook settings"), this, &NotebookListHeader::databaseSettingsRequested);
    moreButton->setMenu(moreMenu);

    auto* searchLayout = new QHBoxLayout(m_searchHost);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    m_searchHost->setMinimumWidth(220);
    m_searchHost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* actions = new QHBoxLayout();
    actions->setSpacing(8);
    actions->addWidget(createButton);
    actions->addWidget(importButton);
    actions->addWidget(exportButton);
    actions->addWidget(moreButton);
    actions->addStretch(1);
    actions->addWidget(m_searchHost, 1);

    auto summaryFont = m_summary->font();
    summaryFont.setBold(true);
    summaryFont.setPointSizeF(summaryFont.pointSizeF() + 1);
    m_summary->setFont(summaryFont);
    m_summary->setObjectName(QStringLiteral("notebookSummary"));

    m_sort->setObjectName(QStringLiteral("notebookSort"));
    m_sort->addItem(tr("Recently changed"), EntryModel::Modified);
    m_sort->addItem(tr("Title"), EntryModel::Title);
    m_sort->addItem(tr("Username"), EntryModel::Username);
    connect(m_sort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NotebookListHeader::sortChanged);
    // Folder lists stay in the existing title order until the user picks a sort.
    m_sort->blockSignals(true);
    m_sort->setCurrentIndex(1);
    m_sort->blockSignals(false);

    auto* sortRow = new QHBoxLayout();
    sortRow->addWidget(m_summary, 1);
    auto* sortLabel = new QLabel(tr("Sort"), this);
    sortLabel->setObjectName(QStringLiteral("notebookSortLabel"));
    sortRow->addWidget(sortLabel);
    sortRow->addWidget(m_sort);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 0, 4, 0);
    layout->setSpacing(8);
    layout->addLayout(actions);
    layout->addLayout(sortRow);
}

void NotebookListHeader::setSummary(const QString& title, int count)
{
    m_summary->setText(tr("%1 (%2)", "List heading and entry count").arg(title).arg(count));
}

void NotebookListHeader::adoptSearch(SearchWidget* search)
{
    if (!search) {
        return;
    }
    if (search->parentWidget() != m_searchHost) {
        m_searchHost->layout()->addWidget(search);
    }
    search->setMinimumWidth(220);
    search->show();
}

int NotebookListHeader::sortColumn() const
{
    return m_sort->currentData().toInt();
}

Qt::SortOrder NotebookListHeader::sortOrder() const
{
    // "Recently changed" reads newest first. Name sorts read A to Z.
    if (m_sort->currentData().toInt() == EntryModel::Modified) {
        return Qt::DescendingOrder;
    }
    return Qt::AscendingOrder;
}

QPushButton* NotebookListHeader::makeGhost(const QString& text)
{
    auto* button = new QPushButton(text, this);
    button->setProperty("notebookGhost", true);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}
