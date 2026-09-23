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

#ifndef KEEPASSX_ENTRYCARDDELEGATE_H
#define KEEPASSX_ENTRYCARDDELEGATE_H

#include <QStyledItemDelegate>

class Entry;

class EntryCardDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit EntryCardDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent* event,
                     QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;

signals:
    void toggleFavorite(Entry* entry);

private:
    static QRect cardRect(const QRect& itemRect);
    static QRect starRect(const QRect& card);
};

#endif // KEEPASSX_ENTRYCARDDELEGATE_H
