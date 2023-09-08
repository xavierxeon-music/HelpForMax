#ifndef BlockItemH
#define BlockItemH

#include "BlockStructure.h"

#include <QMap>
#include <QObject>
#include <QString>

#include "BlockHelp.h"
#include "BlockInitFile.h"
#include "BlockPatcher.h"
#include "BlockRef.h"

namespace Block
{
   class Item : public Structure
   {
      Q_GADGET

   public:
      using Map = QMap<QString, Item*>;

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
         MessageUserDefined,
         Output
      };
      Q_ENUM(Marker)

      enum class Component
      {
         All,
         Patcher,
         Ref,
         Help,
         InitFile,
      };

      static const QList<QByteArray> descriptionMaxTags;

   public:
      Item(const QString& key, const QString& patchPath);
      virtual ~Item();

   public:
      static Map compileMap(const QString& packageDir);

      void load();
      void save(Component component);

      void clear() override;
      bool foundUndocumented() const;
      void markModified();

      const QString& getPatchPath() const;
      const QString& getRefPath() const;

      QString getRefContent();
      void setRefContent(const QString& content);

   private:
      friend class Patcher;
      friend class Ref;
      friend class Help;
      friend class InitFile;

   private:
      void markUndocumented(Base& base);

   private:
      QString key;
      bool isUndocumented;
      bool isModified;

      Patcher patcher;
      Ref ref;
      Help help;
      InitFile init;
   };
} // namespace Block

#endif // NOT BlockItemH
