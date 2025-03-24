#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

int n, m;
int board[20][20];
int rotate_board[20][20];
bool visited[20][20];
int answer;

struct bomb_group{
	vector<pair<int, int>> red_bomb_loc_v;
	pair<int, int> standard_bomb_loc;
	vector<pair<int, int>> bomb_loc_v;
};

void Input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
}

bool find_bomb_group() {
	memset(visited, 0, sizeof(visited));

	bomb_group select_bomb_group;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (visited[i][j] || board[i][j] <= 0) continue;

			queue<pair<int, int>> q;
			visited[i][j] = 1;
			q.push({ i,j });
			bomb_group bg;
			bg.bomb_loc_v.push_back({ i,j });
			bg.standard_bomb_loc = { i,j };
			int board_num = board[i][j];

			while (!q.empty()) {
				auto cur = q.front();	q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (visited[nx][ny] || board[nx][ny] < 0) continue;
					if (board[nx][ny] != board_num && board[nx][ny] != 0) continue;

					if (board[nx][ny] == 0) bg.red_bomb_loc_v.push_back({ nx,ny });
					else {
						if (nx > bg.standard_bomb_loc.X)
							bg.standard_bomb_loc = { nx,ny };
						else if (nx == bg.standard_bomb_loc.X)
							if (ny < bg.standard_bomb_loc.Y)
								bg.standard_bomb_loc = { nx,ny };
					}
					visited[nx][ny] = 1;
					q.push({ nx,ny });
					bg.bomb_loc_v.push_back({ nx,ny });
				}
			}

			for (int k = 0; k < bg.red_bomb_loc_v.size(); k++) {
				visited[bg.red_bomb_loc_v[k].X][bg.red_bomb_loc_v[k].Y] = 0;
			}
			
			if (bg.bomb_loc_v.size() >= 2) {
				if (bg.bomb_loc_v.size() > select_bomb_group.bomb_loc_v.size())
					select_bomb_group = bg;
				else if (bg.bomb_loc_v.size() == select_bomb_group.bomb_loc_v.size()) {
					if (bg.red_bomb_loc_v.size() < select_bomb_group.red_bomb_loc_v.size())
						select_bomb_group = bg;
					else if (bg.red_bomb_loc_v.size() == select_bomb_group.red_bomb_loc_v.size()) {
						if (bg.standard_bomb_loc.X > select_bomb_group.standard_bomb_loc.X)
							select_bomb_group = bg;
						else if (bg.standard_bomb_loc.X == select_bomb_group.standard_bomb_loc.X) {
							if (bg.standard_bomb_loc.Y < select_bomb_group.standard_bomb_loc.Y)
								select_bomb_group = bg;
						}
					}
				}
			}
		}
	}

	if (select_bomb_group.bomb_loc_v.size() == 0) return false;

	int size = select_bomb_group.bomb_loc_v.size();
	answer += (size * size);

	for (int i = 0; i < size; i++) {
		board[select_bomb_group.bomb_loc_v[i].X][select_bomb_group.bomb_loc_v[i].Y] = -2;
	}


	return true;
}

void gravity() {
	for (int i = n - 1; i >= 0; i--) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] < 0) continue;

			int nx = i;

			while (true) {
				if (nx + 1 >= n || board[nx + 1][j] != -2) break;
				nx++;
			}
			board[nx][j] = board[i][j];
			if(nx != i)
				board[i][j] = -2;
		}
	}
}

void rotate() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			rotate_board[(n - 1) - j][i] = board[i][j];
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = rotate_board[i][j];
		}
	}
}

void print_situation() {
	cout << "board\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << "   ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	while (find_bomb_group()) {
		//print_situation();
		gravity();
		//print_situation();
		rotate();
		//print_situation();
		gravity();
		//print_situation();
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();
	return 0;
}