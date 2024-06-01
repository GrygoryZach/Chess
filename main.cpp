#include <iostream>
#include <windows.h>
#include "functions.h"
#include "figures.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


class Desk {
    /*
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
    */
    Piece *desk[8][8] = {
            {new Piece(),  new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
            {new Piece(),  new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
            {new Piece(),  new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
            {new Piece(),  new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
            {new Queen(1), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
            {new Piece(),  new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
            {new Pawn(0),  new Pawn(0), new Pawn(0), new Pawn(0), new Pawn(0), new Pawn(0), new Pawn(0), new Pawn(0)},
            {new Piece(),  new Piece(), new Piece(), new King(0), new Piece(), new Piece(), new Piece(), new Piece()}
    };
    // chess desk is flipped vertically (up to down - 1 to 8, left to right - a to h)

public:
    void print_desk() {
        //system("cls");
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
        printf("C2: %i '%c' (%i); C3: %i '%c' (%i); C4: %i '%c' (%i)\n", desk[6][2]->bw, desk[6][2]->symbol,
               desk[6][2]->symbol, desk[5][2]->bw, desk[5][2]->symbol, desk[5][2]->symbol, desk[4][2]->bw,
               desk[4][2]->symbol, desk[4][2]->symbol);
    }

    void makeMovement() {
        int from_x, from_y, to_x, to_y;
        int result;
        string prim, sec;
        string answer[4] = {"OK\n", "Invalid input!\n", "Invalid movement!\n", "Leads to check!\n"};
        while (true) {
            result = 0;
            cin >> prim >> sec;
            if (prim == sec || not(checkInput(prim) && checkInput(sec)))
                result = 1;
            else {
                chess_to_digit(prim, from_x, from_y);
                chess_to_digit(sec, to_x, to_y);
                printf_s("makeMovement: %i %i %i %i\n", from_x, from_y, to_x, to_y);
                if (desk[from_y][from_x]->bw == -1 || not checkMovement(from_x, from_y, to_x, to_y))
                    result = 2;
                else if (leadsToCheck(from_x, from_y, to_x, to_y))
                    result = 3;
            }
            if (result == 0)
                break;
            print_desk();
            cout << answer[result];
        }
        movePiece(from_x, from_y, to_x, to_y);
    }

    void movePiece(int from_x, int from_y, int to_x, int to_y) {
        desk[to_y][to_x] = desk[from_y][from_x];
        desk[from_y][from_x] = new Piece();
    }

    bool checkInput(string str) {
        return str.length() == 2 && 'A' <= str[0] && str[0] <= 'H' && '1' <= str[1] && str[1] <= '8';

    }

    bool checkMovement(int x0, int y0, int x1, int y1) {
        if ((new Queen(0))->can_move(x0, y0, x1, y1)) {
            int dif = max(abs(x1 - x0), abs(y1 - y0));
            int sign_x = (x1 - x0 > 0) - (x1 - x0 < 0), sign_y = (y1 - y0 > 0) - (y1 - y0 < 0);
            for (int i = 1; i < dif; i++)
                if (desk[y0 + i * sign_y][x0 + i * sign_x]->bw != -1) {
                    printf("Barrier: %i '%c'\n", desk[y0 + i * sign_y][x0 + i * sign_x]->bw,
                           desk[y0 + i * sign_y][x0 + i * sign_x]->symbol);
                    return false;
                }
        }
        if (desk[y1][x1]->bw != -1) {
            printf("can capture %i\t%i %i %i %i\n", desk[y0][x0]->can_capture(x0, y0, x1, y1), x0, y0, x1, y1);
            return desk[y0][x0]->bw != desk[y1][x1]->bw && desk[y0][x0]->can_capture(x0, y0, x1, y1);
        }
        cout << "can move " << desk[y0][x0]->can_move(x0, y0, x1, y1) << endl;

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
                if (desk[i][j]->bw == (1 - bw) && checkMovement(j, i, x_king, y_king)) {
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
                if (desk[i][j]->bw == (1 - bw) && checkMovement(j, i, x_king, y_king)) {
                    printf_s("%i %i  %i", desk[i][j]->bw, bw, checkMovement(j, i, x_king, y_king));
                    return true;
                }
        return false;
    }

    bool leadsToCheck(int x0, int y0, int x1, int y1) {
        // checking if the movement will lead to the check
        int x_king, y_king;
        findKing(x_king, y_king, desk[y0][x0]->bw);
        Piece *checking_piece = new Piece();
        checking_piece = desk[y1][x1];
        printf("leadsToCheck:\nmoving piece from %i %i to %i %i '%c' to '%c'\n", x0, y0, x1, y1, desk[y0][x0]->symbol, desk[y1][x1]->symbol);
        movePiece(x0, y0, x1, y1);
        printf("Is check %i ", isCheck(desk[y_king][x_king]->bw));
        bool will_be_check = isCheck(desk[y_king][x_king]->bw);
        movePiece(x1, y1, x0, y0);
        desk[y1][x1] = checking_piece;
        delete checking_piece;
        return will_be_check;
    }

    bool isCheckmate(int king_bw) {
        if (isCheck(king_bw)) {
            int x_king, y_king;
            findKing(x_king, y_king, king_bw);
            // 1) trying to move the king from the check position
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    if ((!i && !j) && checkMovement(x_king, y_king, x_king + j, y_king + i) &&
                        not leadsToCheck(x_king, y_king, x_king + j, y_king + i))
                        return false;

            // 2) trying to capture the threatening piece
            int x_threatening, y_threatening;
            findThreatening(x_threatening, y_threatening, !king_bw);
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    // looking for a chess piece that can capture an opponent's one that is threatening the king
                    if (desk[i][j]->bw == king_bw && checkMovement(j, i, x_threatening, y_threatening) &&
                        not leadsToCheck(j, i, x_threatening, y_threatening))
                        // if there is a move where after capturing the piece the king
                        // is no longer in check, the situation is not a checkmate.
                        return false;
            // 3) trying to interpose the piece between the king and the threatening piece
            // To do that it's following the threatening piece step by step on its way to the king and trying
            // to interpose the piece on any of these places
            if ((new Queen(0))->can_move(x_king, y_king, x_threatening, y_threatening)) {
                int dif = max(abs(x_king - x_threatening), abs(y_threatening - y_king));
                int sign_x = (x_king - x_threatening > 0) - (x_king - x_threatening < 0);
                int sign_y = (y_king - y_threatening > 0) - (y_king - y_threatening < 0);
                for (int i = 1; i < dif; i++)
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++) {
                            Piece now = *desk[y_threatening + i * sign_y][x_threatening + i * sign_x];
                            if (now.bw == king_bw &&
                                now.can_move(x, y, x_threatening + i * sign_x, y_threatening + i * sign_y) &&
                                not leadsToCheck(x, y, x_threatening + i * sign_x, y_threatening + i * sign_y))
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
