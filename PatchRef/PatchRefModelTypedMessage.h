#ifndef PatchRefModelTypedMessageH
#define PatchRefModelTypedMessageH

#include "PatchRefModelAbstract.h"

namespace PatchRef
{
   namespace Model
   {
      class TypedMessage : public Abstract
      {
         Q_OBJECT

      public:
         TypedMessage(QObject* parent, Ref::Structure& structure);

      private:
         void update() override;
         void rebuild() override;
         Ref::Structure::Digest* getDigest(const QModelIndex& index) override;
         bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
      };
   } // namespace Model
} // namespace PatchRef

#endif // NOT PatchRefModelTypedMessageH
