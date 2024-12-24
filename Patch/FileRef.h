#ifndef FileRefXMLH
#define FileRefXMLH

#include "FileAbstract.h"

#include <QDomElement>

namespace File
{
   class RefXML : public Abstract
   {
   public:
      RefXML(const Package::Info* info, Ref::Structure& structure);

   public:
      void read(const Patch::Info& patchInfo) override;
      void write(const Patch::Info& patchInfo) override;
      QString getFilePath(const Patch::Info& patchInfo) override;

   private:
      using TagMap = QMap<QString, QString>;

   private:
      void readContent(const QByteArray& content);
      QByteArray writeContent(const QString& patchName);

      QDomElement createSubElement(QDomElement parent, const QString& name, const QString& text = QString(), const TagMap& tagMap = TagMap());
      void addDigest(const QDomElement& parentElement, const Ref::Structure::Digest& digest);

      void readDigest(const QDomElement& parentElement, Ref::Structure::Digest& digest) const;
      QString readText(const QDomElement& element) const;
      QDomElement findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const;
      QList<QDomElement> compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap = TagMap()) const;

      QByteArray domToMaxFile(QByteArray domXML) const;
      QByteArray maxFileToDom(QByteArray maxXML) const;

   private:
   };
} // namespace File

#endif // NOT FileRefXMLH
