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

#include "EntryCardDelegate.h"

#include "NotebookCommon.h"
#include "core/Entry.h"
#include "core/Group.h"
#include "gui/DatabaseIcons.h"
#include "gui/Icons.h"
#include "gui/entry/EntryView.h"

#include <QMouseEvent>
#include <QPainter>

EntryCardDelegate::EntryCardDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QRect EntryCardDelegate::cardRect(const QRect& itemRect)
{
    return itemRect.adjusted(8, 4, -8, -4);
}

QRect EntryCardDelegate::starRect(const QRect& card)
{
    return QRect(card.right() - 28, card.center().y() - 12, 24, 24);
}

QSize EntryCardDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    return {option.rect.width(), 78};
}

void EntryCardDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto* view = qobject_cast<const EntryView*>(option.widget);
    Entry* entry = view ? view->entryFromIndex(index) : nullptr;
    if (!entry) {
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    const QRect card = cardRect(option.rect);
    const bool selected = option.state.testFlag(QStyle::State_Selected);
    const bool hover = option.state.testFlag(QStyle::State_MouseOver);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(232, 196, 214, selected ? 70 : 36));
    painter->drawRoundedRect(card.adjusted(0, 2, 0, 3), 14, 14);

    painter->setPen(QPen(QColor(selected ? "#E7A9C6" : "#F0E4F2"), 1));
    painter->setBrush(QColor(selected ? "#FDE8F2" : (hover ? "#FFF7FB" : "#FFFBFE")));
    painter->drawRoundedRect(card, 14, 14);

    const QRect iconBox(card.left() + 12, card.center().y() - 16, 32, 32);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#F8E6F2"));
    painter->drawRoundedRect(iconBox, 10, 10);
    const QPixmap icon = Icons::entryIconPixmap(entry, IconSize::Medium);
    const QSize iconSize = icon.size().scaled(22, 22, Qt::KeepAspectRatio);
    painter->drawPixmap(QRect(iconBox.center() - QPoint(iconSize.width() / 2, iconSize.height() / 2), iconSize), icon);

    const int textLeft = iconBox.right() + 12;
    const int textRight = card.right() - 40;
    const int textWidth = qMax(20, textRight - textLeft);

    QFont titleFont = option.font;
    titleFont.setBold(true);
    titleFont.setPointSizeF(titleFont.pointSizeF() + 0.5);
    painter->setFont(titleFont);
    painter->setPen(QColor("#3E2A4A"));
    const QString title = entry->resolveMultiplePlaceholders(entry->title());
    painter->drawText(
        QRect(textLeft, card.top() + 8, textWidth, 20),
        Qt::AlignLeft | Qt::AlignVCenter,
        painter->fontMetrics().elidedText(title.isEmpty() ? tr("Untitled") : title, Qt::ElideRight, textWidth));

    painter->setFont(option.font);
    painter->setPen(QColor("#8A7094"));
    const QString username = entry->resolveMultiplePlaceholders(entry->username());
    if (!username.isEmpty()) {
        painter->drawText(QRect(textLeft, card.top() + 28, textWidth, 18),
                          Qt::AlignLeft | Qt::AlignVCenter,
                          painter->fontMetrics().elidedText(username, Qt::ElideRight, textWidth));
    }

    int chipRight = textLeft;
    if (entry->group() && entry->group()->parentGroup()) {
        const QString chip = painter->fontMetrics().elidedText(entry->group()->name(), Qt::ElideRight, 120);
        const int chipWidth = painter->fontMetrics().horizontalAdvance(chip) + 16;
        const QRect chipRect(textLeft, card.bottom() - 24, chipWidth, 18);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#F3E6F8"));
        painter->drawRoundedRect(chipRect, 9, 9);
        painter->setPen(QColor("#7A5A92"));
        painter->drawText(chipRect, Qt::AlignCenter, chip);
        chipRight = chipRect.right() + 8;
    }

    const QString when = notebookRelativeTime(entry->timeInfo().lastModificationTime());
    painter->setPen(QColor("#A08CAF"));
    painter->drawText(QRect(chipRight, card.bottom() - 24, qMax(20, textRight - chipRight), 18),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      painter->fontMetrics().elidedText(when, Qt::ElideRight, qMax(20, textRight - chipRight)));

    const bool favorite = notebookEntryIsFavorite(entry);
    painter->setPen(QColor(favorite ? "#D4538C" : "#D5C4DE"));
    QFont starFont = option.font;
    starFont.setPointSizeF(starFont.pointSizeF() + 4);
    painter->setFont(starFont);
    painter->drawText(starRect(card), Qt::AlignCenter, favorite ? QStringLiteral("★") : QStringLiteral("☆"));

    painter->restore();
}

bool EntryCardDelegate::editorEvent(QEvent* event,
                                    QAbstractItemModel* model,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index)
{
    Q_UNUSED(model);
    if (event->type() != QEvent::MouseButtonRelease) {
        return false;
    }

    auto* mouse = static_cast<QMouseEvent*>(event);
    if (!starRect(cardRect(option.rect)).contains(mouse->pos())) {
        return false;
    }

    auto* view = qobject_cast<const EntryView*>(option.widget);
    Entry* entry = view ? view->entryFromIndex(index) : nullptr;
    if (entry) {
        emit toggleFavorite(entry);
    }
    return true;
}
