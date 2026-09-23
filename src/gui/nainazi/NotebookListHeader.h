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

#ifndef KEEPASSX_NOTEBOOKLISTHEADER_H
#define KEEPASSX_NOTEBOOKLISTHEADER_H

#include <QWidget>

class QComboBox;
class QLabel;
class QPushButton;
class SearchWidget;

class NotebookListHeader : public QWidget
{
    Q_OBJECT

public:
    explicit NotebookListHeader(QWidget* parent = nullptr);

    void setSummary(const QString& title, int count);
    void adoptSearch(SearchWidget* search);
    // Column is an EntryModel::ModelColumn. The view applies it.
    int sortColumn() const;
    Qt::SortOrder sortOrder() const;

signals:
    void newEntryRequested();
    void importRequested();
    void exportRequested();
    void newGroupRequested();
    void lockRequested();
    void generatorRequested();
    void settingsRequested();
    void databaseSettingsRequested();
    void sortChanged();

private:
    QPushButton* makeGhost(const QString& text);
    QLabel* m_summary;
    QComboBox* m_sort;
    QWidget* m_searchHost;
};

#endif // KEEPASSX_NOTEBOOKLISTHEADER_H
