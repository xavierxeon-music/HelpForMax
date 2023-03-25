#ifndef BlockItemH
#define BlockItemH

#include "BlockStructure.h"

#include <QMap>
#include <QObject>
#include <QString>

#include "BlockHelp.h"
#include "BlockPatch.h"
#include "BlockRef.h"
#include "BlockSettings.h"

namespace Block
{
   class Item : public Structure
   {
      Q_GADGET

   public:
      using Map = QMap<QString, Item>;

      enum Role
      {
         RoleMarker = Qt::UserRole + 1,
         RoleData = Qt::UserRole + 2
      };

      enum class Marker
      {
         Undefined,
         Patch,
         Argument,
         Attribute,
         MessageStandard,
         MessageFree,
         Output
      };
      Q_ENUM(Marker)

      static const QList<QByteArray> descriptionMaxTags;

   public:
      Item(const QString& key = QString());

   public:
      static Map compileMap(const QString& packageDir);
      void clear() override;

   private:
      friend class Patch;
      friend class Ref;
      friend class Help;
      friend class Settings;

   private:
      void load(const QString& patchPath);
      void markUndocumented(Base& base);

   private:
      QString key;
      bool isUndocumented;

      Patch patch;
      Ref ref;
      Help help;
      Settings settings;
   };
} // namespace Block

#endif // NOT BlockItemH
