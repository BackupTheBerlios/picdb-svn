/******************************************************************
*                xml.cc 
*   Wed Dec  1 16:47:18 2004
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


#include "../include/xml.hpp"

int Picdb::XML::XML::smCount;

Picdb::XML::XML::XML ()
{
  if (!smCount)
    {
      try
	{
	  LogMessage (Log_Info, "XML", "Initialising Xerces-C.");
	  XMLPlatformUtils::Initialize();
	}
      catch (const XMLException &e)
	{
	  PicdbError (Picdb::Errors::XmlError, Log_Critical, "XML", "error while trying to initialise Xerces-C");
	}
    }
  smCount++;
}
Picdb::XML::XML::~XML ()
{
  smCount--;
  if (!smCount)
    {
    }
}
