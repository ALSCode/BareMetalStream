/*
 * register_test.cpp
 *
 *  Created on: 25 авг. 2020 г.
 *      Author: sazhi
 */

#include <array>

#include "bm_stream.hpp"


extern "C"
void test (void){

	StreamSWO cout;

	float raw[] = {3.1416, 2.7183, 1.618};
	std::array<int, 3> arr{123, 456, 789};

	cout << base_t{2021}    << '\n'
	     << float{9.806}    << '\n'
	     << arr             << '\n'
	     << raw             << '\n'
	     << "Hello, Habr!"  << '\n'
	     << ("esreveR me!" | std::views::take(7) | std::views::reverse ) << '\n'
		 << "--------------------\n";
}
