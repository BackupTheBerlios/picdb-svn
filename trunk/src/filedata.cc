/******************************************************************
*                filedata.cc 
*   Thu Dec 30 16:52:01 2004
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


#include "../include/filedata.hpp"
Picdb::FileData::FileData (const char *lines)
{
  const char *clineptr=lines, *tempptr=lines;
  size_used=0;
  blocksize=MManager->BlockSize (lines, 1);
  while (*clineptr)
    {
      ParseLine (clineptr);
      tempptr=clineptr;
      while (*tempptr!='\n' && *tempptr)
	{
	  tempptr++;
	  if (blocksize && lines-tempptr>=blocksize)
	    {
	      tempptr--; 
	      break;
	    }
	}
      if (blocksize && lines-clineptr>=blocksize)
	break;
      clineptr=lines+size_used;//tempptr+1;
    }
  //printf ("%i lines parsed.\n", nlines);
}

Picdb::FileData::~FileData ()
{
  hash_map<const char*, char *, hash<const char*>, eqstr>::iterator iter;
  for (iter=mData.begin(); iter!=mData.end(); iter++)
    {
      if ((*iter).second)
	PicdbFree ((*iter).second);
    }
}

char *&Picdb::FileData::operator[] (const char *k)
{
  return mData[k];
}

void Picdb::FileData::ParseLine (const char *line)
{
  char *key=NULL, *val=NULL;
  const char *p=line;
  const char *kstart=NULL, *vstart=NULL;
  size_t klen=0, vlen=0;
  int st=0; //0: before key, 1: in key, 2:between colon and value, 3: in value
  while (*p && *p!='\n')
    {
      size_used++;
      if (*p=='\r' && *(p+1)=='\n')
	{
	  size_used+=2;
	  p+=2;
	  continue;
	}
      else if (*p=='\r')
	{
	  size_used++;
	  p++;
	  continue;
	}
      switch (st)
	{
	case 0:
	  if (*p>32)
	    {
	      st=1;
	      kstart=p;
	    }
	  else
	    break;
	case 1:
	  if (*p==':')
	    {
	      klen--;
	      p++;
	      st=2;
	    }
	  else
	    {
	      klen++;
	      break;
	    }
	case 2:
	  if (*p>32)
	    {
	      st=3;
	      vstart=p;
	    }
	  else
	    break;
	case 3:
	  vlen++;
	  break;
	}
      p++;
      if (blocksize && size_used>=blocksize)
	break;
    }
  size_used++;
  if (!kstart || !klen)
    return;
  //i'll continue using the default malloc/free functions for keys, since they tned to be unfree'able.
  //\todo we need a non-stl-hash-class which makes it possible to free the keys again. this approach is *not* good.
  //key=(char*)MManager->alloc (klen+1);
  //key=(char*)MManager->memcpy (key, kstart, klen);
  key=(char*)malloc(klen+2);
  memset(key,0,klen+2);
  key=(char*)memcpy (key, kstart,klen+1);
  
  //st=0;//0=in trailing whitespace, 1 in key
  size_t i;
  for (i=klen+1; i>0; i--)
    {
      if (key[i]<=32)
	{
	  key[i]=0;
	  continue;
	}
      break;
    } 
  //printf ("%s\n", key);
  if (vstart && vlen)
    {
      vlen++;
      val=(char*)PicdbAlloc (vlen+1);
      val=(char*)MManager->memcpy (val, vstart, vlen);
      for (i=vlen+1; i>0; i--)
	{
	  if (val[i]<=32)
	    {
	      val[i]=0;
	      continue;
	    }
	  break;
	}
    }
  mData[key]=val;
}
