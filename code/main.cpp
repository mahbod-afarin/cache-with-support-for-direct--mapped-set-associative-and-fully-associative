#include "CacheSimulator.h"
using namespace std; 

int main(int argc, char **argv)
{
	int cache_line_size = -1, cache_size = -1, associativity = -1;
	string policy = "";
	Cache_simulator direct_mapped(DIRECT_MAPPED, argv[1]);
	string op = argv[2];
	if (op == "DM")
	{
		cache_line_size = atoi(argv[4]);
		int cache_size_array = atoi(argv[3]);
		direct_mapped.simulate(cache_size_array, cache_line_size, associativity, policy);
		int m = cache_size_array / cache_line_size;
		cout << "# of sets: " << m << endl;
		cout << "# of ways: 1" << endl;
		cout << "# of tag bits: " << 32 - log2(m) - log2(cache_line_size) << endl;
		cout << "# of index bits: " << log2(m) << endl;
		cout << "# of offset bits: " << log2(cache_line_size) << endl;	
	}
	else if (op == "SA")
	{
		Cache_simulator set_associative(SET_ASSOCIATIVE, argv[1]);
		int associativity_array = atoi(argv[5]);
		cache_line_size = atoi(argv[4]);
		cache_size = atoi(argv[3]);
		set_associative.simulate(cache_size, cache_line_size, associativity_array, policy);
		int m = cache_size / cache_line_size;
		cout << "# of sets: " << m / associativity_array << endl;
		cout << "# of ways: " << associativity_array << endl;
		cout << "# of tag bits: " << 32 - log2(m / associativity_array) - log2(cache_line_size) << endl;
		cout << "# of index bits: " << log2(m / associativity_array) << endl;
		cout << "# of offset bits: " << log2(cache_line_size) << endl;
	}
	else if (op == "FA")
	{
		int associativity = atoi(argv[5]);
		cache_line_size = atoi(argv[4]);
		cache_size = atoi(argv[3]);
		Cache_simulator fully_associative(FULLY_ASSOCIATIVE, argv[1]);
		policy = "LRU";
		fully_associative.simulate(cache_size, cache_line_size, associativity, policy);
		int m = cache_size / cache_line_size;
	    cout << "# of sets: 1" << endl;
		cout << "# of ways: " << associativity << endl;
		cout << "# of tag bits: " << 32 - log2(cache_line_size) << endl;
		cout << "# of index bits: 0" << endl;
		cout << "# of offset bits: " << log2(cache_line_size) << endl;
	}
	return 0;
}
