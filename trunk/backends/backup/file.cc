/******************************************************************
*                file.cc 
*   Sun Nov 21 15:47:29 2004
*   Copyright 2004 Jens Krafczyk
*   segv@gmx.de
******************************************************************/

/******************************************************************
* 
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
* 
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Library General Public License for more details.
* 
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* 
******************************************************************/


#include "../include/file.hpp" 
Picdb::File::File (char *basedir)
{
  Log.Log (Log_Debug, __FILE__, __LINE__, "file_backend", "Creating backend.");
  if (!basedir)
    {
      Log.Log (Log_Warning, __FILE__, __LINE__, "file_backend", "No base directory given");
      mBasedir=MManager->strdup(".");
    }
  else
    {
      Log.Log (Log_Info, __FILE__, __LINE__, "file_backend", "Using %s as base directory.", basedir);
      mBasedir=MManager->strdup(basedir);
    }  
  Log.Log (Log_Debug, __FILE__, __LINE__, "file_backend", "Ready.");
}

Picdb::File::~File ()
{
  Log.Log (Log_Debug, __FILE__, __LINE__, "file_backend", "Deleting backend.");
  //delete mBasedir;
  MManager->free(mBasedir);
  Log.Log (Log_Debug, __FILE__, __LINE__, "file_backend", "Backend deleted.");
}

Picdb::Filelist *Picdb::File::GetFiles()
{
  char *dscpath=(char*)MManager->alloc (MManager->strlen (mBasedir)+16);
  dirent *ent;
  //std::list<char*> *list;
  Filelist *list;

  dscpath = MManager->strcpy (dscpath, mBasedir);
  dscpath = MManager->strcat (dscpath, "/Descriptions");
  Log.Log (Log_Debug, __FILE__, __LINE__, "file_backend", "Opening directory %s", dscpath);
  DIR *d=opendir (dscpath);
  free (dscpath);
  if (!d)
    {
      Log.Log (Log_Error, __FILE__, __LINE__, "file_backend", "Error: %s", strerror(errno));
      return NULL;
    }
  list=new Filelist;//std::list<char*>;
  while ((ent=readdir (d)))
    {
      struct stat s;
      char *fname=(char*)MManager->alloc (MManager->strlen(mBasedir)+16+MManager->strlen(ent->d_name));
      fname=MManager->strcpy (fname, mBasedir);
      fname=MManager->strcat (fname, "/Descriptions/");
      fname=MManager->strcat (fname, ent->d_name);
      stat (fname, &s);
      //printf ("%s\n", fname);
      MManager->free (fname);
      if (S_ISREG (s.st_mode) && strlen(ent->d_name)==32)
        list->push_back (MManager->strdup(ent->d_name));
    }
  Log.Log (Log_Debug, __FILE__, __LINE__, "file_backend", "Closing directory.");
  closedir (d);
  return list;
}

Picdb::FileData *Picdb::File::GetFileData (char *fmd5)
{
  FILE *fp;
  char *fname;
  char *data;
  FileData *fdta;
  unsigned long int dsize;
  fname=(char*)malloc (strlen(mBasedir)+16+strlen(fmd5));
  strcpy(fname, mBasedir);
  strcat (fname, "/Descriptions/");
  strcat (fname, fmd5);
  fp=fopen (fname, "r");
  free (fname);
  if (!fp)
    {
      throw new Picdb::Exception (Picdb::Errors::FileNotFoundError, Picdb::Log_Error, __FILE__, __LINE__, "file_backend", "Error trying to open file: %s", strerror(errno));
    }
  fseek (fp, 0L, SEEK_END);
  dsize=ftell(fp);
  data = (char*)malloc(dsize+1);
  rewind (fp);
  memset (data,0,dsize+1);
  if (fread (data, 1, dsize, fp)<dsize)
    {
      new Picdb::Warning (Picdb::Errors::Error, __FILE__, __LINE__, "file_backend", "%s", strerror(errno));
    }
  fclose(fp);
  fdta=new FileData(data);
  free(data);
  return fdta;
}

Picdb::Image *Picdb::File::GetImage (char *fmd5)
{
  FILE *fp;
  char *fname;
  void *data;
  Image *img;
  unsigned long dsize;
  fname=(char*)malloc (strlen(mBasedir)+2+strlen(fmd5));
  strcpy (fname, mBasedir);
  strcat (fname, "/");
  strcat (fname, fmd5);
  fp=fopen(fname,"rb");
  free(fname);
  if (!fp)
    PicdbError(Errors::Error, Log_Error, "file_backend", "Error trying to open file: %s", strerror(errno));
  fseek(fp, 0L, SEEK_END);
  dsize=ftell(fp);
  data=malloc(dsize);
  rewind(fp);
  if (fread (data, 1, dsize, fp)<dsize)
    PicdbWarning (Errors::Error, "file_backend", "%s", strerror(errno));
  fclose(fp);
  img = new Image (data, dsize);
  free(data);
  return img;
}

Picdb::Image *Picdb::File::GetThumbnail(char *fmd5)
{
  char *ffname=(char*)malloc (strlen(fmd5)+strlen("Thumbnails/")+1);
  Image *i;
  strcpy (ffname, "Thumbnails/");
  strcat (ffname, fmd5);
  i = GetImage (ffname);
  free (ffname);
  return i;
}


Picdb::Backend *LoadBackend(char *name, int argc, char **argv)
{
  if (strcmp (name, "File:1s") || argc != 1)
    {
      //Picdb::Log.Log (Picdb::Log_Error, __FILE__, __LINE__, "file_factory", "Requesting Invalid backend signature.");
      //return NULL;
      throw new Picdb::Exception (Picdb::Errors::Error, Picdb::Log_Error, __FILE__, __LINE__, "file_factory", "Requesting Invalid backend signature: %s", name);
    }
  //char *bdir;
  return new Picdb::File(argv[0]);
}

void UnloadBackend (Picdb::Backend *b, char *name, int argc, char **argv)
{
  if (strcmp (name, "File:1s"))
    {
      //Picdb::Log.Log (Picdb::Log_Error, __FILE__, __LINE__, "file_factory", "Requesting Invalid backend signature.");
      //return;
      throw new Picdb::Exception (Picdb::Errors::Error, Picdb::Log_Error, __FILE__, __LINE__, "file_factory", "Requesting Invalid backend signature: %s", name);
    }
  delete (Picdb::File*) b;
}
