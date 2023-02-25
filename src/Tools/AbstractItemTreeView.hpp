#ifndef AbstractItemTreeViewHPP
#define AbstractItemTreeViewHPP

#include "AbstractItemTreeView.h"

template <typename ItemModel>
ItemModel* Abstract::ItemTreeView::getModel()
{
   return qobject_cast<ItemModel*>(model);
}

#endif // NOT AbstractItemTreeViewHPP
