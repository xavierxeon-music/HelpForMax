#include "ModelItem.h"

static constexpr int VisibleRole = Qt::UserRole + 100;

// ModelItem

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
   setVisible(true);
}

void ModelItem::setVisible(bool enabled)
{
   setData(enabled, VisibleRole);
}

bool ModelItem::isVisible()
{
   return data(VisibleRole).toBool();
}

// FilteredModel

FilteredModel::FilteredModel(QObject* parent)
   : QSortFilterProxyModel(parent)
{
}

bool FilteredModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
   QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
   QVariant visibleData = sourceModel()->data(sourceIndex, VisibleRole);
   if (visibleData.isNull())
      return true;

   const bool visible = visibleData.toBool();
   return visible;
}
