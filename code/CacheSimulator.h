#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <math.h>
#include <iomanip>
using namespace std;

enum cache_policy{DIRECT_MAPPED, SET_ASSOCIATIVE, FULLY_ASSOCIATIVE};

struct info{
	double last_used_time;
	vector<long long> cache_line;
};

class Cache_simulator{
	public:

		Cache_simulator(cache_policy type, string infile_name);
		long long simulate(int cache_size, int cache_line_size, int ways, string policy);
		long long get_total_referenced();
	
	private:

		long long direct_mapped(int cache_size, int cache_line_size);
		long long set_associative(int cache_size, int cache_line_size, int associativity);
		long long fully_associative(int cache_size, int cache_line_size, string replacement_policy);

		long long total_referenced;		
        string input_file_name;
		cache_policy cache_type;
	    chrono::high_resolution_clock::time_point start_time;
};