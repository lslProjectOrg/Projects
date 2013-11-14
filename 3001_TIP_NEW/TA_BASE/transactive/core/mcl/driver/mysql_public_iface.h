/*
  Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.

  The MySQL Connector/C++ is licensed under the terms of the GPLv2
  <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
  MySQL Connectors. There are special exceptions to the terms and
  conditions of the GPLv2 as it is applied to this software, see the
  FLOSS License Exception
  <http://www.mysql.com/about/legal/licensing/foss-exception.html>.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

/*
  Public interface of the MySQL Connector/C++.
  You might not use it but directly include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/

#ifndef MYSQL_PUBLIC_IFACE_H_
#define MYSQL_PUBLIC_IFACE_H_

#include "core/mcl/cppconn/driver.h"
//#include <cppconn/driver.h>
#include "core/mcl/cppconn/exception.h"
//#include <cppconn/exception.h>
#include "core/mcl/cppconn/warning.h"
//#include <cppconn/warning.h>
#include "core/mcl/cppconn/metadata.h"
//#include <cppconn/metadata.h>
#include "core/mcl/cppconn/prepared_statement.h"
//#include <cppconn/prepared_statement.h>
#include "core/mcl/cppconn/resultset.h"
//#include <cppconn/resultset.h>
#include "core/mcl/cppconn/resultset_metadata.h"
//#include <cppconn/resultset_metadata.h>
#include "core/mcl/cppconn/statement.h"
//#include <cppconn/statement.h>
/*#include <cppconn/connection.h"*/
#include "core/mcl/driver/mysql_driver.h"
//#include "mysql_driver.h"
#include "core/mcl/driver/mysql_connection.h"
//#include "mysql_connection.h"
//#include <cppconn/types.h>

#endif /* MYSQL_PUBLIC_IFACE_H_ */
