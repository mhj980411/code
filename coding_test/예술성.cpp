/*
20:30
*/

#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <unordered_set>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

int n;
int board[30][30];
int rotate_board[30][30];
int divide_board[30][30];
bool visited[30][30];

int block_cnt_arr[900];
int block_num_arr[900];
int touch_block_cnt_arr[900][900];
int group_cnt;
int answer;


void seperate_board() {

	memset(divide_board, 0, sizeof(divide_board));
	memset(block_cnt_arr, 0, sizeof(block_cnt_arr));
	memset(block_num_arr, 0, sizeof(block_num_arr));
	memset(touch_block_cnt_arr, 0, sizeof(touch_block_cnt_arr));
	int block_num = 1;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (divide_board[i][j] > 0) continue;

			queue<pair<int, int>> q;
			q.push({ i,j });
			divide_board[i][j] = block_num;
			block_num_arr[block_num - 1] = board[i][j];

			int block_cnt = 1;

			while (!q.empty()) {
				auto cur = q.front();	q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (divide_board[nx][ny] > 0 || board[nx][ny] != board[cur.X][cur.Y]) continue;
					q.push({ nx,ny });
					divide_board[nx][ny] = block_num;
					block_cnt++;
				}
			}
			block_cnt_arr[block_num - 1] = block_cnt;

			block_num++;
		}
	}
	group_cnt = block_num - 1;
}



void calc_touch_cnt() {
	memset(visited, 0, sizeof(visited));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (visited[i][j]) continue;

			visited[i][j] = 1;
			queue<pair<int, int>> q;
			q.push({ i,j });
			int group_num = divide_board[i][j];
			unordered_set<int> touch_group_num_us;

			while (!q.empty()) {
				auto cur = q.front();	q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (divide_board[nx][ny] != divide_board[cur.X][cur.Y]) {
						touch_block_cnt_arr[group_num - 1][divide_board[nx][ny] - 1]++;
						continue;
					}
					if (visited[nx][ny]) continue;
				}
			}
		}
	}

	for (int i = 0; i < group_cnt; i++) {
		for (int j = i + 1; j < group_cnt; j++) {
			answer += (block_cnt_arr[i] + block_cnt_arr[j])
				* block_num_arr[i] * block_num_arr[j]
				* touch_block_cnt_arr[i][j];
		}
	}
}

void rotate() {

	int middle_col = n / 2;
	for (int i = 0; i < n; i++) {
		rotate_board[(n - 1) - middle_col][i] = board[i][middle_col];
	}

	int middle_row = n / 2;
	for (int j = 0; j < n; j++) {
		rotate_board[(n - 1) - j][middle_col] = board[middle_row][j];
	}

	for (int i = 0; i < middle_col; i++) {
		for (int j = 0; j < middle_row; j++) {
			rotate_board[j][(middle_col - 1) - i] = board[i][j];
			rotate_board[j][((middle_col - 1) - i) + (middle_row + 1)] = board[i][j + (middle_row + 1)];
			rotate_board[j + (middle_col + 1)][(middle_col - 1) - i] = board[i + (middle_col + 1)][j];
			rotate_board[j + (middle_col + 1)][((middle_col - 1) - i) + (middle_row + 1)] =
				board[i + (middle_col + 1)][j + (middle_row + 1)];
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = rotate_board[i][j];
		}
	}
}

void simulation() {
	seperate_board();
	int de = 1;
	calc_touch_cnt();
	de = 2;
	for (int i = 0; i < 3; i++) {
		rotate();
		de = 3;
		seperate_board();
		de = 4;
		calc_touch_cnt();
		de = 5;
	}
	cout << answer;
}

void Input() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
}

int main(void) {
	ios::sync_with_stdio(0);
	cout.tie(0);
	cin.tie(0);

	Input();
	simulation();
	
	return 0;
}