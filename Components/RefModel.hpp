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
template <typename ModelClass>
void ListedRefModel<Category>::callOnAllInstances(void (ModelClass::*function)())
{
   for (RefModel* modelInstance : instanceList)
   {
      ModelClass* model = dynamic_cast<ModelClass*>(modelInstance);
      if (!model)
         continue;
      (model->*function)();
   }
}

#endif // NOT RefModelHPP
