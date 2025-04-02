#include <iostream>
#include <queue>
#include <set>

using namespace std;

#define X first
#define Y second

int n, m, k;

int dx[] = { -1,-1,0,1,1,1,0 ,-1 };
int dy[] = { 0,1,1,1,0 ,-1,-1,-1 };

struct atom {
	int x;
	int y;
	int m;
	int s;
	int d;
};

queue<atom> atom_info_q;
queue<atom> atom_board[50][50];
set<pair<int, int>> atom_loc_s;

void Input() {
	cin >> n >> m >> k;

	atom a;
	for (int i = 0; i < m; i++) {
		cin >> a.x >> a.y >> a.m >> a.s >> a.d;
		a.x--;	a.y--;
		atom_info_q.push(a);
		atom_board[a.x][a.y].push(a);
	}
}

void move() {
	int size = atom_info_q.size();
	for (int i = 0; i < size; i++) {

		atom a = atom_info_q.front();	atom_info_q.pop();
		atom_board[a.x][a.y].pop();

		int nx = (a.x + (dx[a.d] * a.s) + (n * a.s)) % n;
		int ny = (a.y + (dy[a.d] * a.s) + (n * a.s)) % n;

		a.x = nx;	a.y = ny;

		atom_board[a.x][a.y].push(a);
		atom_loc_s.insert({ a.x,a.y });
	}
}
void after_move() {
	for (auto it = atom_loc_s.begin(); it != atom_loc_s.end(); it++) {
		pair<int, int> atom_loc = *it;
		if (atom_board[atom_loc.X][atom_loc.Y].size() >= 2) {
			int size = atom_board[atom_loc.X][atom_loc.Y].size();

			int total_mess = 0;
			int total_speed = 0;
			int total_cnt = size;
			int right_dir_flag = 0;
			int diagonol_dir_flag = 0;
			for (int i = 0; i < size; i++) {
				atom a = atom_board[atom_loc.X][atom_loc.Y].front();	atom_board[atom_loc.X][atom_loc.Y].pop();
				total_mess += a.m;
				total_speed += a.s;
				if (a.d % 2 == 0) right_dir_flag = 1;
				else diagonol_dir_flag = 1;
			}

			total_mess /= 5;
			total_speed /= total_cnt;
			int start_dir = 0;
			if (right_dir_flag + diagonol_dir_flag == 2) start_dir = 1;

			if (total_mess > 0) {
				for (int dir = start_dir; dir < 8; dir += 2) {
					atom small_a = { atom_loc.X,atom_loc.Y,total_mess,total_speed,dir };
					atom_board[small_a.x][small_a.y].push(small_a);
					atom_info_q.push(small_a);
				}
			}
		}
		else {
			atom_info_q.push(atom_board[atom_loc.X][atom_loc.Y].front());
		}
	}
}
int calc_answer() {
	int ret = 0;
	int size = atom_info_q.size();
	for (int i = 0; i < size; i++) {
		atom a = atom_info_q.front();	atom_info_q.pop();
		ret += a.m;
	}
	return ret;
}

void simulation() {
	for (int i = 0; i < k; i++) {
		move();
		after_move();
		atom_loc_s.clear();
	}
	cout << calc_answer();
}

int main(void) {

	Input();
	simulation();
	return 0;
}
