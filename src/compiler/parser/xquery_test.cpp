/*
 * Copyright 2006-2008 The FLWOR Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "compiler/parser/xquery_driver.h"
#include <iostream>
     
int main (int argc, char *argv[])
{
	xquery_driver driver(cout);
	for (++argv; argv[0]; ++argv) {
		if (*argv == std::string("-p")) {
			driver.trace_parsing = true;
		}
		else if (*argv == std::string ("-s")) {
			driver.trace_scanning = true;
		}
		else {
			driver.parse(*argv);
			driver.get_expr()->put(cout) << std::endl;
		}
	}
}
/* vim:set et sw=2 ts=2: */
