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
      Item();
      Item(const QString& key, const QString& patchPath);
      virtual ~Item();

   public:
      static Map compileMap(const QString& packageDir);

      void load();
      void save();

      void clear() override;
      bool foundUndocumented() const;

      const QString& getPatchPath() const;
      const QString& getRefPath() const;

   private:
      friend class Patch;
      friend class Ref;
      friend class Help;
      friend class Settings;

   private:
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
