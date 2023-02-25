#ifndef ModelItemH
#define ModelItemH

#include <QStandardItem>

class ModelItem : public QStandardItem
{
public:
   static const int VisibleRole;

public:
   ModelItem();
   ModelItem(const QString& text);
   ModelItem(const QIcon& icon, const QString& text);

public:
   void setVisble(bool enabled);
};

#endif // NOT ModelItemH
