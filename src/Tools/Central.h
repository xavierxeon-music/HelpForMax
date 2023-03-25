#ifndef CentralH
#define CentralH

#include "AbstractFunctionHub.h"
#include "Block/BlockItem.h"

class MainWindow;

class Central;

struct FunctionHub : public Abstract::FunctionHub<Central>
{
   virtual void setPackagePath(QString packageDir);
   virtual void setModified(bool enabled, QString key);
   virtual void patchSelected(QString patchPath, QString key);
   virtual void componentSelected(Block::Item::Marker marker, QVariant data);
};

class Central : public FunctionHub
{
public:
   Central();

public:
   static QString getPackagePath();
   static QString getAuthor();
   static QString getPackageName();

   void compileBlockMap(const QString& packagePath);
   const Block::Item::Map getBlockMap() const;

   const QString& getCurrentKey() const;
   const Block::Item& block();
   Block::Item& blockRef();

   void selectBlock(const QString& key);
   bool isBlockUndocumented(const QString& key) const;

   void saveBlocks();

protected:
   static QString packageAuthor;
   static QString packageName;

private:
   QString currentKey;
   Block::Item::Map blockMap;
};

#endif // NOT CentralH
