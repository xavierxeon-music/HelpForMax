#ifndef SelectModelH
#define SelectModelH

#include "Tools/Central.h"
#include <QStandardItemModel>

#include "Tools/Central.h"
#include <QFileInfo>

namespace Select
{
   class Model : public QStandardItemModel,
                 private FunctionHub
   {
      Q_OBJECT

   public:
      enum Role
      {
         RolePatchPath = Qt::UserRole + 1,
         RoleKey = Qt::UserRole + 2
      };

   public:
      Model(QObject* parent, Central* central);

   private:
      void setPackagePath(QString packageDir) override;
      void setModified(bool enabled, QString key) override;

   private:
      Central* central;
   };
} // namespace Select

#endif // NOT SelectModelH
