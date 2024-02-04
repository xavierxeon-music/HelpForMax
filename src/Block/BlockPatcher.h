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
      QString id;            // box id

      using List = QList<MaxClass>;

      MaxClass(const QJsonObject& boxObject);
   };

   using MessageMap = QMap<QString, MaxClass::List>;

private:
   Inlet::ConnectionMap compileInletConnectionMap(const QJsonObject patcherObject);
   MessageMap compileInletMessageMap(const QJsonObject patcherObject, const QStringList& boxTypeList);
   void addRouteMessage(const MaxClass& maxClass);
   void addTypeRouteMessage(const MaxClass& maxClass, const QJsonObject patcherObject);
   Structure::Output& findOrCreateOutput(const int id, const QString& name);
   void readPatcherargs(const QStringList& arguments);

private:
   Block* block;
   QString patchPath;
};

#endif // NOT BlockPatcherH
