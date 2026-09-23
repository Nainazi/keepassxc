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

#ifndef KEEPASSX_NOTEBOOKDETAIL_H
#define KEEPASSX_NOTEBOOKDETAIL_H

#include <QWidget>

class Entry;
class QLabel;
class QProgressBar;
class QToolButton;

class NotebookDetail : public QWidget
{
    Q_OBJECT

public:
    explicit NotebookDetail(QWidget* parent = nullptr);

    void setEntry(Entry* entry);
    void refresh();

signals:
    void editRequested();
    void deleteRequested();
    void copyRequested(const QString& text);
    void favoriteToggled(Entry* entry);
    void autoTypeRequested();
    void openUrlRequested();
    void cloneRequested();

private:
    struct Field
    {
        QWidget* row = nullptr;
        QLabel* value = nullptr;
        QToolButton* reveal = nullptr;
    };

    Field makeField(const QString& caption, bool revealable);
    void setFieldText(Field& field, const QString& text, bool conceal);
    QString fieldText(const Field& field) const;

    Entry* m_entry = nullptr;
    bool m_passwordShown = false;

    QWidget* m_body;
    QLabel* m_placeholder;
    QLabel* m_icon;
    QLabel* m_title;
    QLabel* m_subtitle;
    QToolButton* m_star;
    Field m_username;
    Field m_password;
    Field m_url;
    Field m_notes;
    Field m_modified;
    Field m_totp;
    QWidget* m_strengthCard;
    QLabel* m_strengthLabel;
    QLabel* m_strengthReason;
    QProgressBar* m_strengthBar;
};

#endif // KEEPASSX_NOTEBOOKDETAIL_H
