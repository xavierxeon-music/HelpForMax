#ifndef HelpPatchParserH
#define HelpPatchParserH

#include <PatchStructure.h>

#include <QDomElement>
#include <QMap>
#include <QObject>

namespace Help
{
   class PatchParser : public PatchStructure
   {
      Q_GADGET
   public:
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
         Message,
         Output
      };
      Q_ENUM(Marker)

      static const QList<QByteArray> descriptionMaxTags;

   public:
      PatchParser();
      PatchParser(const QString& patchPath);

   public:
      void writeXML();

   private:
      using TagMap = QMap<QString, QString>;
      friend class Persona;

   private:
      QDomElement createSubElement(QDomElement parent, const QString& name, const QString& text = QString(), const TagMap& tagMap = TagMap());
      void addDigest(const QDomElement& parentElement, const Digest& digest);

      void readXML();
      void readDigest(const QDomElement& parentElement, Digest& digest) const;
      QString readText(const QDomElement& element) const;
      QDomElement findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const;
      QList<QDomElement> compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap = TagMap()) const;
      QByteArray domToMaxFile(QByteArray domXML) const;
      QByteArray maxFileToDom(QByteArray maxXML) const;

      void addJSON();
      Output& findOrCreateOutput(const int id);

   private:
      QString patchName;
      QString patchPath;
      QString helpPath;
   };
} // namespace Help

#endif // NOT HelpPatchParserH
