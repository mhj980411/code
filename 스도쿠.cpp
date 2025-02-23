#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define X first
#define Y second

vector<pair<int, int>> empty_loc_v;
vector<vector<int>> copy_board;

bool check_board(pair<int, int> empty_loc) {
    bool visited[10] = { 0, };
    for (int i = 0; i < 9; i++) {
        if (copy_board[i][empty_loc.Y] == 0) continue;

        if (visited[copy_board[i][empty_loc.Y]] == true) {
            return false;
        }
        else {
            visited[copy_board[i][empty_loc.Y]] = true;
        }
    }

    memset(visited, 0, sizeof(visited));

    for (int i = 0; i < 9; i++) {
        if (copy_board[empty_loc.X][i] == 0) continue;

        if (visited[copy_board[empty_loc.X][i]] == true) {
            return false;
        }
        else {
            visited[copy_board[empty_loc.X][i]] = true;
        }
    }

    memset(visited, 0, sizeof(visited));

    int start_x = (empty_loc.X / 3) * 3, start_y = (empty_loc.Y / 3) * 3;

    for (int i = start_x; i < start_x + 3; i++) {
        for (int j = start_y; j < start_y + 3; j++) {
            if (copy_board[i][j] == 0) continue;

            if (visited[copy_board[i][j]] == true) {
                return false;
            }
            else {
                visited[copy_board[i][j]] = true;
            }
        }
    }

    return true;
}

void dfs(int level) {

    if (level == empty_loc_v.size()) {

        int de = 1;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cout << copy_board[i][j] << " ";
            }
            cout << '\n';
        }
        cout << '\n';

        return;
    }

    for (int i = 1; i <= 9; i++) {
        pair<int, int> empty_loc = empty_loc_v[level];
        copy_board[empty_loc.X][empty_loc.Y] = i;
        if (check_board(empty_loc) == true) {
            dfs(level + 1);
        }
        copy_board[empty_loc.X][empty_loc.Y] = 0;
    }

}

vector<vector<int>> solution(vector<vector<int>> input_board) {
    vector<vector<int>> answer;

    for (int i = 0; i < input_board.size(); i++) {
        for (int j = 0; j < input_board[0].size(); j++) {
            if (input_board[i][j] == 0)
                empty_loc_v.push_back({ i,j });
        }
    }
    copy_board = input_board;

    dfs(0);



    return answer;
}

int main(void) {

    vector<vector<int>> input_board = { {
                    {5, 3, 0, 0, 7, 0, 0, 0, 0},
                    {6, 0, 0, 1, 9, 5, 0, 0, 0},
                    {0, 9, 8, 0, 0, 0, 0, 6, 0},
                    {8, 0, 0, 0, 6, 0, 0, 0, 3},
                    {4, 0, 0, 8, 0, 3, 0, 0, 1},
                    {7, 0, 0, 0, 2, 0, 0, 0, 6},
                    {0, 6, 0, 0, 0, 0, 2, 8, 0},
                    {0, 0, 0, 4, 1, 9, 0, 0, 5},
                    {0, 0, 0, 0, 8, 0, 0, 7, 9}
                } };

    solution(input_board);

    return 1;
}