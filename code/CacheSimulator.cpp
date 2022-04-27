#include "CacheSimulator.h"
using namespace std;

Cache_simulator::Cache_simulator(cache_policy type, string infile_name)
{
	cache_type = type;
	input_file_name = infile_name;
	start_time = chrono::high_resolution_clock::now();
	total_referenced = 0;
}

long long Cache_simulator::simulate(int cache_size, int cache_line_size, int associativity, string policy)
{
	long long cache_hit = -1;
	if (cache_type == DIRECT_MAPPED)
		cache_hit = direct_mapped(cache_size, cache_line_size);
	else if (cache_type == SET_ASSOCIATIVE)
		cache_hit = set_associative(cache_size, cache_line_size, associativity);
	else if (cache_type == FULLY_ASSOCIATIVE)
		cache_hit = fully_associative(cache_size, cache_line_size, policy);
	else
	{
		cout << "Error" << endl;
		exit(1);
	}
	return cache_hit;
}

long long Cache_simulator::direct_mapped(int cache_size, int cache_line_size)
{
	int cache_entries = cache_size / cache_line_size;
	vector<vector<long long>> cache(cache_entries, vector<long long>(cache_line_size, -1));
	ifstream infile(input_file_name);
	string line;
	char instruction_type;
	long long address, offset, cache_line_index, base, cache_index;
	int cache_hit = 0;
	total_referenced = 0;
	while (getline(infile, line))
	{
		total_referenced++;
		stringstream ss;
		ss << line;
		ss >> instruction_type >> dec >> offset >> hex >> address;

		address = address + offset;
		base = address / cache_line_size;
		cache_index = base % cache_entries;
		cache_line_index = address % cache_line_size;

		if (cache[cache_index][cache_line_index] != address)
		{
			for (int i = 0; i < cache_line_size; i++)
				cache[cache_index][i] = base * cache_line_size + i;
		}
		else
			cache_hit++;
	}
	infile.close();
	cout << "Miss Rate: " << (float)(total_referenced - cache_hit) / total_referenced << endl;
	cout << "Hit Rate: " << (float)cache_hit / total_referenced << endl;

	return cache_hit;
}

long long Cache_simulator::set_associative(int cache_size, int cache_line_size, int associativity)
{
	int cache_entries = cache_size / cache_line_size / associativity;
	struct info tmp;
	tmp.last_used_time = -1;
	tmp.cache_line.resize(cache_line_size, -1);

	vector<vector<struct info>> cache(associativity, vector<struct info>(cache_entries, tmp));

	ifstream infile(input_file_name);
	string line;
	char instruction_type;
	long long address, offset, cache_line_index, base, cache_index;
	long long cache_hit = 0;
	total_referenced = 0;

	while (getline(infile, line))
	{
		bool hit = false;
		total_referenced++;

		stringstream ss;
		ss << line;
		ss >> instruction_type >> dec >> offset >> hex >> address;

		address = address + offset;
		base = address / cache_line_size;
		cache_index = base % cache_entries;
		cache_line_index = address % cache_line_size;

		for (unsigned int i = 0; i < cache.size(); i++)
		{
			if (cache[i][cache_index].cache_line[cache_line_index] == address)
			{
				cache_hit++;
				cache[i][cache_index].last_used_time = chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start_time).count();
				hit = true;
				break;
			}
		}
		if (!hit)
		{
			double min = 99999999999999;
			int victim_index = -1;
			for (unsigned int i = 0; i < cache.size(); i++)
			{
				if (cache[i][cache_index].last_used_time < min)
				{
					min = cache[i][cache_index].last_used_time;
					victim_index = i;
				}
			}

			cache[victim_index][cache_index].last_used_time = chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start_time).count();
			for (unsigned int i = 0; i < cache[victim_index][cache_index].cache_line.size(); i++)
			{
				cache[victim_index][cache_index].cache_line[i] = base * cache_line_size + i;
			}
		}
	}

	infile.close();

	cout << "Miss Rate: " << (float)(total_referenced - cache_hit) / total_referenced << endl;
	cout << "Hit Rate: " << (float)cache_hit / total_referenced << endl;

	return cache_hit;
}

long long Cache_simulator::fully_associative(int cache_size, int cache_line_size, string policy)
{
	long long return_value = -1;
	if (policy.compare("LRU") == 0)
		return_value = set_associative(cache_size, cache_line_size, cache_size / cache_line_size);
	else if (policy.compare("HOT-COLD") == 0)
	{
		//add other policies 
	}
	else
	{
		cout << "Error" << endl;
		exit(1);
	}
	
	return return_value;
}

long long Cache_simulator::get_total_referenced()
{
	return total_referenced;
}
