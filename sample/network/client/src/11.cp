#include "header.h"
#include <map>
#include <thread>
#include "zen_poll.h"
#include "zen_cast.h"
#include "zen_atom.h"

void test_atom()
{
	typedef Zen::AtomMap<int, int> AM;
	AM am;
	
	for(int i = 0; i < 100; ++i) am.insert(i, i);
	
	AM::Map __mm;

	am.execute([&__mm](AM::Map & map) {
		for(auto i = map.begin(); i != map.end(); )
		{
			if(i->first % 3 == 0)
			{
				i = map.erase(i);
			}
			else ++i;
		}
		__mm = map;
	});
	for(auto i = __mm.begin(); i != __mm.end(); ++i)
	{
		cout << i->first << "->" << i->second << endl;
	}
	
	typedef Zen::AtomSet<int> AS;
	AS as;
	AS::Set __set;
	for(int i = 0; i < 100; ++i) as.insert(i);
	as.execute([&__set](AS::Set & set) {
		__set = set;
	});
	for(auto i = __set.begin(); i != __set.end(); ++i)
		cout << *i << endl;
}
