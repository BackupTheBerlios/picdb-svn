/******************************************************************
*                config.cc 
*   Sun Nov 21 20:40:53 2004
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


#include "../include/config.hpp"
Picdb::Config *Picdb::ConfigFile=NULL;

void Picdb::XML::Config::CfgSaxHandler::endElement (const XMLCh *const uri, const XMLCh *const localname, const XMLCh *const qname)
{
  Picdb::XML::Config::StrX cqname(qname);
  if (!strcmp (cqname.localForm(), "config") || !strcmp (cqname.localForm(), "option"))
  {
    free (mElemPath.back());
    mElemPath.pop_back();
  }
}

void Picdb::XML::Config::CfgSaxHandler::startElement (const XMLCh *const uri, const XMLCh *const localname, const XMLCh *const qname, const Attributes &attrs)
{
  Picdb::XML::Config::StrX *att_name=NULL;
  Picdb::XML::Config::StrX *att_val=NULL;
  Picdb::XML::Config::StrX *att_file=NULL;
  Picdb::XML::Config::StrX cqname(qname);
  unsigned int len = attrs.getLength();
  for (unsigned int index = 0; index < len; index++)
    {
      Picdb::XML::Config::StrX an(attrs.getQName(index));
      if (!strcmp (an.localForm(), "name"))
	att_name = new StrX (attrs.getValue(index));
      if (!strcmp (an.localForm(), "value"))
	att_val = new StrX (attrs.getValue(index));
      if (!strcmp (an.localForm(), "file"))
	att_file = new StrX(attrs.getValue(index));
    }
  if (!strcmp (cqname.localForm(), "config") || !strcmp (cqname.localForm(), "option") && att_name)
    {
      mElemPath.push_back (strdup (att_name->localForm()));
      if (att_name)
	delete att_name;
      if (att_val)
	{
	  AddOption (att_val);
	  delete att_val;
	}
    }
  else if (!strcmp (cqname.localForm(), "include") && att_file)
    {
      //printf ("Trying to include file %s\n", att_file->localForm());
      AddConfig (att_file);
      delete att_file;
    }
  else
    {
      PicdbWarning (Errors::XmlError, "Config", "Invalid xml ? (qname = %s)", cqname.localForm());
    }
  //printf ("%s\n", Picdb::XML::Config::StrX(qname).localForm());
}

void Picdb::XML::Config::CfgSaxHandler::AddOption (Picdb::XML::Config::StrX *v, char *suffix)
{
  std::list<char *>::iterator iter;
  unsigned int namemem=1024;
  unsigned int namelen=1;
  char *optname=(char*)PicdbAlloc(1024);
  optname[0]=0;
  for (iter=mElemPath.begin(); iter!=mElemPath.end(); iter++)
    {
      if (strlen (*iter) + namelen +1 >= namemem)
	optname = (char*)MManager->realloc (optname, namelen+strlen(*iter)+1024);
      if (MManager->strlen (optname))
	MManager->strcat (optname, ".");
      MManager->strcat (optname, *iter);
      namelen += strlen(*iter)+1;
    }
  if (suffix)
    {
      namelen += strlen(suffix);
      if (namelen > namemem)
	optname = (char*)MManager->realloc (optname, namelen+1);
      MManager->strcat (optname, ".");
      MManager->strcat (optname, suffix);
    }
  //printf ("%s:%s\n", optname, v->localForm());
  char *mval=(char*)PicdbAlloc (strlen(v->localForm()));
  MManager->strcpy (mval, v->localForm());
  cfg->mData[optname]=mval;;
  cfg->mKeys.push_back(optname);
  //MManager->free (mval);
  //free(optname);
  //MManager->free(optname);
}

void Picdb::XML::Config::CfgSaxHandler::AddOption (char *v, char *suffix)
{
  std::list<char *>::iterator iter;
  unsigned int namemem=1024;
  unsigned int namelen=1;
  char *optname=(char*)PicdbAlloc(1024);
  optname[0]=0;
  for (iter=mElemPath.begin(); iter!=mElemPath.end(); iter++)
    {
      if (strlen (*iter) + namelen +1 >= namemem)
	optname = (char*)realloc (optname, namelen+strlen(*iter)+1024);
      if (strlen (optname))
	strcat (optname, ".");
      strcat (optname, *iter);
      namelen += strlen(*iter)+1;
    }
  if (suffix)
    {
      namelen += strlen(suffix);
      if (namelen > namemem)
	optname = (char*)realloc (optname, namelen+1);
      if (strlen(optname))
	strcat (optname, ".");
      strcat (optname, suffix);
    }
  //printf ("%s:%s\n", optname, v->localForm());
  if (v)
    {
      cfg->mData[optname]=MManager->strdup (v);
      cfg->mKeys.push_back(optname);
      free(v);
    }
  //free(optname);
}

void Picdb::XML::Config::CfgSaxHandler::AddConfig (Picdb::XML::Config::StrX *cfgfilename)
{
  Picdb::Config *icfg;
  try
    {
      icfg = new Picdb::Config (cfgfilename->localForm());
    }
  catch (Exception *e)
    {
      delete e;
      return;
    }
  Picdb::Config::list_t keys;
  Picdb::Config::list_t::iterator iter;
  keys=icfg->mKeys;
  for (iter=keys.begin(); iter!=keys.end(); iter++)
    {
      //printf ("%s:%s\n", *iter, icfg->mData[*iter]);
      AddOption (icfg->mData[*iter], (char*)*iter);
    }
}

void Picdb::XML::Config::CfgSaxHandler::error (const SAXParseException &exc)
{}
void Picdb::XML::Config::CfgSaxHandler::fatalError (const SAXParseException &exc)
{}
void Picdb::XML::Config::CfgSaxHandler::warning (const SAXParseException &exc)
{}



Picdb::Config::Config (const char *cfgfilename)
{
  success=1;
  LogMessage (Log_Info, "Config", "Trying to create configuration from file %s", cfgfilename);
  try
    {
      SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
      Picdb::XML::Config::CfgSaxHandler handler(this);
      parser->setContentHandler(&handler);
      parser->setErrorHandler(&handler);
      parser->parse(cfgfilename);
    }
  catch (...)
    {
      PicdbError (Errors::XmlError, Log_Error, "Config", "Failure while trying to parse configuration.");
      success=0;
    }
  if (success)
    LogMessage (Log_Info, "Config", "Successfully parsed configuration with %i options.", mKeys.size());
}

Picdb::Config::~Config ()
{
  if (!success)
    return;
  hash_map<const char*, char *, hash<const char*>, eqstr>::iterator iter;
  for (iter=mData.begin(); iter!=mData.end(); iter++)
    {
      PicdbFree ((*iter).second);
      //free ((char*)(*iter).first);
    }
  Picdb::Config::list_t::iterator kiter;
  for (kiter=mKeys.begin(); kiter!=mKeys.end(); kiter++)
    PicdbFree ((void*)*kiter);
}

char *Picdb::Config::operator[](const char *key)
{
  if (success)
    return mData[key];
  return NULL;
}

const std::list<const char*> &Picdb::Config::Keys()
{
  return mKeys;
}
