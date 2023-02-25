#include "ModelItem.h"

const int ModelItem::VisibleRole = Qt::UserRole + 100;

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
   setVisble(true);
}

void ModelItem::setVisble(bool enabled)
{
   setData(VisibleRole, enabled);
}
