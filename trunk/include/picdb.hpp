/******************************************************************
*                picdb.hpp 
*   Sun Nov 21 12:43:41 2004
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

#ifndef PICDB_PICDB_HPP
#define PICDB_PICDB_HPP
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include <string.h>
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <stdlib.h>
#include <stdio.h>
XERCES_CPP_NAMESPACE_USE

/*#include "logger.hpp"
#include "exception.hpp"
#include "warning.hpp"
#include "xml.hpp"
#include "filedata.hpp"
#include "backend.hpp"
#include "config.hpp"
#include "filelist.hpp"
#include "image.hpp"
//#include "db.hpp"
#include "xmllogstream.hpp"
#include "sqlbackend.hpp"*/
//#include "memorymanager.hpp"
namespace Picdb
{
  namespace XML
  {
  }
  namespace Errors
  {
  }
  int Init();
  void Shutdown();
  class MemoryManager; //Should not be used in Logger, Exception, Warning and LogStream derived classes.
  class Logger;
  class LogStream;
  class LogLevel;
  class Exception;
  class ErrorType;
  class Warning;
  class FileData;
  class Filelist;
  class Backend;
  class Config;
  class Image;
  class DB;
  class XmlLogStream;
  class XhtmlLogStream;
  class SqlBackend;
}
#endif
