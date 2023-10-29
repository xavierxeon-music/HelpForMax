#ifndef BlockHelpH
#define BlockHelpH

#include "Block.h"

#include <QString>

class Block::Help
{
public:
   Help(Block* block);

public:
   void write();

private:
   Block* block;
   QString helpPath;
};

#endif // NOT BlockHelpH
