/******************************************************************
*                memorymanager.hpp 
*   Thu Dec  2 15:15:32 2004
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


#ifndef PICDB_MEMORYMANAGER_HPP
#define PICDB_MEMORYMANAGER_HPP
#include "picdb.hpp"
#include "exception.hpp"
#include "warning.hpp"
#include <stdarg.h>
#include <stdlib.h>
//#include <hash_map>
#include <map.h>
#include <list>
namespace Picdb
{
  class MemoryManager;
  extern MemoryManager *MManager;
}

/**
 *This class should emulate the most important 'str*' and 'mem*' - functions, plus malloc, realloc, free and some printf-like functions.
 *In contrary to the default functions, the functions provided here should never cause a segmentation fault/buffer overflow/whatever, at lest, if they get
 *a pointer allocated with MemoryManager::alloc(). Instead, they should take the needed measures to cause the requested operation to complete successfully
 *(e.g. allocate more memory for a string operation etc.). Additionally, the destructor checks for unfree'd memory and logs warnings containing information on
 *where the memory was allocated and how much memory was leaked.
 */
class Picdb::MemoryManager
{
public:
  MemoryManager ();
  //!==malloc(s);
  void *alloc (size_t s, void *fcntaddr=NULL, const char *file=NULL, const int line=0, const char *func=NULL);
  //!==free(p)
  void free (void *p, void *fcntaddr=NULL, const char *file=NULL, const int line=0, const char *func=NULL);
  //!==realloc (p,ns)
  void *realloc (void *p, size_t ns);
  //!Returns the Size of an allocated block at *block
  size_t BlockSize (const void *block, int exact_ptr=1);

  //!==strlen(s+start)
  size_t strlen(const char *s, int start=0);
  //!==strdup(s+start)
  char *strdup (const char *s, int start=0);
  //!==strcat(dest,src)
  char *strcat (char *dest, const char *src);
  //!==strncat(dest,src,n);
  char *strncat (char *dest, const char *src, size_t n);
  //!==strcpy(dest,src)
  char *strcpy (char *dest, const char *src);
  //!==strncpy(dest,src)
  char *strncpy (char *dest, const char *src, size_t n);
  int strcmp (const char *s1, const char *s1);
  int strncmp (const char *s1, const char *s2, size_t n);
  int strcasecmp (const char*, const char *);
  int strncasecmp (const char *, const char *, size_t);
  char *index (const char *, char);
  char *rindex (const char*, char);
  char *strpbrk (const char*, const char*);
  char *strstr (const char*, const char*);

  int memcmp (const void *, const void *, size_t);
  void memset (void *, char, size_t);
  void *memcpy (void *dest, const void *src, size_t n);
  
  int sprintf (char *, char *, ...);
  int asprintf (char **, char *, ...);
  int vsprintf (char *, char *, va_list);
  int vasprintf (char **, char *, va_list);
  virtual ~MemoryManager (); 
protected:
  typedef struct
  {
    void *address;
    size_t size;
    char *file;
    int line;
    char *fctn;
    void *alloc_from;
    int freed;
  } ptrinfo_t;
  map <void*, ptrinfo_t*> mMemory;
};
#define PicdbAlloc(s) ::Picdb::MManager->alloc ( s , NULL, __FILE__, __LINE__,  __PRETTY_FUNCTION__)
#define PicdbFree(p) ::Picdb::MManager->free ( p , NULL, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif
