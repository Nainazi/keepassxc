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

#ifndef KEEPASSX_NOTEBOOKSIDEBAR_H
#define KEEPASSX_NOTEBOOKSIDEBAR_H

#include "NotebookCommon.h"

#include <QSharedPointer>
#include <QWidget>

class Database;
class GroupView;
class QLabel;
class QListWidget;

class NotebookSidebar : public QWidget
{
    Q_OBJECT

public:
    explicit NotebookSidebar(GroupView* groupView, QWidget* parent = nullptr);

    void refresh(const QSharedPointer<Database>& db);
    // Clears the smart-list highlight when a real group is selected.
    void setSmartActive(bool active);

signals:
    void smartViewSelected(int view);

private:
    GroupView* m_groupView;
    QLabel* m_iconLabel;
    QLabel* m_nameLabel;
    QLabel* m_countLabel;
    QListWidget* m_smartList;
    bool m_block = false;
};

#endif // KEEPASSX_NOTEBOOKSIDEBAR_H
