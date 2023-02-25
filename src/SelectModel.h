#ifndef HelpSelectModelH
#define HelpSelectModelH

#include <Central.h>
#include <QStandardItemModel>

#include "HelpPersona.h"
#include <QFileInfo>

namespace Help
{
   class SelectModel : public QStandardItemModel,
                       private Central::FunctionHub
   {
      Q_OBJECT

   public:
      enum Role
      {
         RolePatchPath = Qt::UserRole + 1,
         RoleKey = Qt::UserRole + 2
      };

   public:
      SelectModel(QObject* parent);

   private:
      using InfoMap = QMap<QString, QFileInfo>;

   private:
      void setPackagePath(QString packageDir) override;
      void recursiveSearch(const QString& path, InfoMap& infoMap);
   };
} // namespace Help

#endif // NOT HelpSelectModelH
