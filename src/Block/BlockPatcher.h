#ifndef BlockPatcherH
#define BlockPatcherH

#include "Block.h"

class Block::Patcher
{
public:
   Patcher(Block* block, const QString& patchPath);

public:
   const QString& getPath() const;
   void read();

private:
   struct Inlet
   {
      int index;
      QStringList connectedIdList;

      using ConnectionMap = QMap<QString, Inlet>;
   };

   struct MaxClass
   {
      QString boxType;       // first text entry
      QStringList arguments; // the rest
      int index;             // position in boxesArray of patcher object

      using List = QList<MaxClass>;

      MaxClass(const QJsonObject& boxObject, int index = 0);
   };

   using MessageMap = QMap<QString, MaxClass::List>;

private:
   Inlet::ConnectionMap compileInletConnectionMap(const QJsonObject patcherObject);
   MessageMap compileInletMessageMap(const QJsonObject patcherObject, const QStringList& boxTypeList);
   Structure::Output& findOrCreateOutput(const int id, const QString& name);
   void readPatcherargs(const QStringList& arguments);

private:
   Block* block;
   QString patchPath;
};

#endif // NOT BlockPatcherH
