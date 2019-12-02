//
//  main.cpp
//  zena_dev
//
//  Created by lover on 2019/11/29.
//  Copyright © 2019 lover. All rights reserved.
//

#include <iostream>
#include <string>
#include "zen-core/zen_exception.h"
#include "zen-string/zen_csv.h"
#include "zen-string/zen_localization.h"

int main(int argc, const char * argv[]) {
	// insert code here...
	std::string src ="1997,Ford,E350,\"ac, abs, moon\",3000.00\n\
	1999,Chevy,\"Venture \"\"Extended Edition\"\"\",\"\",4900.00\n\
	1999,Chevy,\"Venture \"\"Extended Edition, Very Large\"\"\",\"\",5000.00\n\
	1996,Jeep,Grand Cherokee,\"MUST SELL!\n\
	air, moon roof, loaded\",4799.00\n";
	Zen::CSVLoader csv_loader;
	csv_loader.decode(src);

	auto & data = csv_loader.getContent();
	for(auto & v : data.rows)
	{
		for(auto & i : v)
		{
			std::cout << i << std::endl;
		}
	}
	Zen::Localization local;
	local.initWithCSVContent(data);
	std::cout << "Hello, World!\n";
	return 0;
}
