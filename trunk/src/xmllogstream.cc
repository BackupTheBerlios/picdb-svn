/******************************************************************
*                xmllogstream.cc 
*   Wed Dec  1 14:44:11 2004
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


#include "../include/xmllogstream.hpp"
std::map<const int, char*> Picdb::XhtmlLogStream::mColorNames;


Picdb::XmlLogStream::XmlLogStream (char *filename)
{
  stream=fopen (filename,"w");
  StartDocument();
}

Picdb::XmlLogStream::XmlLogStream (FILE *fp)
{
  stream=fp;
  StartDocument();
}

void Picdb::XmlLogStream::StartDocument ()
{
  if (!stream)
    return;
  fprintf (stream, "<?xml version='1.0' encoding='iso-8859-1'?>\n<log>\n");
}

Picdb::XmlLogStream::~XmlLogStream ()
{
  if (stream)
    {
      fprintf (stream, "</log>\n");
      fclose(stream);
    }
}

char *Picdb::XmlLogStream::GetDate()
{
  time_t T;
  tm TM;
  char *result;
  time (&T);
  localtime_r (&T, &TM);
  asprintf (&result, "%02i-%02i-%02i", TM.tm_year+1900, TM.tm_mon+1, TM.tm_mday);
  return result;
}

char *Picdb::XmlLogStream::GetTime()
{
  time_t T;
  tm TM;
  char *result;
  time (&T);
  localtime_r (&T, &TM);
  asprintf (&result, "%02i:%02i:%02i", TM.tm_hour, TM.tm_min, TM.tm_sec);
  return result;  
}

void Picdb::XmlLogStream::Write (Picdb::LogLevel &lvl, char *subsystem, char *txt)
{
  if(!stream)
    return;
  char *date, *t;
  date=GetDate();
  t=GetTime();
  fprintf (stream, "<entry date='%s' time='%s' subsystem='%s' level='%s'>%s</entry>\n", date, t, subsystem, lvl.GetName(), txt);
  free(date);
  free(t);
}

void Picdb::XmlLogStream::Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt)
{
  if(!stream)
    return;
  char *date, *t;
  date=GetDate();
  t=GetTime();
  fprintf (stream, "<entry file='%s' line='%i' date='%s' time='%s' subsystem='%s' level='%s'>%s</entry>\n", file, line, date, t, subsystem, lvl.GetName(), txt);
  free(date);
  free(t);
}


Picdb::XhtmlLogStream::XhtmlLogStream(FILE *fp):XmlLogStream()
{
  stream=fp;
  InitColorNames();
  StartDocument();
}
Picdb::XhtmlLogStream::XhtmlLogStream(char *filename):XmlLogStream()
{
  stream=fopen (filename,"w");
  InitColorNames();
  StartDocument();
}

Picdb::XhtmlLogStream::~XhtmlLogStream()
{
  if (stream)
    {
      fprintf (stream, "</table></body></html>\n");
      fclose(stream);
    }
}

void Picdb::XhtmlLogStream::Write (Picdb::LogLevel &lvl, char *subsystem, char *txt)
{
  Write (lvl, "&amp;", 0, subsystem, txt);
}

void Picdb::XhtmlLogStream::Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt)
{
  if(!stream)
    return;
  char *date, *t;
  date=GetDate();
  t=GetTime();
  //fprintf (stream, "<entry file='%s' line='%i' date='%s' time='%s' subsystem='%s' level='%s'>%s</entry>\n", file, line, date, t, subsystem, lvl.GetName(), txt);
  fprintf (stream, "<tr><td>%s, %s</td><td style='color: %s; font-weight: bold;'>%s</td><td>%s</td><td>%s</td><td>%i</td><td>%s</td></tr>\n", date, t, mColorNames[lvl.GetColor()],lvl.GetName(), subsystem, file, line, txt);
  free(date);
  free(t);
}

void Picdb::XhtmlLogStream::StartDocument()
{
  if (!stream)
    return;
  fprintf (stream, "<?xml version='1.0' encoding='iso-8859-1'?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html>\n");
  fprintf (stream, "<head><title>Log</title>\n");
  fprintf (stream, "<style type='text/css'>");
  fprintf (stream, "body {background: #0f0f0f; color: #9f9f3f;}\ntable {width: 100%; border-width: 0px}\ntd,th{border-style: ridge; border-width: 1px; border-color: #9f9f3f;}\n");
  fprintf (stream, "</style></head>\n<body>\n");
  fprintf (stream, "<table>\n");
  fprintf (stream, "<tr><th>Time</th><th>Level</th><th>Subsystem</th><th>File</th><th>Line</th><th>Message</th></tr>\n");
}

void Picdb::XhtmlLogStream::InitColorNames()
{
  if (!mColorNames.empty())
    return;
  mColorNames[0]="#000000";
  mColorNames[1]="#7f0000";
  mColorNames[2]="#007f00";
  mColorNames[3]="#7f7f00";
  mColorNames[4]="#00007f";
  mColorNames[5]="#7f007f";
  mColorNames[6]="#007f7f";
  mColorNames[7]="#7f7f7f";
  mColorNames[10]="#000000";
  mColorNames[11]="#ff0000";
  mColorNames[12]="#00ff00";
  mColorNames[13]="#ffff00";
  mColorNames[14]="#0000ff";
  mColorNames[15]="#ff00ff";
  mColorNames[16]="#00ffff";
  mColorNames[17]="#ffffff";
}
