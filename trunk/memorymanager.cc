/******************************************************************
*                memorymanager.cc 
*   Thu Dec  2 15:15:31 2004
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


#include "../include/memorymanager.hpp"

Picdb::MemoryManager *Picdb::MManager;

Picdb::MemoryManager::MemoryManager ()
{
  LogMessage (Log_Info, "MemoryManager", "Initialising");
}

Picdb::MemoryManager::~MemoryManager ()
{
  std::map <void *, ptrinfo_t *>::iterator iter;
  size_t LeakSize=0;
  LogMessage (Log_Info, "MemoryManager", "Shutting down.");
  for (iter=mMemory.begin();iter!=mMemory.end();iter++)
    {
      if ((*iter).second==NULL)
	continue;
      if (!(*iter).second->freed)
	{
	  PicdbWarning (Errors::Error, "MemoryManager", "Memory leak from %p (%s:%i)(%s): %i bytes at %p not freed", (*iter).second->alloc_from, (*iter).second->file, (*iter).second->line, (*iter).second->fctn, (*iter).second->size, (*iter).second->address);
	  LeakSize += (*iter).second->size;
	  ::free ((*iter).first);
	}
      ::free ((*iter).second->file);
      ::free ((*iter).second->fctn);
      ::free ((*iter).second);
    }
  if (LeakSize)
    {
      int mb=LeakSize/(1024*1024);
      LeakSize=LeakSize%(1024*1024);
      int kb=LeakSize/1024;
      LeakSize=LeakSize%1024;
      PicdbWarning (Errors::Error, "MemoryManager", "Total leak-size: %iMB %iKB %iB", mb, kb, LeakSize);
    }
}

/**
 *@param s how many bytes to allocate
 *@param fcntaddr the address of the calling function (for debugging purposes)
 *@param file the file where 'free' was called (...)
 *@param line the line where 'free' was called (...)
 *@param func the function where 'free' was called (...)
 *@return NULL on error, address of the allocated block otherwise
 */
void *Picdb::MemoryManager::alloc (size_t s, void *fcntaddr, const char *file,const  int line, const char *func)
{
  void *block=::malloc(s);
  ptrinfo_t *info = new ptrinfo_t;
  info->address=block;
  info->size=s;
  if (!fcntaddr)
    fcntaddr = __builtin_return_address (0);
  if (file)
    info->file=::strdup(file);
  else
    info->file=::strdup("(unknown)");
  info->line=line;
  if (func)
    info->fctn=::strdup(func);
  else
    info->fctn=::strdup("(unknown)");
  info->alloc_from=fcntaddr;
  info->freed=0;
  mMemory[block]=info;
  ::memset (block, 0, s);
  return block;
}

/**
 *@param block the block to be free'd
 *@param fcntaddr the address of the calling function (for debugging purposes)
 *@param file the file where 'free' was called (...)
 *@param line the line where 'free' was called (...)
 *@param func the function where 'free' was called (...)
 */
void Picdb::MemoryManager::free (void *block, void *fcntaddr, const char *file, const int line, const char *func)
{
  ptrinfo_t *info = mMemory[block];
  fcntaddr = __builtin_return_address (0);
  if (!info)
    {
      PicdbWarning (Errors::Error, "MemoryManager", "Trying to free unallocated block %p. Caller is %p. (%s:%i:%s)", block, fcntaddr, file, line, func);
      return;
    }
  if (info->freed)
    {
      PicdbWarning (Errors::Error, "MemoryManager", "Trying to free block %p which was already free'd. Caller is %p (%s:%i:%s)", block, fcntaddr, file, line, func);
      return;
    }
  ::free (block);
  info->freed=1;
}

/**
 *@param str the string to retrieve the length from
 *@param start where to begin counting (0 to use the complete string)
 *@return 0 on error, the length of the (remaining) string otherwise
 */
size_t Picdb::MemoryManager::strlen (const char *str, int start)
{
  const char *p;
  if (!str)
    return 0;
  if (!mMemory[(void*)str])
    return ::strlen (str+start); //we don't have any information about the given memory address, so we can as well use the original functions
  ptrinfo_t *info = mMemory[(void*)str];
  for (p=str+start; (unsigned int)(p-str)<=info->size && *p; p++)
    {}
  //if (p-str>info->size)
  //  return 0;
  return (size_t)(p-str-start);
}

/**
 *@param str the string to duplicate
 *@param start first char to duplicate (0 to duplicate the complete string)
 *@return NULL on error, a pointer to the new string otherwise
 */
char * Picdb::MemoryManager::strdup (const char *str, int start)
{
  char *dest;
  ptrinfo_t *info, *dinfo;
  if (!str)  //we got NULL pointer. this would cause a crash in the next block, so we return now.
    return NULL;
  info = mMemory[(void*)str];
  if (!info) //the source pointer doesn't belong to us. just try to duplicate it using the default strcpy but register the copy for us.
    {
      dest=::strdup (str+start);
      dinfo=new ptrinfo_t;
      dinfo->address=dest;
      dinfo->size = ::strlen(dest)+1;
      dinfo->file=::strdup("(unknown)");
      dinfo->line=0;
      dinfo->fctn=::strdup("(unknown)");
      dinfo->alloc_from=__builtin_return_address (0);
      dinfo->freed=0;
      mMemory[(void*)dest] = dinfo;
      return dest;
    }
  //We've got one of our own blocks and can do full error checking... :)
  if (info->freed)
    {
      PicdbWarning (Errors::Error, "MemoryManager", "Trying to get a duplicate of an already free'd string...");
      return NULL;
    }
  size_t memsize = info->size - start;
  if (info->size-start <=0)
    {
      PicdbWarning (Errors::Error, "MemoryManager", "Invalid index given. Block size is %li, index is %li", info->size, start);
      return NULL;
    }
  dest = (char*)alloc (memsize+1);
  ::memcpy (dest, str, memsize);
  dinfo=new ptrinfo_t;
  dinfo->address=dest;
  dinfo->size=memsize+1;
  dinfo->file=::strdup("(unknown)");
  dinfo->line=0;
  dinfo->fctn=::strdup("(unknown)");
  dinfo->alloc_from=__builtin_return_address (0);
  dinfo->freed=0;
  mMemory[(void*)dest]=dinfo;
  return dest;
}

/**
 *@param p the address of the block to retrieve the size from
 *@param exact_ptr if nonzero, the pointer must match an allocated block exactly, if zero, the pointer can be any address in an allocated block
 *@return 0 on error, the size of the block if the pointer matches exactly, the remaining size of the block, counted from the given pointer
 */
size_t Picdb::MemoryManager::BlockSize (void *p, int exact_ptr)
{
  ptrinfo_t *info=mMemory[p];
  if (exact_ptr)
    {
      if (!info)
	return 0;
      if (info->freed)
	{
	  PicdbWarning (Errors::Error, "MemoryManager", "Trying to check blocksize of already free'd block %p.", p);
	  return 0;
	}
      return info->size;
    }
  else
    {
      if (info && !info->freed)
	return info->size;
      //LogMessage (Log_Debug, "MemoryManager", "Fuzzy checking for ptr %p.", p);
      std::map <void*, ptrinfo_t*>::iterator iter;
      for (iter=mMemory.begin();iter!=mMemory.end();iter++)
	{
	  if (!(*iter).second)
	    continue;
	  //if ((*iter).second->freed)
	  //  continue;
	  info = (*iter).second;
	  void *start=info->address;
	  void *end=((char*)info->address)+info->size;
	  //LogMessage (Log_Debug, "MemoryManager", "checking ptr %p (%i)...", info->address, info->size);
	  if (p>=start && p<end && !info->freed)
	    return (info->size - ((char*)p-(char*)start));
	  else if (info->freed)
	    PicdbWarning (Errors::Error, "MemoryManager", "Trying to check blocksize of already free'd block %p ?", info->address);
	}
    }
  return 0;
}

/**
 *@param p pointer of block to reallocate
 *@param ns new size of the block
 *@return NULL on error, pointer to the new block otherwise
 *\bug doesn't always change the 'size' field for the newly allocated block. 
 */
void *Picdb::MemoryManager::realloc (void *p, size_t ns)
{
  void *np;
  if (!p)
    return NULL;
  if (!mMemory[(void*)p])
    {
      np= ::realloc (p, ns);
      ptrinfo_t *info = new ptrinfo_t;
      info->size=ns;
      info->file=strdup("(unknown)");
      info->fctn=strdup("(unknown)");
      info->line=0;
      info->alloc_from=NULL;
      info->freed=0;
      mMemory[np]=info;
      return np;
    }
  ptrinfo_t *info=mMemory[(void*)p];
  if (info->size >= ns || ns<1)
    return p;
  np=::realloc (p,ns);
  if (np!=p)
    {
      info->freed=1;
      ptrinfo_t *oi=info;
      info = new ptrinfo_t;
      info->size=ns;
      info->file = ::strdup (oi->file);
      info->line = oi->line;
      info->fctn = ::strdup (oi->fctn);
      info->alloc_from = oi->alloc_from;
      info->freed=0;
      info->address=np;
      mMemory[np]=info;
      //PicdbWarning (Errors::Error, "MemoryManager", "reallocated block %p(%i)->%p(%i)", p,oi->size, np, ns);
    }
  else
    {
      mMemory[np]->size=ns;
    }
  return np;  
}

/**
 *@param dest the string to which 'src' should be appended
 *@param src the string to append to 'dest'
 *@return NULL on error, 'dest' otherwise
 */
char *Picdb::MemoryManager::strcat (char *dest, const char *src)
{
  unsigned long int index=0;
  unsigned long int dindex=0;
  ptrinfo_t *sinfo, *dinfo;
  sinfo = mMemory[(void*)src];
  dinfo = mMemory[(void*)dest];
  if (!dinfo && !sinfo) //neither source address nor destination address can be checked. no need to try any tricks here, just let ::strcat do it's job...
    {
      ::strcat (dest, src);
      return dest;
    }
  dindex = strlen(dest);
  while (src[index])
    {
      if (dinfo && index+dindex > dinfo->size)
	{
	  dest = (char*)realloc (dest, dinfo->size+256);
	}
      if (sinfo && index > sinfo->size)
	{
	 dest[index]=0;
	 break;
	}
      dest[index+dindex]=src[index];
      index++;
    }
  return dest;
}

/**
 *@param dest the string to which 'src' should be appended
 *@param src the string to append to 'dest'
 *@param n copy at most this many character
 *@return NULL on error, 'dest' otherwise.
 */
char *Picdb::MemoryManager::strncat (char *dest, const char *src, size_t n)
{
  unsigned long int index=0;
  unsigned long int dindex=0;
  ptrinfo_t *sinfo, *dinfo;
  if (!n)
    return dest; //we do not seem to want to copy anything... 
  sinfo = mMemory[(void*)src];
  dinfo = mMemory[(void*)dest];
  if (!dinfo && !sinfo) //neither source address nor destination address can be checked. no need to try any tricks here, just let ::strcat do it's job...
    {
      ::strncat (dest, src, n);
      return dest;
    }
  dindex = strlen(dest);
  while (src[index] && index<=n)
    {
      if (dinfo && index+dindex > dinfo->size)
	{
	  dest = (char*)realloc (dest, dinfo->size+256);
	}
      if (sinfo && index > sinfo->size)
	{
	 dest[index+dindex]=0;
	 break;
	}
      dest[index+dindex]=src[index];
      index++;
    }
  dest[index+dindex]=0;
  return dest;
}

/**
 *@param s1
 *@param s2
 *@return <0 if s1<s2, 0 if s1==s2, >0 if s1>s2
 */
int Picdb::MemoryManager::strcmp (const char *s1, const char *s2)
{
  unsigned long int index=0;
  ptrinfo_t *info1, *info2;
  if (!s1 && s2) //NULL is less than everything else
    return -1;
  if (!s2 && s1) //everything is greater than NULL
    return 1;
  if (!s2 && !s1) //NULL is equal to NULL.
    return 0;
  info1=mMemory[(void*)s1];
  info2=mMemory[(void*)s2];
  if (!info1 && !info2)
    return ::strcmp (s1,s2);
  while (1)
    {
      //printf ("index %i\n", index);
      int s1_end = (s1[index]==0);
      if (info1)
	s1_end |= (index>=info1->size);
      int s2_end = (s2[index]==0);
      if (info2)
	s2_end |= (index>=info2->size);
      if (s1_end && s2_end)
	{
	  //printf ("eqv, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 0; //both strings are equal
	}
      else if (s1_end && !s2_end) //s1 is less than s2 
	{
	  //printf ("lt, index=%i, s1s:%i, s2s:%i, *s1:%02x, *s2:%02x\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return -1;
	}
      else if (!s1_end && s2_end) //s2 is less than s1
	{
	  //printf ("gt, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 1;
	}
      if (s1[index] < s2[index])
	return -1;
      else if (s1[index] > s2[index])
	return 1;
      index++;
    }
}

/**
 *@param s1
 *@param s2
 *@param n compare at most this many bytes.
 *@return <0 if s1<s2, 0 if s1==s2, >0 if s1>s2
 */
int Picdb::MemoryManager::strncmp (const char *s1, const char *s2, size_t n)
{
  unsigned long int index=0;
  ptrinfo_t *info1, *info2;
  if (!s1 && s2) //NULL is less than everything else
    return -1;
  if (!s2 && s1) //everything is greater than NULL
    return 1;
  if (!s2 && !s1) //NULL is equal to NULL.
    return 0;
  info1=mMemory[(void*)s1];
  info2=mMemory[(void*)s2];
  if (!info1 && !info2)
    return ::strcmp (s1,s2);
  while (1)
    {
      //printf ("index %i\n", index);
      if (index==n) //maximum number of chars to compare reached.
	{
	  return 0; //strins are equal so far.
	}
      int s1_end = (s1[index]==0);
      if (info1)
	s1_end |= (index>=info1->size);
      int s2_end = (s2[index]==0);
      if (info2)
	s2_end |= (index>=info2->size);
      if (s1_end && s2_end)
	{
	  //printf ("eqv, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 0; //both strings are equal
	}
      else if (s1_end && !s2_end) //s1 is less than s2 
	{
	  //printf ("lt, index=%i, s1s:%i, s2s:%i, *s1:%02x, *s2:%02x\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return -1;
	}
      else if (!s1_end && s2_end) //s2 is less than s1
	{
	  //printf ("gt, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 1;
	}
      if (s1[index] < s2[index])
	return -1;
      else if (s1[index] > s2[index])
	return 1;
      index++;
    }
}


/**
 *@param s1
 *@param s2
 *@return <0 if s1<s2, 0 if s1==s2, >0 if s1>s2
 */
int Picdb::MemoryManager::strcasecmp (const char *s1, const char *s2)
{
  unsigned long int index=0;
  ptrinfo_t *info1, *info2;
  if (!s1 && s2) //NULL is less than everything else
    return -1;
  if (!s2 && s1) //everything is greater than NULL
    return 1;
  if (!s2 && !s1) //NULL is equal to NULL.
    return 0;
  info1=mMemory[(void*)s1];
  info2=mMemory[(void*)s2];
  if (!info1 && !info2)
    return ::strcmp (s1,s2);
  while (1)
    {
      //printf ("index %i\n", index);
      int s1_end = (s1[index]==0);
      char s1c, s2c;
      if (info1)
	s1_end |= (index>=info1->size);
      int s2_end = (s2[index]==0);
      if (info2)
	s2_end |= (index>=info2->size);
      if (s1_end && s2_end)
	{
	  //printf ("eqv, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 0; //both strings are equal
	}
      else if (s1_end && !s2_end) //s1 is less than s2 
	{
	  //printf ("lt, index=%i, s1s:%i, s2s:%i, *s1:%02x, *s2:%02x\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return -1;
	}
      else if (!s1_end && s2_end) //s2 is less than s1
	{
	  //printf ("gt, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 1;
	}
      s1c = s1[index];
      s2c = s2[index];
      if (s1c >='A' && s1c<='Z')
	s1c += 'a' - 'A';
      if (s2c >='A' && s2c<='Z')
	s2c += 'a' - 'A';
      if (s1c < s2c)
	return -1;
      else if (s1c > s2c)
	return 1;
      index++;
    }
}

/**
 *@param s1
 *@param s2
 *@param n compare at most this many bytes.
 *@return <0 if s1<s2, 0 if s1==s2, >0 if s1>s2
 */
int Picdb::MemoryManager::strncasecmp (const char *s1, const char *s2, size_t n)
{
  unsigned long int index=0;
  ptrinfo_t *info1, *info2;
  if (!s1 && s2) //NULL is less than everything else
    return -1;
  if (!s2 && s1) //everything is greater than NULL
    return 1;
  if (!s2 && !s1) //NULL is equal to NULL.
    return 0;
  info1=mMemory[(void*)s1];
  info2=mMemory[(void*)s2];
  if (!info1 && !info2)
    return ::strcmp (s1,s2);
  while (1)
    {
     if (index==n) //maximum number of chars to compare reached.
	{
	  return 0; //strins are equal so far.
	}
      //printf ("index %i\n", index);
      int s1_end = (s1[index]==0);
      char s1c, s2c;
      if (info1)
	s1_end |= (index>=info1->size);
      int s2_end = (s2[index]==0);
      if (info2)
	s2_end |= (index>=info2->size);
      if (s1_end && s2_end)
	{
	  //printf ("eqv, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 0; //both strings are equal
	}
      else if (s1_end && !s2_end) //s1 is less than s2 
	{
	  //printf ("lt, index=%i, s1s:%i, s2s:%i, *s1:%02x, *s2:%02x\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return -1;
	}
      else if (!s1_end && s2_end) //s2 is less than s1
	{
	  //printf ("gt, index=%i, s1s:%i, s2s:%i, *s1:%i, *s2:%i\n", index, info1->size, info2->size, s1[index], s2[index]);
	  return 1;
	}
      s1c = s1[index];
      s2c = s2[index];
      if (s1c >='A' && s1c<='Z')
	s1c += 'a' - 'A';
      if (s2c >='A' && s2c<='Z')
	s2c += 'a' - 'A';
      if (s1c < s2c)
	return -1;
      else if (s1c > s2c)
	return 1;
      index++;
    }
}
