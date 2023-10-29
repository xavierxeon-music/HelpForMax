#ifndef CentralH
#define CentralH

#include "AbstractFunctionHub.h"
#include "Block/Block.h"

class MainWindow;

class Central;

struct FunctionHub : public Abstract::FunctionHub<Central>
{
   virtual void setPackagePath(QString packageDir);
   virtual void setModified(bool enabled, QString key);
   virtual void patchSelected(QString patchPath, QString key);
   virtual void componentSelected(Block::Marker marker, QVariant data);
   virtual void restoreState();
   virtual void saveState();
};

class Central
{
public:
   Central();

public:
   static QString getPackagePath();
   static QString getAuthor();
   static QString getPackageName();

   void compileBlockMap(const QString& packagePath);
   const Block::Map getBlockMap() const;

   const QString& getCurrentKey() const;
   const Block& block();
   Block& blockRef();

   void selectBlock(const QString& key);
   bool isBlockUndocumented(const QString& key) const;

   void saveBlocks(Block::Component component);

   void readPackageInfo(QString packagePath);

public:
   static const QBrush udocBrush;
   static const QBrush docBrush;

protected:
private:
   static QString packageAuthor;
   static QString packageName;

   QString currentKey;
   Block::Map blockMap;
};

#endif // NOT CentralH
