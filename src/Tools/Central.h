#ifndef CentralH
#define CentralH

#include "AbstractFunctionHub.h"
#include "Block/BlockItem.h"
#include "Tools/PatchParser.h"

class MainWindow;

class Central;

struct FunctionHub : public Abstract::FunctionHub<Central>
{
   virtual void setPackagePath(QString packageDir);
   virtual void setModified(bool enabled, QString key);
   virtual void patchSelected(QString patchPath, QString key);
   virtual void componentSelected(PatchParser::Marker marker, QVariant data);
};

class Central : public FunctionHub
{
public:
   Central();

public:
   static QString getPackagePath();
   static QString getAuthor();
   static QString getPackageName();

   const QString& getCurrentKey() const;
   void compileBlockMap(const QString& packagePath);

   const PatchParser& parser();
   PatchParser& parserRef();

   void savePatchStructures();
   void loadPatchStructure(QString patchPath, const QString& key);
   void selectPatchStructure(const QString& key);
   bool isPatchStructureUndocumented(const QString& key) const;
   QString getRefPath(const QString& key) const;

protected:
   static QString packageAuthor;
   static QString packageName;

private:
   using ParserMap = QMap<QString, PatchParser>;

private:
   QString currentKey;
   Block::Item::Map blockMap;
   ParserMap parserMap;
};

#endif // NOT CentralH
