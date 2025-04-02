/*
20:55
*/

#include <iostream>

using namespace std;

#define X first
#define Y second

int dx[] = { 0,1,0,-1 };
int dy[] = { -1,0,1,0 };

int n;

int board[500][500];
int answer;

void Input() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
}

void calc_dust(int nx,int ny,int dust, int& cur_dust) {
	cur_dust -= dust;
	if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
		answer += dust;
	}
	else {
		board[nx][ny] += dust;
	}
}

void dust_move(int x,int y,int dir) {
	
	if (board[x][y] == 0) return;

	int total_dust = board[x][y];
	int cur_dust = total_dust;
	board[x][y] = 0;

	int nx = x + (dx[dir] * 2);
	int ny = y + (dy[dir] * 2);
	int dust = (total_dust * 5) / 100;
	
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[dir] + dx[((dir - 1) + 4) % 4];
	ny = y + dy[dir] + dy[((dir - 1) + 4) % 4];
	dust = (total_dust * 10) / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[dir] + dx[(dir + 1) % 4];
	ny = y + dy[dir] + dy[(dir + 1) % 4];
	dust = (total_dust * 10) / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[((dir - 1) + 4) % 4];
	ny = y + dy[((dir - 1) + 4) % 4];
	dust = (total_dust * 7) / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[(dir + 1) % 4];
	ny = y + dy[(dir + 1) % 4];
	dust = (total_dust * 7) / 100;
	calc_dust(nx, ny, dust, cur_dust);
	
	nx = x + dx[((dir - 1) + 4) % 4] * 2;
	ny = y + dy[((dir - 1) + 4) % 4] * 2;
	dust = (total_dust * 2) / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[(dir + 1) % 4] * 2;
	ny = y + dy[(dir + 1) % 4] * 2;
	dust = (total_dust * 2) / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[(dir + 2) % 4] + dx[((dir - 1) + 4) % 4];
	ny = y + dy[(dir + 2) % 4] + dy[((dir - 1) + 4) % 4];
	dust = total_dust / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[(dir + 2) % 4] + dx[(dir + 1) % 4];
	ny = y + dy[(dir + 2) % 4] + dy[(dir + 1) % 4];
	dust = total_dust / 100;
	calc_dust(nx, ny, dust, cur_dust);

	nx = x + dx[dir];
	ny = y + dy[dir];
	calc_dust(nx, ny, cur_dust, cur_dust);
}

void simulation() {

	int dir = 0;
	int dist = 1;
	int cur_dist = 0;
	int ch_dir_cnt = 0;

	int nx = n / 2;
	int ny = n / 2;
	while (true) {
		nx += dx[dir];
		ny += dy[dir];
		dust_move(nx, ny, dir);
		cur_dist++;

		if (cur_dist == dist) {
			cur_dist = 0;
			dir = (dir + 1) % 4;
			ch_dir_cnt++;
		}

		if (ch_dir_cnt == 2) {
			ch_dir_cnt = 0;
			dist++;
		}
		

		if (nx == 0 && ny == 0) break;
	}
}

int main(void) {
	Input();
	simulation();
	cout << answer;

	return 0;
}