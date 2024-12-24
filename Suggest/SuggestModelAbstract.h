#ifndef SuggestModelAbstractH
#define SuggestModelAbstractH

#include <QStandardItemModel>

#include "RefStructure.h"

namespace Suggest
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest, const Ref::Structure::PatchPart& part);
         ~Abstract();

      public:
         virtual void rebuild() = 0;
         static void rebuildAll();

      protected:
         Ref::Structure& structure;
         const Ref::Structure& suggest;
         const Ref::Structure::PatchPart part;

      private:
         static QList<Abstract*> instanceList;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelAbstractH
