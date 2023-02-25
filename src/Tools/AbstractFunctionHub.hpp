#ifndef AbstractFunctionHubHPP
#define AbstractFunctionHubHPP

#include "AbstractFunctionHub.h"

template <typename WhatEver>
QList<Abstract::FunctionHub<WhatEver>*> Abstract::FunctionHub<WhatEver>::instanceList;

template <typename WhatEver>
Abstract::FunctionHub<WhatEver>::FunctionHub()
{
   instanceList.append(this);
}

template <typename WhatEver>
Abstract::FunctionHub<WhatEver>::~FunctionHub()
{
   instanceList.removeAll(this);
}

template <typename WhatEver>
template <typename ClassType, typename... ArgumentsType>
void Abstract::FunctionHub<WhatEver>::callOnAllHubInstances(void (ClassType::*functionPointer)(ArgumentsType...), ArgumentsType... arguments)
{
   static_assert(std::is_base_of<FunctionHub, ClassType>::value, "ClassType must derive from FunctionHub");

   for (FunctionHub<WhatEver>* instance : instanceList)
   {
      if (this == instance)
         continue;

      ClassType* classInstance = static_cast<ClassType*>(instance);
      (classInstance->*functionPointer)(arguments...);
   }
}

#endif // NOT AbstractFunctionHubHPP
