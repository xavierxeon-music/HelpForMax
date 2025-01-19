#ifndef SuggestTransferModelH
#define SuggestTransferModelH

#include <QStandardItemModel>

namespace Suggest
{
   namespace Transfer
   {
      class Model : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Model(QObject* parent);
      };
   } // namespace Transfer
} // namespace Suggest

#endif // NOT SuggestTransferModelH
