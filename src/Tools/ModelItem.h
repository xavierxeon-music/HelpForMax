#ifndef ModelItemH
#define ModelItemH

#include <QStandardItem>

class ModelItem : public QStandardItem
{
public:
   ModelItem();
   ModelItem(const QString& text);
   ModelItem(const QIcon& icon, const QString& text);
};

#endif // NOT ModelItemH
