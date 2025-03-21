#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,-1,0,1,1,1,0,-1 };
int dy[] = { 0,-1 ,-1,-1,0,1,1,1 };

int m,t;
pair<int, int> pack_man_loc;

struct monster {
	int r;
	int c;
	int d;
};

queue<monster> monster_board1[4][4];
queue<monster> monster_board2[4][4];
queue<monster> monster_egg_board[4][4];
int monster_body_age_board[4][4];

void Input() {
	cin >> m >> t;

	cin >> pack_man_loc.X >> pack_man_loc.Y;
	pack_man_loc.X--;	pack_man_loc.Y--;

	monster mon;
	for (int i = 0; i < m; i++) {
		cin >> mon.r >> mon.c >> mon.d;
		mon.r--;	mon.c--;	mon.d--;
		monster_board1[mon.r][mon.c].push(mon);
	}
}

void monster_copy(queue<monster>(&monster_board)[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (monster_board[i][j].size() > 0) {
				int size = monster_board[i][j].size();
				for (int k = 0; k < size; k++) {
					monster mon = monster_board[i][j].front();	monster_board[i][j].pop();
					monster_egg_board[i][j].push(mon);
					monster_board[i][j].push(mon);
				}
			}
		}
	}
}

void monster_move(queue<monster>(&monster_board)[4][4], queue<monster>(&move_monster_board)[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (monster_board[i][j].size() > 0) {
				int size = monster_board[i][j].size();
				for (int k = 0; k < size; k++) {
					monster mon = monster_board[i][j].front();	monster_board[i][j].pop();

					int dir = mon.d;
					for (int cnt = 0; cnt < 8; cnt++) {
						int nx = mon.r + dx[dir];
						int ny = mon.c + dy[dir];
						if (!(monster_body_age_board[nx][ny] > 0
							|| (nx == pack_man_loc.X && ny == pack_man_loc.Y)
							|| (nx < 0 || ny < 0 || nx >= 4 || ny >= 4)
							)) {
							mon.r = nx;
							mon.c = ny;
							mon.d = dir;
							break;
						}
						dir = (dir + 1) % 8;
					}

					move_monster_board[mon.r][mon.c].push(mon);
				}
			}
		}
	}
}

void move_packman(queue<monster>(&monster_board)[4][4]) {
	int pack_man_dx[4] = { -1,0,1,0 };
	int pack_man_dy[4] = { 0,-1,0,1 };

	int select_dir_arr[3] = { 0, };
	int max_cnt = -21e8;
	bool visited[4][4] = {0,};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				memset(visited, 0, sizeof(visited));
				int eat_cnt = 0;

				int nx = pack_man_loc.X + pack_man_dx[i];
				int ny = pack_man_loc.Y + pack_man_dy[i];
				if (nx < 0 || ny < 0 || nx >= 4 || ny >= 4) continue;
				if (monster_board[nx][ny].size() > 0 && visited[nx][ny] == 0) {
					eat_cnt += monster_board[nx][ny].size();
					visited[nx][ny] = 1;
				}

				nx = nx + pack_man_dx[j];
				ny = ny + pack_man_dy[j];
				if (nx < 0 || ny < 0 || nx >= 4 || ny >= 4) continue;
				if (monster_board[nx][ny].size() > 0 && visited[nx][ny] == 0) {
					eat_cnt += monster_board[nx][ny].size();
					visited[nx][ny] = 1;
				}

				nx = nx + pack_man_dx[k];
				ny = ny + pack_man_dy[k];
				if (nx < 0 || ny < 0 || nx >= 4 || ny >= 4) continue;
				if (monster_board[nx][ny].size() > 0 && visited[nx][ny] == 0) {
					eat_cnt += monster_board[nx][ny].size();
					visited[nx][ny] = 1;
				}

				if (eat_cnt > max_cnt) {
					max_cnt = eat_cnt;
					select_dir_arr[0] = i;	select_dir_arr[1] = j;	select_dir_arr[2] = k;
				}
			}
		}
	}
	int nx = pack_man_loc.X, ny = pack_man_loc.Y;	
	for (int i = 0; i < 3; i++) {
		nx += pack_man_dx[select_dir_arr[i]];	ny += pack_man_dy[select_dir_arr[i]];
		if (monster_board[nx][ny].size() > 0) {
			int size = monster_board[nx][ny].size();
			for (int k = 0; k < size; k++) {
				monster_board[nx][ny].pop();
			}
			monster_body_age_board[nx][ny] = 3;
		}
	}
	pack_man_loc = { nx,ny };
}

void reduce_monster_body_age() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (monster_body_age_board[i][j] > 0) {
				monster_body_age_board[i][j]--;
			}
		}
	}
}

void monster_copy_complete(queue<monster>(&monster_board)[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (monster_egg_board[i][j].size() > 0) {
				int size = monster_egg_board[i][j].size();
				for (int k = 0; k < size; k++) {
					monster mon = monster_egg_board[i][j].front();	monster_egg_board[i][j].pop();
					monster_board[i][j].push(mon);
				}
			}
		}
	}
}


void simulation() {
	for (int i = 1; i <= t; i++) {
		if (i % 2 == 1) {
			monster_copy(monster_board1);
			int de = 1;
			monster_move(monster_board1, monster_board2);
			de = 2;
			move_packman(monster_board2);
			de = 3;
			reduce_monster_body_age();
			monster_copy_complete(monster_board2);
			de = 4;
		}
		else {
			monster_copy(monster_board2);
			int de = 1;
			monster_move(monster_board2, monster_board1);
			de = 2;
			move_packman(monster_board1);
			de = 3;
			reduce_monster_body_age();
			monster_copy_complete(monster_board1);
			de = 4;
		}
	}
	int answer = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			answer += monster_board1[i][j].size();
			answer += monster_board2[i][j].size();
		}
	}
	cout << answer;
}


int main(void) {
	Input();
	simulation();
	return 0;
}