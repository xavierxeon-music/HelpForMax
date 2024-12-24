#include "SuggestModelAbstract.h"

QList<Suggest::Model::Abstract*> Suggest::Model::Abstract::instanceList;

Suggest::Model::Abstract::Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest, const Ref::Structure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , suggest(suggest)
   , part(part)
{
   instanceList.append(this);
}

Suggest::Model::Abstract::~Abstract()
{
   instanceList.removeAll(this);
}

void Suggest::Model::Abstract::rebuildAll()
{
   for (Abstract* model : instanceList)
      model->rebuild();
}
