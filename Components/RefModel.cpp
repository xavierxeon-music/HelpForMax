#include "RefModel.h"

RefModel::RefModel(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
}

RefModel::~RefModel()
{
}

const Ref::Structure::PatchPart& RefModel::getPart() const
{
   return part;
}

void RefModel::removeContent()
{
   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);
}
