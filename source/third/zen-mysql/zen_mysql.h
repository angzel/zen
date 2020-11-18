/*
 Copyright (c) 2013- ClearSky G.

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "zen_db_connection.h"
#include "zen_db_query.h"
#include "zen_db_statement.h"

/**
 ALTER TABLE `test`.`logs`
 ADD COLUMN `name` text NOT NULL AFTER `ID`,
 DROP PRIMARY KEY,
 ADD PRIMARY KEY (`ID`, `name`) USING BTREE;

 ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY ':password';

 */

/**
make sure dynamic libraries have been included.
 LD_RUNPATH_SEARCH_PATHS = <....>/mysql8/lib
 */
