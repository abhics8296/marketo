#include "taglist.h"

#include <QList>
#include <QLabel>
#include <QDebug>

TagList::TagList(QWidget* parent)
    : QListWidget(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setFlow(QListView::LeftToRight);
    setContentsMargins(0, 0, 0, 0);
    setFixedHeight(24);
    sizePolicy().setHorizontalPolicy(QSizePolicy::MinimumExpanding);
}

void TagList::stretchWidth()
{
    QFont font;
    font.setFamily(font.defaultFamily());
    QFontMetrics fm(font);
    int size = 0;
    for (auto item : findItems("*", Qt::MatchWildcard))
        size += fm.width(tagText(row(item))) + 5;
    
    setFixedWidth(size);
}

QString TagList::tagText(int i) const {
    return (dynamic_cast<QLabel*> (itemWidget(item(i))))->text();
}

QStringList TagList::addTags(const QString &tags)
{
    QStringList listOfTags;
    QStringListIterator tagIter(tags.split(";", QString::SkipEmptyParts));
    while (tagIter.hasNext()) {
        QString tag(tagIter.next());
        QListWidgetItem *item = nullptr;
        
        // the full item list is a set
        for (auto iter : findItems("*", Qt::MatchWildcard))
            if (tag == tagText(row(iter))) {
                item = iter;
                break;
            }
            
        if (item == nullptr) {
            listOfTags << tag;
            QListWidgetItem *newItem = new QListWidgetItem(this);
            QLabel *newLabel = new QLabel(tag);
            newLabel->setAlignment(Qt::AlignHCenter);
            setItemWidget(newItem, newLabel);
            newItem->setText("");
            newItem->setSizeHint(itemWidget(newItem)->sizeHint() + QSize(5, 5));
            addItem(newItem);
        } else {
            auto itemToDelete = takeItem(row(item));
            delete itemWidget(itemToDelete);
            delete itemToDelete;
        }
    } // end of outer while
    return listOfTags;
}

TagList::~TagList()
{
    for (auto item : findItems("*", Qt::MatchWildcard))
        delete itemWidget(item);
}

#include "taglist.moc"