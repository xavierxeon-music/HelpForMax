#ifndef BlockRefH
#define BlockRefH

#include "Block.h"

#include <QDomElement>

class Block::Ref
{
public:
   Ref(Block* block);

public:
   const QString& getPath() const;

   void read();
   void readContent(const QString& content);

   void write();
   QString writeContent();

private:
   using TagMap = QMap<QString, QString>;

private:
   QDomElement createSubElement(QDomElement parent, const QString& name, const QString& text = QString(), const TagMap& tagMap = TagMap());
   void addDigest(const QDomElement& parentElement, const Structure::Digest& digest);

   void readDigest(const QDomElement& parentElement, Structure::Digest& digest) const;
   QString readText(const QDomElement& element) const;
   QDomElement findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const;
   QList<QDomElement> compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap = TagMap()) const;

   QString domToMaxFile(QString domXML) const;
   QString maxFileToDom(QString maxXML) const;

private:
   Block* block;
   QString refPath;
};

#endif // NOT BlockRefH
