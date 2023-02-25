#include "ModelItem.h"

ModelItem::ModelItem()
   : ModelItem(QIcon(), QString())
{
}

ModelItem::ModelItem(const QString& text)
   : ModelItem(QIcon(), text)
{
}

ModelItem::ModelItem(const QIcon& icon, const QString& text)
   : QStandardItem(icon, text)
{
   setEditable(false);
}
