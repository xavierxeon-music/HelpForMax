#ifndef LockH
#define LockH

class Lock
{
public:
   Lock(bool& variable);
   ~Lock();

private:
   bool& variable;
};

#endif // NOT LockH
