#include "PatchRefModelAbstract.h"

QList<PatchRef::Model::Abstract*> PatchRef::Model::Abstract::instanceList;

PatchRef::Model::Abstract::Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
   instanceList.append(this);
}

PatchRef::Model::Abstract::~Abstract()
{
   instanceList.removeAll(this);
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

const Ref::Structure::PatchPart& PatchRef::Model::Abstract::getPart() const
{
   return part;
}

void PatchRef::Model::Abstract::updateAll()
{
   for (Abstract* model : instanceList)
      model->update();
}

void PatchRef::Model::Abstract::rebuildAll()
{
   for (Abstract* model : instanceList)
      model->rebuild();
}
