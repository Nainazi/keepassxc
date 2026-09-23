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

#ifndef KEEPASSX_NOTEBOOKCOMMON_H
#define KEEPASSX_NOTEBOOKCOMMON_H

#include <QDateTime>
#include <QString>

class Entry;

// Smart lists sit above the real group tree. Folder means "whatever group is selected".
enum class NotebookView
{
    Folder = 0,
    All,
    Frequent,
    Recent,
    Favorite
};

// Same tag KeePassXC importers already write, so starred rows and imports match.
QString notebookFavoriteTag();
bool notebookEntryIsFavorite(const Entry* entry);
QString notebookRelativeTime(const QDateTime& time);

#endif // KEEPASSX_NOTEBOOKCOMMON_H
