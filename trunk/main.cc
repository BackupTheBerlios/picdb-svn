#include <stdio.h>
#include <logger.hpp>
#include <warning.hpp>
#include <xmllogstream.hpp>
#include <list>
#include <memorymanager.hpp>
#include <config.hpp>
#include <filelist.hpp>
#include <os.hpp>
#include <filedata.hpp>
#include <backend.hpp>


using namespace Picdb;
//(void*)(Picdb::Backend *GetBackend(char* name,...)) a;
/*typedef Backend*
  (*Factory_t)
  (
   char*,
   ...
   )
;*/
/*Factory_t be;
  Destructor_t kbe;*/

void LogTest()
{
  LogMessage (Log_Debug, "main", "test");
  LogMessage (Log_Info, "main", "test");
  LogMessage (Log_Warning, "main", "test");
  LogMessage (Log_Error, "main", "test");
  LogMessage (Log_Critical, "main", "test");
} 


//int mm_main (int agrc, char **argv)
//{
// void *p;
//  char *str, *xstr;
//  Log.AddStream (new TerminalLogStream);
//  Picdb::MemoryManager *mm;
//  Picdb::MManager = new Picdb::MemoryManager;
//  mm=Picdb::MManager;
  //mm->alloc (1024);
  //mm->alloc (2048);
  //mm->free (mm->alloc(4096));
  //p=mm->alloc(8192);
  //mm->free(p);
  //mm->free(p);
  //PicdbAlloc(1024);
  //PicdbAlloc(2048);
  //PicdbFree(PicdbAlloc(4096));
  //p=PicdbAlloc (8192);
  //printf ("%i, %i\n", mm->BlockSize(p), mm->BlockSize((char*)p+1));
  //p=mm->realloc (p, 0x400*64);
  //mm->realloc (malloc(1024), 0x400*1024);
  //printf ("%i, %i\n", mm->BlockSize(p,0), mm->BlockSize((char*)p+1,0));
  //PicdbFree(p);
  //printf ("%i, %i\n", mm->BlockSize(p,0), mm->BlockSize((char*)p+1,0));
  //PicdbFree(p);
  //str=(char*)PicdbAlloc(2);
  /*str=(char*)mm->strdup ("hello, world", 5);
  printf ("%s\n%i\n", str, mm->BlockSize(str));

  xstr=(char*)mm->strdup(str);
  printf ("%s\n%i\n", xstr , mm->BlockSize(xstr));

  xstr=(char*)realloc (xstr, 1024);
  printf ("%s\n%i\n", xstr, mm->BlockSize(xstr));

  str =mm->strcat (str, mm->strdup ("01234567890123467980123456789"));
  printf ("%s\n%i\n", str, mm->BlockSize(str)); */

  //str=mm->strdup ("HELL");
  //str=mm->strncat (str, ",World", 1);
  //xstr=mm->strdup ("HeLlo");
  //printf ("%s == %s ?\n", str, xstr);
  // printf ("%i\n", mm->strncasecmp (str, xstr, 5));
//  str=mm->strdup("Hell");
  //xstr=mm->strdup ("Hell");
//  xstr=(char*)mm->alloc(512);
  //xstr=(char*)mm->memcpy (xstr, str, 512);
//  xstr=mm->strncpy (xstr, str, 3);
  //printf ("%i\n", mm->rindex (str,'l')-str);
  //printf ("%s\n", mm->rindex (str,'l'));
//  printf ("%s==%s\n%i\n", str,xstr,mm->memcmp (str, xstr,5));
//  delete (Picdb::MManager);
//}

int config_main (int argc, char **argv)
{
  Log.AddStream (new TerminalLogStream);
  Init();
  Config *cfg =new Config("config.xml");
  Picdb::Config::list_t keys;
  Picdb::Config::list_t::iterator iter;
  keys=cfg->Keys();
  for (iter=keys.begin();iter!=keys.end();iter++ )
    {
      printf ("%s\n    ", *iter);
      printf ("%s\n", (*cfg)[*iter]);
    }
  delete cfg;
  Shutdown();
}

int filedata_main (int argc, char **argv)
{
  Log.AddStream (new TerminalLogStream);
  Init();
  //FILE *fp=fopen ("00004f3f929ab968ecf293bc6c16eaef","r");
  FILE *fp=fopen ("14355dbddcdaf68a134a5cd745b11161", "r");
  char *Buffer=(char*)malloc (10240);
  memset (Buffer,0,10240);
  fread (Buffer, 1,10240, fp);
  fclose(fp);
  //printf ("%s", Buffer);
  FileData *fd=new FileData (Buffer);
  printf ("%s\n", (*fd)["comment"]);
  delete fd;
  Shutdown();
}

int main (int argc, char **argv)
{
  Log.AddStream (new TerminalLogStream);
  Init();
  char *v[]={"/home/segfault/Documents/Pictures"};
  Backend *be = LoadBackend ("libpicdbbefile.so", "File:1s", 1, v/*"/home/segfault/Documents/Pictures"*/);
  FileList *fl = be->GetFileList();
  fl->Dump();
  delete fl;
  delete be;
  Shutdown();
}

int lib_main (int argc, char **argv)
{
  Picdb::OS *os;
  Log.AddStream (new TerminalLogStream);
  Init();
  printf ("OS: %p\n", OS::Instance);
  os=OS::Instance;
  void *lhdl = os->OpenLibrary ("libm.so.6");
  float (*c) (float) = (float (*)(float))os->GetLibrarySymbol (lhdl, "cosf");
  printf ("%f\n", c(0.0f));
  os->CloseLibrary (lhdl);
  Shutdown();
}

int filelist_main (int argc, char **argv)
{
  Log.AddStream (new TerminalLogStream);
  Init();
  FileList *l=new FileList;
  l->PushBack ("h");
  l->PushBack ("e");
  l->PushBack ("l");
  l->PushBack ("o");
  l->Dump();
  FileList *j=new FileList;
  (*j)=(*l);
  j->Dump();
  delete j;
  delete l;
  Shutdown();
}
