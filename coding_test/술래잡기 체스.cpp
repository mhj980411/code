#include <iostream>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,-1,0,1,1,1,0,-1 };
int dy[] = { 0,-1,-1,-1,0,1,1,1 };

pair<int, int> board[4][4];

struct thief {
	int x;
	int y;
	int dir;
	bool Iscatch = false;
};

thief thief_arr[16];

struct catcher {
	int x;
	int y;
	int dir;
};

catcher catcher_info;

int answer;
int max_answer = -21e8;

void Input() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cin >> board[i][j].first >> board[i][j].second;
			thief_arr[board[i][j].first - 1] = { i,j,board[i][j].second - 1};
		}
	}
	answer += board[0][0].first;
	thief_arr[board[0][0].first - 1].Iscatch = true;
	catcher_info = { 0,0,board[0][0].second - 1};
	board[0][0] = { 0,0 };
}

void move_thief() {
	for (int i = 0; i < 16; i++) {
		if (thief_arr[i].Iscatch == true) continue;
		int dir = thief_arr[i].dir;

		int nx = thief_arr[i].x + dx[dir];
		int ny = thief_arr[i].y + dy[dir];
		
		bool move_flag = false;

		for (int j = 0; j < 8; j++) {
			if (!((nx == catcher_info.x && ny == catcher_info.y) || (nx < 0 || ny < 0 || nx >= 4 || ny >= 4))) {
				move_flag = true;
				break;
			}
			dir = (dir + 1) % 8;
			nx = thief_arr[i].x + dx[dir];
			ny = thief_arr[i].y + dy[dir];
		}
		
		if (move_flag == true) {
			board[thief_arr[i].x][thief_arr[i].y].second = dir + 1;	// 얘때문에 틀림 ㅠ
			pair<int, int> moved_block = board[nx][ny];
			if (!(moved_block.first == 0)) { // 중괄호 제외 실수 ㅠ
				thief_arr[moved_block.first - 1].x = thief_arr[i].x;	thief_arr[moved_block.first - 1].y = thief_arr[i].y;
			}
			board[nx][ny] = board[thief_arr[i].x][thief_arr[i].y];
			board[thief_arr[i].x][thief_arr[i].y] = moved_block;
			thief_arr[i].x = nx;	thief_arr[i].y = ny;	thief_arr[i].dir = dir;
			
		}
	}
}

void dfs(int level) {

	if (level >= 16) return;
	max_answer = max(max_answer, answer);

	catcher backup_catcher = catcher_info;
	thief backup_thief_arr[16];
	pair<int, int> backup_board[16][16];

	for (int i = 0; i < 16; i++) {
		backup_thief_arr[i] = thief_arr[i];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			backup_board[i][j] = board[i][j];
		}
	}

	move_thief();

	for (int dist = 1; dist < 4; dist++) {
		int nx = catcher_info.x + (dx[catcher_info.dir] * dist);
		int ny = catcher_info.y + (dy[catcher_info.dir] * dist);
		if (nx < 0 || ny < 0 || nx >= 4 || ny >= 4) break;
		if (board[nx][ny].first == 0 && board[nx][ny].second == 0) continue;

		catcher_info.x = nx;	catcher_info.y = ny;	catcher_info.dir = board[nx][ny].second-1;
		pair<int, int> backup_board = board[nx][ny];
		answer += board[nx][ny].first;
		thief_arr[board[nx][ny].first - 1].Iscatch = true;
		board[nx][ny] = { 0, 0 };
		dfs(level + 1);
		board[nx][ny] = backup_board;
		thief_arr[board[nx][ny].first - 1].Iscatch = false;
		answer -= board[nx][ny].first;
		catcher_info = backup_catcher;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = backup_board[i][j];
		}
	}

	for (int i = 0; i < 16; i++) {
		thief_arr[i] = backup_thief_arr[i];
	}
}

int main(void) {
	Input();
	dfs(0);
	int de = 1;
	cout << max_answer;
	return 0;
}