#ifndef BlockRefH
#define BlockRefH

#include <QDomElement>

#include "BlockStructure.h"

namespace Block
{
   class Item;

   class Ref
   {
   public:
      Ref(Item* item);

   public:
      const QString& getPath() const;
      void read();
      void write();

   private:
      using TagMap = QMap<QString, QString>;

   private:
      QDomElement createSubElement(QDomElement parent, const QString& name, const QString& text = QString(), const TagMap& tagMap = TagMap());
      void addDigest(const QDomElement& parentElement, const Structure::Digest& digest);

      void readDigest(const QDomElement& parentElement, Structure::Digest& digest) const;
      QString readText(const QDomElement& element) const;
      QDomElement findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const;
      QList<QDomElement> compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap = TagMap()) const;
      QByteArray domToMaxFile(QByteArray domXML) const;
      QByteArray maxFileToDom(QByteArray maxXML) const;

   private:
      Item* item;
      QString refPath;
   };
} // namespace Block

#endif // NOT BlockRefH
