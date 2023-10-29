#ifndef BlockInitFileH
#define BlockInitFileH

#include "Block.h"

class Block::InitFile
{
public:
   InitFile(Block* block);

public:
   void write();

private:
   Block* block;
   QString initPath;
};

#endif // NOT BlockInitFileH
