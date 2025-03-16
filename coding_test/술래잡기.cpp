#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

struct runner {
    int x;
    int y;
    int d;
    int cur_dir;
    bool Iscatch = false;
};

struct chaser {
    int x;
    int y;
    int cur_dir;
};

int n, m, h, k;

vector<runner> runner_v;

queue<int> runner_index_board[100][100];
bool tree_board[100][100];
int answer = 0;
chaser chaser_info;

void Input() {
    cin >> n >> m >> h >> k;

    runner r;
    for (int i = 0; i < m; i++) {
        cin >> r.x >> r.y >> r.d;
        r.x--;  r.y--;
        if (r.d == 1) r.cur_dir = 1;
        else r.cur_dir = 0;

        runner_v.push_back(r);
        runner_index_board[r.x][r.y].push(i);
    }

    pair<int, int> tree_loc;
    for (int i = 0; i < h; i++) {
        cin >> tree_loc.X >> tree_loc.Y;
        tree_loc.X--;   tree_loc.Y--;
        tree_board[tree_loc.X][tree_loc.Y] = 1;
    }

    chaser_info = { n / 2,n / 2,2 };
}

void runner_move() {
    for (int i = 0; i < runner_v.size(); i++) {
        if (runner_v[i].Iscatch == true) continue;

        runner r = runner_v[i];
        int dist = abs(r.x - chaser_info.x) + abs(r.y - chaser_info.y);
        if (dist > 3) continue;

        runner_index_board[r.x][r.y].pop();
        int nx = r.x + dx[r.cur_dir];
        int ny = r.y + dy[r.cur_dir];

        if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
            r.cur_dir = (r.cur_dir + 2) % 4;
            nx = r.x + dx[r.cur_dir];
            ny = r.y + dy[r.cur_dir];
            if (!(nx == chaser_info.x && ny == chaser_info.y)) {
                r.x = nx;   r.y = ny;
            }
        }
        else {
            if (!(nx == chaser_info.x && ny == chaser_info.y)) {
                r.x = nx;   r.y = ny;
            }
        }

        runner_v[i] = r;
        runner_index_board[r.x][r.y].push(i);
    }
}

void catch_runner(int turn) {

    int catch_runner_cnt = 0;

    int nx = chaser_info.x;
    int ny = chaser_info.y;

    for (int iter = 0; iter < 3; iter++) {
       
        if (tree_board[nx][ny] == 0 && runner_index_board[nx][ny].size() > 0) {
            int size = runner_index_board[nx][ny].size();
            for (int i = 0; i < size; i++) {
                int index = runner_index_board[nx][ny].front();     runner_index_board[nx][ny].pop();
                runner_v[index].Iscatch = true;
                catch_runner_cnt++;
            }
        }
        nx += dx[chaser_info.cur_dir];
        ny += dy[chaser_info.cur_dir];
    }

    answer += (turn * catch_runner_cnt);
}


void chaser_move() {
    int dist = 1;
    int ch_dir_cnt = 2;
    int cur_move_dist = 0;

    bool reverse_flag = false;

    for (int i = 0; i < k; i++) {
        runner_move();

        chaser_info.x += dx[chaser_info.cur_dir];
        chaser_info.y += dy[chaser_info.cur_dir];
        cur_move_dist++;

        if (reverse_flag == false) {
            if (cur_move_dist == dist) {
                chaser_info.cur_dir = ((chaser_info.cur_dir - 1) + 4) % 4;
                cur_move_dist = 0;
                ch_dir_cnt--;
            }

            if (ch_dir_cnt == 0) {
                ch_dir_cnt = 2;
                dist++;
            }

            if (chaser_info.x == 0 && chaser_info.y == 0) {
                chaser_info.cur_dir = 0;
                dist = n - 1;
                ch_dir_cnt = 3;
                cur_move_dist = 0;
                reverse_flag = true;
            }
        }
        else {
            if (cur_move_dist == dist) {
                chaser_info.cur_dir = (chaser_info.cur_dir + 1) % 4;
                cur_move_dist = 0;
                ch_dir_cnt--;
            }
            if (ch_dir_cnt == 0) {
                ch_dir_cnt = 2;
                dist--;
            }
            if (chaser_info.x == n / 2 && chaser_info.y == n / 2) {
                chaser_info.cur_dir = 2;
                dist = 1;
                ch_dir_cnt = 2;
                cur_move_dist = 0;

                reverse_flag = false;
            }
        }
        catch_runner(i + 1);
    }
}

int main(void) {
    Input();
    chaser_move();
    cout << answer;
    return 0;
}
