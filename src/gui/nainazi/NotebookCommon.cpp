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

#include "NotebookCommon.h"

#include "core/Entry.h"

#include <QObject>

QString notebookFavoriteTag()
{
    return QObject::tr("Favorite", "Tag for favorite entries");
}

bool notebookEntryIsFavorite(const Entry* entry)
{
    if (!entry) {
        return false;
    }
    const QStringList tags = entry->tagList();
    return tags.contains(notebookFavoriteTag()) || tags.contains(QStringLiteral("Favorite"))
           || tags.contains(QStringLiteral("收藏"));
}

QString notebookRelativeTime(const QDateTime& time)
{
    if (!time.isValid()) {
        return {};
    }

    const QDateTime local = time.toLocalTime();
    const qint64 secs = local.secsTo(QDateTime::currentDateTime());
    if (secs < 45) {
        return QObject::tr("Just now");
    }
    if (secs < 3600) {
        const int mins = qMax(1, static_cast<int>(secs / 60));
        return QObject::tr("%n min ago", nullptr, mins);
    }
    if (secs < 86400) {
        const int hours = qMax(1, static_cast<int>(secs / 3600));
        return QObject::tr("%n h ago", nullptr, hours);
    }
    if (local.date() == QDate::currentDate().addDays(-1)) {
        return QObject::tr("Yesterday");
    }
    if (secs < 86400 * 7) {
        const int days = qMax(1, static_cast<int>(secs / 86400));
        return QObject::tr("%n days ago", nullptr, days);
    }
    return local.date().toString(QStringLiteral("yyyy-MM-dd"));
}
