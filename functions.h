#pragma once
#include <iostream>

void chess_to_digit(const std::string chess_notation, int& x, int& y) {
	x = static_cast<int>(chess_notation[0]) - 65;
	y = 7 - (static_cast<int>(chess_notation[1]) - 49);
}
