#pragma once
#include <iostream>
#include <cmath>
#include "functions.h"

using namespace std;

struct Piece {
	int bw;  // space - -1, black - 0, white - 1
	char symbol;  // whites - uppercase, black - lowercase

    Piece() {bw = -1; symbol = ' ';}
	explicit Piece(int bw_) {bw = bw_;}

	virtual bool can_move(int x0, int y0, int x1, int y1) { return 0; }
	virtual bool can_capture(int x0, int y0, int x1, int y1) { return 0; }
};

struct Pawn : public Piece {
	explicit Pawn(int bw) : Piece(bw) {Piece::symbol = 'P' + 32 * (bw == 1);}

	bool can_move(int x0, int y0, int x1, int y1) override{
		return (x0 == x1) && ((y1 - y0) * pow(-1, !bw) == 1 || (y0 == 1 + (5 * !bw) && (y1 - y0) * pow(-1, !bw) == 2));
	}
	bool can_capture(int x0, int y0, int x1, int y1) override{
		return y1 == y0 + 1 * pow(-1, !bw) && (x1 == x0 + 1 || x1 == x0 - 1);
	}
};

struct Rook : public Piece {
	explicit Rook(int bw_) : Piece(bw_) {Piece::symbol = 'R' + 32 * (bw == 1);}

	bool can_move(int x0, int y0, int x1, int y1) override{
		return x0 == x1 || y0 == y1;
	}
	bool can_capture(int x0, int y0, int x1, int y1) override{
		return can_move(x0, y0, x1, y1);
	}
};

struct Bishop : public Piece {
	explicit Bishop(int bw_) : Piece(bw_) {Piece::symbol = 'B' + 32 * (bw == 1);}

	bool can_move(int x0, int y0, int x1, int y1) override{
		return abs(x1 - x0) == abs(y1 - y0);
	}
	bool can_capture(int x0, int y0, int x1, int y1) override{
		return can_move(x0, y0, x1, y1);
	}
};

struct Knight : public Piece {
	explicit Knight(int bw_) : Piece(bw_) {Piece::symbol = 'N' + 32 * (bw == 1);}

	bool can_move(int x0, int y0, int x1, int y1) override{
		return (abs(x1 - x0) == 2 && abs(y1 - y0) == 1) || (abs(x1 - x0) == 1 && abs(y1 - y0) == 2);
	}
	bool can_capture(int x0, int y0, int x1, int y1) override{
		return can_move(x0, y0, x1, y1);
	}
};

struct Queen : public Piece {
	explicit Queen(int bw_) : Piece(bw_) {Piece::symbol = 'Q' + 32 * (bw == 1);}

	bool can_move(int x0, int y0, int x1, int y1) override{
		return abs(x1 - x0) == abs(y1 - y0) || (x0 == x1 || y0 == y1);
	}
	bool can_capture(int x0, int y0, int x1, int y1) override{
		return can_move(x0, y0, x1, y1);
	}
};

struct King : public Piece {
	explicit King (int bw_) : Piece(bw_) {Piece::symbol = 'K' + 32 * (bw == 1);}

	bool can_move(int x0, int y0, int x1, int y1) override{
		return abs(x1 - x0) == 1 || abs(y1 - y0) == 1;
	}
	bool can_capture(int x0, int y0, int x1, int y1) override{
		return can_move(x0, y0, x1, y1);
	}
};
