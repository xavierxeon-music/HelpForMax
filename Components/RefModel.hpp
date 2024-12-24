#ifndef RefModelHPP
#define RefModelHPP

#include "RefModel.h"

template <CompileTimeString Category>
ListedRefModel<Category>::InstanceMap ListedRefModel<Category>::instanceMap;

template <CompileTimeString Category>
ListedRefModel<Category>::ListedRefModel(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part)
   : RefModel(parent, structure, part)
{
   instanceMap[parent].append(this);
}

template <CompileTimeString Category>
ListedRefModel<Category>::~ListedRefModel()
{
   for (InstanceMap::iterator it = instanceMap.begin(); it != instanceMap.end(); it++)
   {
      it.value().removeAll(this);
   }
}

template <CompileTimeString Category>
template <typename ModelClass>
void ListedRefModel<Category>::callOnAllInstances(QObject* parent, void (ModelClass::*function)())
{
   if (!instanceMap.contains(parent))
      return;

   for (RefModel* modelInstance : instanceMap[parent])
   {
      ModelClass* model = dynamic_cast<ModelClass*>(modelInstance);
      if (!model)
         continue;
      (model->*function)();
   }
}

#endif // NOT RefModelHPP
