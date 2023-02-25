#ifndef AbstractFunctionHubH
#define AbstractFunctionHubH

#include <QList>

namespace Abstract
{
   template <typename WhatEver> // the typename is not used
   struct FunctionHub
   {
   protected:
      inline FunctionHub();
      inline virtual ~FunctionHub();

   protected:
      template <typename ClassType, typename... ArgumentsType>
      void callOnAllHubInstances(void (ClassType::*functionPointer)(ArgumentsType...), ArgumentsType... arguments);

   private:
      static QList<FunctionHub*> instanceList;
   };
} // namespace Abstract

#ifndef AbstractFunctionHubHPP
#include "AbstractFunctionHub.hpp"
#endif // NOT AbstractFunctionHubHPP

#endif // NOT AbstractFunctionHubH
