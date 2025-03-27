/*
22:46
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 0,1,-1,0 };
int dy[] = { 1,0,0,-1 };

int n, q;

int board[64][64];
int rotate_board[64][64];
int melt_board[64][64];
bool visited[64][64];
vector<int> rotate_level_v;

void Input() {
	cin >> n >> q;

	int size = pow(2, n);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cin >> board[i][j];
		}
	}
	int num;
	for (int i = 0; i < q; i++) {
		cin >> num;
		rotate_level_v.push_back(num);
	}
}

void rotate(int level) {
	if (level == 0) return;

	int size = pow(2, n);
	int select_size = pow(2, level);
	int select_small_size = pow(2, level - 1);

	for (int i = 0; i < size; i += select_size) {
		for (int j = 0; j < size; j += select_size) {
			int dir = 0;
			for (int x = i; x < i + select_size; x += select_small_size) {
				for (int y = j; y < j + select_size; y += select_small_size) {

					for (int nx = x; nx < x + select_small_size; nx++) {
						for (int ny = y; ny < y + select_small_size; ny++) {
							rotate_board[nx + (dx[dir] * select_small_size)][ny + (dy[dir] * select_small_size)]
								= board[nx][ny];
						}
					}
					dir++;
				}
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = rotate_board[i][j];
		}
	}
}

void melting() {
	memset(melt_board, 0, sizeof(melt_board));
	int size = pow(2, n);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) continue;

			int ice_cnt = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				if (nx < 0 || ny < 0 || nx >= size || ny >= size) continue;
				if (board[nx][ny] > 0) ice_cnt++;
			}

			if (ice_cnt < 3) {
				melt_board[i][j] = (board[i][j] - 1);
				board[i][j] = 101;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if(board[i][j] == 101)
				board[i][j] = melt_board[i][j];
		}
	}
}

int find_largest_group() {

	memset(visited, 0, sizeof(visited));
	int max_cnt = 0;

	int size = pow(2, n);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0 || visited[i][j]) continue;

			queue<pair<int, int>> q;
			q.push({ i,j });
			visited[i][j] = 1;
			int cnt = 1;

			while (!q.empty()) {
				auto cur = q.front();	q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (nx < 0 || ny < 0 || nx >= size || ny >= size) continue;
					if (visited[nx][ny] || board[nx][ny] == 0) continue;
					cnt++;
					visited[nx][ny] = 1;
					q.push({ nx,ny });
				}
			}
			if (max_cnt < cnt)
				max_cnt = cnt;
		}
	}

	return max_cnt;
}

int total_ice() {
	int size = pow(2, n);
	
	int ret = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] > 0)
				ret += board[i][j];
		}
	}
	return ret;
}

void simulation() {
	for (int i = 0; i < q; i++) {
		int level = rotate_level_v[i];
		rotate(level);
		int de = 1;
		melting();
	}
	cout << total_ice() << '\n';
	cout << find_largest_group();
}

int main(void) {
	Input();
	simulation();

	return 0;
}