#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0, -1 };

int n,m;

int board[20][20];
bool visited[20][20];

int dice[6] = { 1,4,2,3,6,5 };
int dice_dir = 1;
pair<int, int> dice_loc;
int answer;

void Input() {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
}

void rotate_dice() {
	
	int nx = dice_loc.X + dx[dice_dir];
	int ny = dice_loc.Y + dy[dice_dir];

	if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
		dice_dir = (dice_dir + 2) % 4;
		nx = dice_loc.X + dx[dice_dir];
		ny = dice_loc.Y + dy[dice_dir];
	}

	dice_loc = { nx,ny };
	
	int rotate_dice[6] = { 0, };
	if (dice_dir == 0) {
		rotate_dice[0] = dice[5];
		rotate_dice[1] = dice[1];
		rotate_dice[2] = dice[0];
		rotate_dice[3] = dice[3];
		rotate_dice[4] = dice[2];
		rotate_dice[5] = dice[4];
	}
	else if (dice_dir == 1) {
		rotate_dice[0] = dice[1];
		rotate_dice[1] = dice[4];
		rotate_dice[2] = dice[2];
		rotate_dice[3] = dice[0];
		rotate_dice[4] = dice[3];
		rotate_dice[5] = dice[5];
	}
	else if (dice_dir == 2) {
		rotate_dice[0] = dice[2];
		rotate_dice[1] = dice[1];
		rotate_dice[2] = dice[4];
		rotate_dice[3] = dice[3];
		rotate_dice[4] = dice[5];
		rotate_dice[5] = dice[0];
	}
	else if (dice_dir == 3) {
		rotate_dice[0] = dice[3];
		rotate_dice[1] = dice[0];
		rotate_dice[2] = dice[2];
		rotate_dice[3] = dice[4];
		rotate_dice[4] = dice[1];
		rotate_dice[5] = dice[5];
	}

	for (int i = 0; i < 6; i++) {
		dice[i] = rotate_dice[i];
	}

	if (dice[4] > board[dice_loc.X][dice_loc.Y]) {
		dice_dir = ((dice_dir - 1) + 4) % 4;
	}
	else if(dice[4] < board[dice_loc.X][dice_loc.Y]) {
		dice_dir = (dice_dir + 1) % 4;
	}
}

void get_score() {
	memset(visited, 0, sizeof(visited));

	queue<pair<int, int>> q;
	q.push(dice_loc);
	visited[dice_loc.X][dice_loc.Y] = 1;
	int cnt = 1;

	while (!q.empty()) {
		auto cur = q.front();	q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (visited[nx][ny] || board[nx][ny] != board[cur.X][cur.Y]) continue;

			cnt++;
			q.push({ nx,ny });
			visited[nx][ny] = 1;
		}
	}

	answer += (cnt * board[dice_loc.X][dice_loc.Y]);
}

void simulation() {
	for (int i = 0; i < m; i++) {
		rotate_dice();
		get_score();
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();
	return 0;
}