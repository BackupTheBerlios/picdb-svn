#include "../include/memorymanager.hpp"
//#include "../include/picdb.hpp"
#include "../include/os.hpp"
#ifdef __unix__
#include "../include/unixos.hpp"
#endif

int Picdb::Init()
{
  MManager = new MemoryManager;
#ifdef __unix__
  new UnixOS;
  //ConfigFile=new Config("config.xml");
#else
#error unkown os
#endif
  return 0;
}

void Picdb::Shutdown()
{
  if (MManager)
    delete MManager;
  MManager=NULL;
  //if (ConfigFile)
  //  delete ConfigFile;
  //ConfigFile=NULL;
  if (OS::Instance)
    delete OS::Instance;
  OS::Instance=NULL;
}
