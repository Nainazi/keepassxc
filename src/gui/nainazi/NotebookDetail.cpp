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

#include "NotebookDetail.h"

#include "NotebookCommon.h"
#include "core/Clock.h"
#include "core/Entry.h"
#include "core/Group.h"
#include "core/PasswordHealth.h"
#include "gui/DatabaseIcons.h"
#include "gui/Icons.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QProgressBar>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

NotebookDetail::Field NotebookDetail::makeField(const QString& caption, bool revealable)
{
    Field field;
    auto* row = new QFrame(m_body);
    row->setObjectName(QStringLiteral("notebookField"));
    auto* layout = new QVBoxLayout(row);
    layout->setContentsMargins(12, 8, 8, 8);
    layout->setSpacing(2);

    auto* captionLabel = new QLabel(caption, row);
    captionLabel->setObjectName(QStringLiteral("notebookFieldCaption"));

    auto* valueRow = new QHBoxLayout();
    valueRow->setSpacing(4);
    field.value = new QLabel(row);
    field.value->setObjectName(QStringLiteral("notebookFieldValue"));
    field.value->setTextInteractionFlags(Qt::TextSelectableByMouse);
    field.value->setWordWrap(true);
    field.value->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    valueRow->addWidget(field.value, 1);

    if (revealable) {
        field.reveal = new QToolButton(row);
        field.reveal->setCheckable(true);
        field.reveal->setAutoRaise(true);
        field.reveal->setToolTip(tr("Show or hide"));
        valueRow->addWidget(field.reveal);
    }

    auto* copyButton = new QToolButton(row);
    copyButton->setAutoRaise(true);
    copyButton->setIcon(icons()->icon(QStringLiteral("clipboard-text")));
    copyButton->setToolTip(tr("Copy"));
    valueRow->addWidget(copyButton);
    connect(copyButton, &QToolButton::clicked, this, [this, field] {
        const QString text = fieldText(field);
        if (!text.isEmpty()) {
            emit copyRequested(text);
        }
    });

    layout->addWidget(captionLabel);
    layout->addLayout(valueRow);
    field.row = row;
    return field;
}

NotebookDetail::NotebookDetail(QWidget* parent)
    : QWidget(parent)
    , m_body(new QWidget(this))
    , m_placeholder(new QLabel(tr("Select an entry to see the username, password, and notes here."), this))
    , m_icon(new QLabel(m_body))
    , m_title(new QLabel(m_body))
    , m_subtitle(new QLabel(m_body))
    , m_star(new QToolButton(m_body))
{
    setObjectName(QStringLiteral("notebookDetail"));
    setMinimumWidth(280);

    m_placeholder->setObjectName(QStringLiteral("notebookDetailPlaceholder"));
    m_placeholder->setWordWrap(true);
    m_placeholder->setAlignment(Qt::AlignCenter);

    m_icon->setFixedSize(42, 42);
    auto titleFont = m_title->font();
    titleFont.setBold(true);
    titleFont.setPointSizeF(titleFont.pointSizeF() + 3);
    m_title->setFont(titleFont);
    m_title->setWordWrap(true);
    m_title->setObjectName(QStringLiteral("notebookDetailTitle"));
    m_subtitle->setObjectName(QStringLiteral("notebookDetailSubtitle"));
    m_subtitle->setWordWrap(true);

    m_star->setAutoRaise(true);
    m_star->setToolTip(tr("Favorite"));
    m_star->setStyleSheet(
        QStringLiteral("QToolButton { font-size: 16px; color: #D4538C; border: none; background: transparent; }"));
    connect(m_star, &QToolButton::clicked, this, [this] {
        if (m_entry) {
            emit favoriteToggled(m_entry);
        }
    });

    auto* editButton = new QToolButton(m_body);
    editButton->setAutoRaise(true);
    editButton->setIcon(icons()->icon(QStringLiteral("entry-edit")));
    editButton->setToolTip(tr("Edit"));
    connect(editButton, &QToolButton::clicked, this, &NotebookDetail::editRequested);

    auto* moreButton = new QToolButton(m_body);
    moreButton->setAutoRaise(true);
    moreButton->setText(QStringLiteral("···"));
    moreButton->setToolTip(tr("More"));
    auto* moreMenu = new QMenu(moreButton);
    moreMenu->addAction(tr("Auto-Type"), this, &NotebookDetail::autoTypeRequested);
    moreMenu->addAction(tr("Open URL"), this, &NotebookDetail::openUrlRequested);
    moreMenu->addAction(tr("Clone"), this, &NotebookDetail::cloneRequested);
    moreMenu->addSeparator();
    moreMenu->addAction(tr("Delete"), this, &NotebookDetail::deleteRequested);
    moreButton->setMenu(moreMenu);
    moreButton->setPopupMode(QToolButton::InstantPopup);

    auto* header = new QHBoxLayout();
    header->setSpacing(8);
    header->addWidget(m_icon, 0, Qt::AlignTop);
    auto* titles = new QVBoxLayout();
    titles->setSpacing(0);
    titles->addWidget(m_title);
    titles->addWidget(m_subtitle);
    header->addLayout(titles, 1);
    header->addWidget(m_star, 0, Qt::AlignTop);
    header->addWidget(editButton, 0, Qt::AlignTop);
    header->addWidget(moreButton, 0, Qt::AlignTop);

    m_username = makeField(tr("Username"), false);
    m_password = makeField(tr("Password"), true);
    m_url = makeField(tr("URL"), false);
    m_notes = makeField(tr("Notes"), false);
    m_modified = makeField(tr("Modified"), false);
    m_totp = makeField(tr("TOTP"), false);

    if (m_password.reveal) {
        connect(m_password.reveal, &QToolButton::toggled, this, [this](bool shown) {
            m_passwordShown = shown;
            refresh();
        });
    }

    m_strengthCard = new QFrame(m_body);
    m_strengthCard->setObjectName(QStringLiteral("notebookStrength"));
    auto* strengthLayout = new QVBoxLayout(m_strengthCard);
    strengthLayout->setContentsMargins(12, 10, 12, 10);
    strengthLayout->setSpacing(4);
    m_strengthLabel = new QLabel(m_strengthCard);
    m_strengthLabel->setObjectName(QStringLiteral("notebookStrengthLabel"));
    m_strengthReason = new QLabel(m_strengthCard);
    m_strengthReason->setWordWrap(true);
    m_strengthReason->setObjectName(QStringLiteral("notebookStrengthReason"));
    m_strengthBar = new QProgressBar(m_strengthCard);
    m_strengthBar->setRange(0, 100);
    m_strengthBar->setTextVisible(false);
    m_strengthBar->setFixedHeight(8);
    strengthLayout->addWidget(m_strengthLabel);
    strengthLayout->addWidget(m_strengthReason);
    strengthLayout->addWidget(m_strengthBar);

    auto* fields = new QVBoxLayout(m_body);
    fields->setContentsMargins(12, 12, 12, 12);
    fields->setSpacing(8);
    fields->addLayout(header);
    fields->addWidget(m_username.row);
    fields->addWidget(m_password.row);
    fields->addWidget(m_url.row);
    fields->addWidget(m_notes.row);
    fields->addWidget(m_modified.row);
    fields->addWidget(m_totp.row);
    fields->addWidget(m_strengthCard);
    fields->addStretch(1);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidget(m_body);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_placeholder);
    layout->addWidget(scroll, 1);
    m_body->setVisible(false);
}

void NotebookDetail::setEntry(Entry* entry)
{
    m_entry = entry;
    m_passwordShown = false;
    if (m_password.reveal) {
        m_password.reveal->setChecked(false);
    }
    refresh();
}

void NotebookDetail::setFieldText(Field& field, const QString& text, bool conceal)
{
    field.value->setProperty("plainText", text);
    if (conceal && !text.isEmpty()) {
        field.value->setText(QString(12, QChar(0x2022)));
    } else if (text.isEmpty()) {
        field.value->setText(tr("Empty"));
    } else {
        field.value->setText(text);
    }
}

QString NotebookDetail::fieldText(const Field& field) const
{
    return field.value->property("plainText").toString();
}

void NotebookDetail::refresh()
{
    const bool hasEntry = m_entry != nullptr;
    m_placeholder->setVisible(!hasEntry);
    m_body->setVisible(hasEntry);
    if (!hasEntry) {
        return;
    }

    const QString title = m_entry->resolveMultiplePlaceholders(m_entry->title());
    m_title->setText(title.isEmpty() ? tr("Untitled") : title);
    m_icon->setPixmap(
        Icons::entryIconPixmap(m_entry, IconSize::Large).scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QString subtitle;
    if (m_entry->group()) {
        subtitle = m_entry->group()->name();
    }
    m_subtitle->setText(subtitle);
    m_star->setText(notebookEntryIsFavorite(m_entry) ? QStringLiteral("★") : QStringLiteral("☆"));

    setFieldText(m_username, m_entry->resolveMultiplePlaceholders(m_entry->username()), false);

    const QString password = m_entry->resolveMultiplePlaceholders(m_entry->password());
    setFieldText(m_password, password, !m_passwordShown);
    if (m_password.reveal) {
        m_password.reveal->setIcon(icons()->onOffIcon(QStringLiteral("password-show"), m_passwordShown));
        m_password.reveal->setVisible(!password.isEmpty());
    }

    setFieldText(m_url, m_entry->resolveMultiplePlaceholders(m_entry->displayUrl()), false);
    const QString notes = m_entry->resolveMultiplePlaceholders(m_entry->notes());
    setFieldText(m_notes, notes, false);
    m_notes.value->setMinimumHeight(notes.isEmpty() ? 0 : 36);

    setFieldText(m_modified, Clock::toString(m_entry->timeInfo().lastModificationTime().toLocalTime()), false);

    if (m_entry->hasTotp()) {
        setFieldText(m_totp, m_entry->totp(), false);
        m_totp.row->setVisible(true);
    } else {
        m_totp.row->setVisible(false);
    }

    if (password.isEmpty()) {
        m_strengthCard->setVisible(false);
        return;
    }

    const PasswordHealth health(password);
    QString qualityName;
    int percent = 40;
    QString chunk = QStringLiteral("#E7A0A8");
    switch (health.quality()) {
    case PasswordHealth::Quality::Bad:
    case PasswordHealth::Quality::Poor:
        qualityName = tr("Poor", "Password quality");
        percent = 28;
        chunk = QStringLiteral("#E08A98");
        break;
    case PasswordHealth::Quality::Weak:
        qualityName = tr("Weak", "Password quality");
        percent = 48;
        chunk = QStringLiteral("#E2B15A");
        break;
    case PasswordHealth::Quality::Good:
        qualityName = tr("Good", "Password quality");
        percent = 72;
        chunk = QStringLiteral("#7DBE8A");
        break;
    case PasswordHealth::Quality::Excellent:
        qualityName = tr("Excellent", "Password quality");
        percent = 94;
        chunk = QStringLiteral("#5AAA9A");
        break;
    }
    m_strengthCard->setVisible(true);
    m_strengthLabel->setText(tr("Password strength: %1").arg(qualityName));
    const QString reason = health.scoreReason().trimmed();
    m_strengthReason->setText(reason.isEmpty() ? tr("Stored only in this encrypted passbook.") : reason);
    m_strengthBar->setValue(percent);
    m_strengthBar->setStyleSheet(
        QStringLiteral("QProgressBar { background: #F6E4EE; border: none; border-radius: 4px; }"
                       "QProgressBar::chunk { background: %1; border-radius: 4px; }")
            .arg(chunk));
}
