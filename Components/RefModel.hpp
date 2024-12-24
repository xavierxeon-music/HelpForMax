#ifndef RefModelHPP
#define RefModelHPP

#include "RefModel.h"

template <CompileTimeString Category>
QList<RefModel*> ListedRefModel<Category>::instanceList;

template <CompileTimeString Category>
ListedRefModel<Category>::ListedRefModel(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part)
   : RefModel(parent, structure, part)
{
   instanceList.append(this);
}

template <CompileTimeString Category>
ListedRefModel<Category>::~ListedRefModel()
{
   instanceList.removeAll(this);
}

template <CompileTimeString Category>
void ListedRefModel<Category>::rebuildAll()
{
   for (RefModel* model : instanceList)
      model->rebuild();
}

template <CompileTimeString Category>
void ListedRefModel<Category>::updateAll()
{
   for (RefModel* model : instanceList)
      model->update();
}

#endif // NOT RefModelHPP
