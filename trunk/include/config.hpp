/******************************************************************
*                config.hpp 
*   Sun Nov 21 20:40:54 2004
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


#ifndef PICDB_CONFIG_HPP
#define PICDB_CONFIG_HPP
//#include <picdb.hpp>
#include "xml.hpp"
//#include "filelist.hpp"
#include "exception.hpp"
#include "warning.hpp"
#include "logger.hpp"
#include "memorymanager.hpp"
#include "picdb.hpp"
#include <hash_map.h>

namespace Picdb
{
  class Config;
  namespace XML
  {
    namespace Config
    {
      class StrX;
      class CfgSaxHandler;
    }
  }
  extern Picdb::Config *ConfigFile;
}
class Picdb::XML::Config::StrX
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char*   fLocalForm;
};

class Picdb::XML::Config::CfgSaxHandler: public DefaultHandler
{
public:
  CfgSaxHandler (Picdb::Config *c){cfg=c;}
  virtual void 	endElement (const XMLCh *const uri, const XMLCh *const localname, const XMLCh *const qname);
  virtual void 	startElement (const XMLCh *const uri, const XMLCh *const localname, const XMLCh *const qname, const Attributes &attrs);
  virtual void 	error (const SAXParseException &exc);
  virtual void 	fatalError (const SAXParseException &exc);
  virtual void 	warning (const SAXParseException &exc);
protected:
  void AddOption (Picdb::XML::Config::StrX *v, char *suffix=NULL);
  void AddOption (char *v, char *suffix=NULL);
  void AddConfig (Picdb::XML::Config::StrX *cfgfilename);
  Picdb::Config *cfg;
  std::list<char *> mElemPath;
};

class Picdb::Config
{
  friend class Picdb::XML::Config::CfgSaxHandler;
public:
  typedef std::list<const char*> list_t ;
  Config (const char *cfgfilename);
  virtual ~Config (); 
  char *operator[](const char*);
  const list_t &Keys();
protected:
  struct eqstr
  {
    bool operator()(const char* s1, const char* s2) const
    {
      return strcmp(s1, s2) == 0;
    }
  };
  int success;
  Picdb::XML::XML xml_instance;
  hash_map<const char*, char *, hash<const char*>, eqstr> mData;
  //std::list <const char*> mKeys;
  list_t mKeys;
};
#endif
