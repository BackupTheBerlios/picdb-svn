xCat() #xCat filename str_to_prepend str_to_append
{
    echo -e -n "import sys\nfor i in file('$1'):\n  sys.stdout.write ('%s%s%s' % ('$2',i,'$3'))\n" | python
}

copyright() #copyright filename author mail license
{
    cat <<EOF
/******************************************************************
*                $1 
*   `date +%c`
*   Copyright `date +%Y` $2
*   $3
******************************************************************/
EOF
    printf "\n/******************************************************************\n"
    xCat ${4}_hdr.txt '* '
    printf "******************************************************************/\n\n\n"
}
lower()
{
    printf "print '$1'.lower()\n" | python
}

upper()
{
    printf "print '$1'.upper()\n" | python
}

for i in $*
do
  f=`lower $i`
  I=`upper $i`
  copyright $f.cc 'Jens Krafczyk' 'segv@gmx.de' gpl > src/$f.cc
  copyright $f.hpp 'Jens Krafczyk' 'segv@gmx.de' gpl > include/$f.hpp
  cat <<EOF >> src/$f.cc
#include "../include/$f.hpp"
Picdb::$i::$i ()
{
}
Picdb::$i::~$i ()
{
}
EOF
  cat <<EOF >>  include/$f.hpp
#ifndef PICDB_${I}_HPP
#define PICDB_${I}_HPP
namespace Picdb
{
  class $i;
}
class Picdb::$i
{
public:
  $i ();
  virtual ~$i (); 
};
#endif
EOF
done
