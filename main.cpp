#include <iostream>
#include <windows.h>
#include "functions.h"
#include "figures.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


class Desk {
    /*
	char desk[8][9] = {
		"RNBQKBNR",
		"PPPPPPPP",
		"        ",
		"        ",
		"        ",
		"        ",
		"pppppppp",
		"rnbqkbnr"
	};*/
    Piece *desk[8][8] = {
            {new Rook(1), new Knight(1), new Bishop(1), new Queen(1), new King(1), new Bishop(1), new Knight(1), new Rook(1)},
            {new Pawn(1), new Pawn(1),   new Pawn(1),   new Pawn(1),  new Pawn(1), new Pawn(1),   new Pawn(1),   new Pawn(1)},
            {new Piece(), new Piece(),   new Piece(),   new Piece(),  new Piece(), new Piece(),   new Piece(),   new Piece()},
            {new Piece(), new Piece(),   new Piece(),   new Piece(),  new Piece(), new Piece(),   new Piece(),   new Piece()},
            {new Piece(), new Piece(),   new Piece(),   new Piece(),  new Piece(), new Piece(),   new Piece(),   new Piece()},
            {new Piece(), new Piece(),   new Piece(),   new Piece(),  new Piece(), new Piece(),   new Piece(),   new Piece()},
            {new Pawn(0), new Pawn(0),   new Pawn(0),   new Pawn(0),  new Pawn(0), new Pawn(0),   new Pawn(0),   new Pawn(0)},
            {new Rook(0), new Knight(0), new Bishop(0), new Queen(0), new King(0), new Bishop(0), new Knight(0), new Rook(0)}
    };
    // chess desk is flipped vertically (up to down - 1 to 8, left to right - a to h)

public:
    void print_desk() {
        system("cls");
        printf_s("     ");
        for (int i = 0; i < 8; i++)
            printf_s("   %c  ", char(65 + i));
        cout << endl << endl;

        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 3; k++) {
                SetConsoleTextAttribute(hConsole, 15);
                if (k == 1) {
                    printf_s("  %i  ", 8 - i);
                } else printf_s("     ");
                for (int j = 0; j < 8; j++) {
                    if ((i + j) % 2 == 1)
                        SetConsoleTextAttribute(hConsole, 15);
                    else
                        SetConsoleTextAttribute(hConsole, 240);
                    if (k == 1) {
                        if ((i + j) % 2 == 1) SetConsoleTextAttribute(hConsole, 15);
                        else SetConsoleTextAttribute(hConsole, 240);
                        printf_s("   %c  ", desk[i][j]->symbol);
                    } else
                        printf_s("      ");
                }
                if (k == 1) {
                    SetConsoleTextAttribute(hConsole, 15);
                    printf_s("  %i", 8 - i);
                }
                SetConsoleTextAttribute(hConsole, 15);
                cout << endl;
            }
        }
        SetConsoleTextAttribute(hConsole, 15);
        cout << endl;
        printf_s("     ");

        for (int i = 0; i < 8; i++)
            printf_s("   %c  ", char(65 + i));
        cout << endl << endl;
    }

    void makeMovement() {
        int from_x, from_y, to_x, to_y;
        string prim, sec;
        cin >> prim >> sec;
        while (true) {
            print_desk();
            if (prim == sec || not(check_input(prim) && check_input(sec)))
                cout << "Invalid input!\n";
            else {
                chess_to_digit(prim, from_x, from_y);
                chess_to_digit(sec, to_x, to_y);
                if (not(check_movement(from_x, from_y, to_x, to_y)))
                    cout << "Invalid movement!\n";
                else
                    break;
            }
            cin >> prim >> sec;
        }

        movePiece(from_x, from_y, to_x, to_y);
    }

    void movePiece(int from_x, int from_y, int to_x, int to_y){
        desk[to_y][to_x] = desk[from_y][from_x];
        desk[from_y][from_x] = new Piece();
    }

    bool check_input(string str) {
        return str.length() == 2 && 'A' <= str[0] && str[0] <= 'H' && '1' <= str[1] && str[1] <= '8';
    }

    bool check_movement(int x0, int y0, int x1, int y1) {
        if ((new Queen(0))->can_move(x0, y0, x1, y1)) {
            int d = max(abs(x1 - x0), abs(y1 - y0));
            int sign_x = (x1 - x0 > 0) - (x1 - x0 < 0), sign_y = (y1 - y0 > 0) - (y1 - y0 < 0);
            for (int i = 1; i < d; i++) {
                if (desk[y0 + i * sign_y][x0 + i * sign_x]->bw != -1)
                    return false;
            }
        }

        if (desk[y1][x1]->bw != -1)
            return desk[y0][x0]->bw != desk[y1][x1]->bw && desk[y0][x0]->can_capture(x0, y0, x1, y1);

        return desk[y0][x0]->can_move(x0, y0, x1, y1);
    }

    void findKing(int &x, int &y, int bw) {
        // finds coordinates of king of given colour
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (desk[i][j]->symbol == 'K' + 32 * (bw == 1)) {
                    y = i;
                    x = j;
                    break;
                }
    }

    void findAttacking(int &x, int &y, int bw) {
        // finds the piece, that attacks the king
        // x, y - king's coordinates, bw - king's colour
        int x_king, y_king;
        findKing(x_king, y_king, bw);
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (desk[i][j]->bw == !bw && check_movement(j, i, x_king, y_king)){
                    y = i;
                    x = j;
                    break;
                }
    }

    bool isCheck(int bw, int x_king, int y_king) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (desk[i][j]->bw == !bw && check_movement(j, i, x_king, y_king))
                    return true;
        return false;
    }

    bool isCheckmate(int king_bw) {
        int x_king, y_king;
        findKing(x_king, y_king, king_bw);
        if (isCheck(king_bw, x_king, y_king)) {
            // 1) try to move the king from the check position
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    if (not(i == 0 && j == 0) && check_movement(x_king, y_king, x_king + j, y_king + i) && not isCheck(king_bw, x_king + j, y_king + i))
                        return false;
                }
            // 2) try to capture the attacking piece
            int x_attacking, y_attacking;
            findAttacking(x_attacking, y_attacking, !king_bw);
            // Piece attacking_piece = desk[y_attacking][x_attacking];
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    if (desk[i][j]->bw == king_bw && check_movement(j, i, x_attacking, y_attacking)) {
                        movePiece(j, i, x_attacking, y_attacking);
                        if (not isCheck(king_bw, x_king, y_king)) {
                            movePiece(j, i, x_attacking, y_attacking);
                            return false;
                        }
                    }
        }
        return true;
    }
};

int main() {
    Desk desk;
    for (int i = 0; i < 25; i++) {
        desk.print_desk();
        desk.makeMovement();
    }
    desk.print_desk();

    return 0;
}
