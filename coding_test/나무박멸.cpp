#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0,1,1,-1,-1 };
int dy[] = { 0,1,0,-1,1,-1,1,-1 };

int n, m, k, c;

int board[20][20];
int spread_tree_board[20][20];
int tree_killer_board[20][20];
int answer;

void Input() {
	cin >> n >> m >> k >> c;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
}

void grow_tree() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {

			if (board[i][j] > 0) {
				int tree_cnt = 0;
				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (board[nx][ny] > 0)
						tree_cnt++;
				}
				board[i][j] += tree_cnt;
			}
		}
	}
}

void spread_tree() {

	memset(spread_tree_board, 0, sizeof(spread_tree_board));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				vector<pair<int, int>> empty_loc_v;

				for (int dir = 0; dir < 4; dir++) {
					int nx = i + dx[dir];
					int ny = j + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (board[nx][ny] == 0 && tree_killer_board[nx][ny] == 0) {
						empty_loc_v.push_back({ nx,ny });
					}
				}

				if (empty_loc_v.size() > 0) {

					int spread_tree_value = board[i][j] / empty_loc_v.size();

					for (int k = 0; k < empty_loc_v.size(); k++) {
						spread_tree_board[empty_loc_v[k].X][empty_loc_v[k].Y] += spread_tree_value;
					}
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (spread_tree_board[i][j] > 0) {
				board[i][j] += spread_tree_board[i][j];
			}
		}
	}
}

void spread_tree_killer() {
	int max_tree_kill = -21e8;
	pair<int, int> select_loc;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) {
				int tree_kill = board[i][j];
				for (int dir = 4; dir < 8; dir++) {
					for (int dist = 1; dist <= k; dist++) {
						int nx = i + (dx[dir] * dist);
						int ny = j + (dy[dir] * dist);

						if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
						if (board[nx][ny] == 0 || board[nx][ny] == -1) break;

						tree_kill += board[nx][ny];
					}
				}
				if (max_tree_kill < tree_kill) {
					max_tree_kill = tree_kill;
					select_loc = { i,j };
				}
			}
		}
	}

	if (max_tree_kill != -21e8) {
		tree_killer_board[select_loc.X][select_loc.Y] = c + 1;
		answer += board[select_loc.X][select_loc.Y];
		board[select_loc.X][select_loc.Y] = 0;
		
		for (int dir = 4; dir < 8; dir++) {
			for (int dist = 1; dist <= k; dist++) {
				int nx = select_loc.X + (dx[dir] * dist);
				int ny = select_loc.Y + (dy[dir] * dist);

				if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
				if (board[nx][ny] == 0 || board[nx][ny] == -1) {
					tree_killer_board[nx][ny] = c + 1;
					break;
				}

				tree_killer_board[nx][ny] = c + 1;
				answer += board[nx][ny];
				board[nx][ny] = 0;
			}
		}
	}
}

void reduce_tree_killer_age() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tree_killer_board[i][j] > 0) {
				tree_killer_board[i][j]--;
			}
		}
	}
}


void print_situation() {
	cout << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}


void simulation() {
	for (int i = 0; i < m; i++) {
		grow_tree();
		/*print_situation();
		int de = 1;*/
		spread_tree();
		/*print_situation();
		de = 2;*/
		spread_tree_killer();
		/*print_situation();
		de = 3;*/
		reduce_tree_killer_age();
	}
	cout << answer;
}



int main(void) {
	Input();
	simulation();

	return 0;
}