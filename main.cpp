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
            int dif = max(abs(x1 - x0), abs(y1 - y0));
            int sign_x = (x1 - x0 > 0) - (x1 - x0 < 0), sign_y = (y1 - y0 > 0) - (y1 - y0 < 0);
            for (int i = 1; i < dif; i++)
                if (desk[y0 + i * sign_y][x0 + i * sign_x]->bw != -1)
                    return false;

            return true;
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

    void findThreatening(int &x, int &y, int bw) {
        // finds the piece, that attacks the king
        // x, y, bw - king's coordinates and colour
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

    bool isCheck(int bw) {
        // checking if there is a piece of opposite to the king's color on the table, that can move to the king's position
        int x_king, y_king;
        findKing(x_king, y_king, bw);
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (desk[i][j]->bw == !bw && check_movement(j, i, x_king, y_king))
                    return true;
        return false;
    }

    bool leadsToCheck(int x0, int y0, int x1, int y1){
        // checking if the movement will lead to the check
        int x_king, y_king;
        findKing(x_king, y_king, desk[x0][y0]->bw);

        Piece checking_piece = *desk[y0][x0];
        movePiece(x0, y0, x1, y1);
        bool will_be_check = isCheck(desk[x0][y0]->bw);
        movePiece(x1, y1, x0, y0);
        desk[y1][x1] = &checking_piece;
        return will_be_check;
    }

    bool isCheckmate(int king_bw) {
        if (isCheck(king_bw)) {
            int x_king, y_king;
            findKing(x_king, y_king, king_bw);
            // 1) trying to move the king from the check position
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    if ((!i && !j) && check_movement(x_king, y_king, x_king + j, y_king + i) &&
                        not leadsToCheck(x_king, y_king, x_king + j, y_king + i))
                        return false;

            // 2) trying to capture the threatening piece
            int x_threatening, y_threatening;
            findThreatening(x_threatening, y_threatening, !king_bw);
            Piece threatening_piece = *desk[y_threatening][x_threatening];
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    // looking for a chess piece that can capture an opponent's one that is threatening the king
                    if (desk[i][j]->bw == king_bw && check_movement(j, i, x_threatening, y_threatening)) {
                        // if it was found, we capture the threatening chess and check again if the king is under attack now
                        movePiece(j, i, x_threatening, y_threatening);
                        bool is_check = isCheck(king_bw);
                        movePiece(x_threatening, y_threatening, j, i);
                        desk[y_threatening][x_threatening] = &threatening_piece;
                        // if there is a movement, that leads to getting out of check - the situation is not a checkmate
                        // if there is a movement of capturing of the threatening piece,
                        // in which the king has get out of check - the situation is not a checkmate
                        // if there is a move where, after capturing the piece, the king is no longer in check, the situation is not checkmate.
                        if (not is_check)
                            return false;
                    }
            // 3) trying to interpose the piece between the king and the threatening piece
            // to do that, we follow the threatening piece step by step on its way to the king and trying to interpose
            // the piece on any of these places
            if ((new Queen(0))->can_move(x_king, y_king, x_threatening, y_threatening)) {
                int dif = max(abs(x_king - x_threatening), abs(y_threatening - y_king));
                int sign_x = (x_king - x_threatening > 0) - (x_king - x_threatening < 0);
                int sign_y = (y_king - y_threatening > 0) - (y_king - y_threatening < 0);
                for (int i = 1; i < dif; i++)
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++){
                            Piece now = *desk[y_threatening + i * sign_y][x_threatening + i * sign_x];
                            if (now.bw == king_bw && now.can_move(x, y, x_threatening + i * sign_x, y_threatening + i * sign_y)) {
                                movePiece(x, y, x_threatening + i * sign_x, y_threatening + i * sign_y);
                                bool is_check = isCheck(king_bw);
                                movePiece(x_threatening + i * sign_x, y_threatening + i * sign_y, x, y);
                                // if there is a  movement, that leads to getting out of check - the situation is not a checkmate
                                if (not is_check)
                                    return false;
                            }
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
