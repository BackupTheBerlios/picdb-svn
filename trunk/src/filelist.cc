/******************************************************************
*                filelist.cc 
*   Sun Dec 19 13:21:22 2004
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


#include "../include/filelist.hpp"
Picdb::FileList::FileList ()
{}

Picdb::FileList::~FileList ()
{
  Clear();
}


Picdb::FileList::FileList (FileList &that)
{
  (*this)=that;
}

Picdb::FileList &Picdb::FileList::operator --()
{
  i--;
  return *this;
}

Picdb::FileList &Picdb::FileList::operator ++()
{
  i++;
return *this;
}

Picdb::FileList &Picdb::FileList::operator --(int)
{
  i--;
return *this;
}


Picdb::FileList &Picdb::FileList::operator ++(int)
{
  i++;
  return *this;
}

Picdb::FileList &Picdb::FileList::operator =(Picdb::FileList &that)
{
  Clear();
  for (that.ResetBegin();!that.IsEnd();that++)
    {
      char *scp=(char*)PicdbAlloc (MManager->strlen (*that)+1);
      scp=MManager->strcpy (scp,*that);
      l.push_back (scp);
    }
  return *this;
}

int Picdb::FileList::IsEnd(){return i==l.end();}

int Picdb::FileList::IsBegin(){return i==l.begin();}

void Picdb::FileList::ResetBegin(){i=l.begin();}

void Picdb::FileList::ResetEnd(){i=l.end();}

char *Picdb::FileList::operator *(){return *i;}

void Picdb::FileList::PushBack(const char *s)
{
  char *scp=(char*)PicdbAlloc (MManager->strlen(s)+1);
  scp=MManager->strcpy (scp,s);
  l.push_back (scp);
}

unsigned long int  Picdb::FileList::Size()
{
  return l.size();
}

void Picdb::FileList::Dump()
{
  std::list<char*>::iterator c;
  for (c=l.begin(); c!=l.end(); c++)
    printf ("%s\n", *c);  
}

void Picdb::FileList::Clear()
{
  for (ResetBegin();!IsEnd();(*this)++)
    {
      PicdbFree (**this);
    }
  l.clear();
}

void Picdb::FileList::Sort()
{
  l.sort();
}
