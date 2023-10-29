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
   Structure::Output& findOrCreateOutput(const int id);
   void readPatcherargs(QString text);

private:
   Block* block;
   QString patchPath;
};

#endif // NOT BlockPatcherH
