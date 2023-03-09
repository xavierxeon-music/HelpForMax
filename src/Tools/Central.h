#ifndef CentralH
#define CentralH

#include "AbstractFunctionHub.h"

#include "PatchParser.h"

class MainWindow;

class Central;

struct FunctionHub : public Abstract::FunctionHub<Central>
{
   virtual void setPackagePath(QString packageDir);
   virtual void setModified(bool enabled);
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
   const PatchParser& parser();
   PatchParser& parserRef();
   void savePatchStructures();
   void buildPatchStructure(QString patchPath, const QString& key);

protected:
   static QString packageAuthor;
   static QString packageName;

private:
   using ParserMap = QMap<QString, PatchParser>;

private:
   QString currentKey;
   ParserMap parserMap;
};

#endif // NOT CentralH
