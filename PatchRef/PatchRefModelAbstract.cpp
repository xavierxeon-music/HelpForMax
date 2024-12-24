#include "PatchRefModelAbstract.h"

PatchRef::Model::Abstract::Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part)
   : ListedRefModel<"PatchRef">(parent, structure, part)
{
}

void PatchRef::Model::Abstract::updateDigestItem(QStandardItem* digestItem, const Ref::Structure::Digest& digest)
{
   QString description = digest.text;
   digestItem->setText(description);

   if (digest.description.isEmpty())
      digestItem->setIcon(QIcon());
   else
      digestItem->setIcon(QIcon(":/DocDescription.svg"));
}

void PatchRef::Model::Abstract::createBeforeItem(const QModelIndex& index)
{
   Q_UNUSED(index)
}

void PatchRef::Model::Abstract::removeItem(const QModelIndex& index)
{
   Q_UNUSED(index)
}
