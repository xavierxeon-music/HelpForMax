#ifndef ComponentModelH
#define ComponentModelH

#include "Tools/Central.h"

#include <QStandardItemModel>

namespace Component
{
   class Model : public QStandardItemModel,
                 private FunctionHub
   {
      Q_OBJECT
   public:
      class Filtered;

   public:
      Model(QObject* parent, Central* central);

   public:
      void patchSelected(QString patchPath, QString key) override;
      void update();
      QStandardItem* getItem(const int& row, const int& column = 0) const;

   private:
      void setModified(bool enabled, QString key) override;
      void rebuild();

   private:
      Central* central;
   };
} // namespace Component

#endif // NOT ComponentModelH
