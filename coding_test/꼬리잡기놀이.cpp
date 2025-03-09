/*
16:50
*/

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

int n, m, k;
int answer;

vector<vector<pair<int,int>>> team_loc_v_v;

int board[20][20];
pair<int,int> team_board[20][20];
bool visited[20][20];

void dfs(int level, pair<int, int> cur_loc, int index) {
	
	for (int dir = 0; dir < 4; dir++) {
		int nx = cur_loc.X + dx[dir];
		int ny = cur_loc.Y + dy[dir];

		if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
		if (visited[nx][ny] || board[nx][ny] == 4 || board[nx][ny] == 0) continue;
		if (level == 0 && board[nx][ny] == 3) continue;
		team_loc_v_v[index].push_back({ nx,ny });
		team_board[nx][ny] = { index, team_board[cur_loc.X][cur_loc.Y].second + 1 };
		board[nx][ny] = 4;
		visited[nx][ny] = 1;
		dfs(level + 1,{ nx,ny }, index);
	}
}

void Input() {
	cin >> n >> m >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1) {
				vector<pair<int, int>> team_loc_v;

				team_board[i][j] = { team_loc_v_v.size(),1 };

				team_loc_v.push_back({ i,j });
				team_loc_v_v.push_back(team_loc_v);
				board[i][j] = 4;
			}
		}
	}

	for (int i = 0; i < m; i++) {
		dfs(0,team_loc_v_v[i][0], i);
	}
}

void team_move() {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < team_loc_v_v[i].size(); j++) {
			pair<int, int> team_loc = team_loc_v_v[i][j];
			
			// 머리사람일 경우
			if (j == 0) {
				for (int dir = 0; dir < 4; dir++) {
					int nx = team_loc.X + dx[dir];
					int ny = team_loc.Y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (team_board[nx][ny].second == 2) continue;
					if (board[nx][ny] == 4 || team_board[nx][ny].second == team_loc_v_v[i].size()) {
						team_loc = { nx,ny };
						team_board[nx][ny] = { i,j + 1 };
						break;
					}
				}
			}
			// 꼬리사람일 경우
			else if (j == team_loc_v_v[i].size() - 1) {
				for (int dir = 0; dir < 4; dir++) {
					int nx = team_loc.X + dx[dir];
					int ny = team_loc.Y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (team_board[nx][ny].second == team_loc_v_v[i].size() - 1) {
						if (team_board[team_loc.X][team_loc.Y].second != 1)
							team_board[team_loc.X][team_loc.Y] = { 0, 0};
						team_loc = { nx,ny };
						team_board[nx][ny] = { i,team_loc_v_v[i].size() };
						break;
					}
				}
			}
			// 중간사람일 경우
			else {
				for (int dir = 0; dir < 4; dir++) {
					int nx = team_loc.X + dx[dir];
					int ny = team_loc.Y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
					if (team_board[nx][ny].second == team_board[team_loc.X][team_loc.Y].second - 1) {
						team_loc = { nx,ny };
						team_board[nx][ny] = { i,j + 1 };
						break;
					}
				}
			}


			team_loc_v_v[i][j] = team_loc;
		}
	}
}

void get_score(pair<int,int> loc) {
	answer += pow(team_board[loc.X][loc.Y].second, 2);

	int team_index = team_board[loc.X][loc.Y].first;

	vector<pair<int, int>> team_loc_v;
	for (int j = team_loc_v_v[team_index].size() - 1; j >= 0; j--) {
		team_loc_v.push_back(team_loc_v_v[team_index][j]);
		team_board[team_loc_v_v[team_index][j].X][team_loc_v_v[team_index][j].Y] = { team_index,team_loc_v.size() };
	}

	team_loc_v_v[team_index] = team_loc_v;
}

void throw_ball(int round) {
	int index = round - 1;
	int quotient = index / n;
	int remainder = index % n;

	if (quotient == 0) {
		for (int j = 0; j < n; j++) {
			if (team_board[remainder][j].second > 0) {
				get_score({ remainder,j });
				break;
			}
		}
	}
	else if (quotient == 1) {
		for (int i = n - 1; i >= 0; i--) {
			if (team_board[i][remainder].second > 0) {
				get_score({ i,remainder });
				break;
			}
		}
	}
	else if (quotient == 2) {
		for (int j = n - 1; j >= 0; j--) {
			if (team_board[(n - 1) - remainder][j].second > 0) {
				get_score({ (n - 1) - remainder,j });
				break;
			}
		}
	}
	else if (quotient == 3) {
		for (int i = 0; i < n; i++) {
			if (team_board[i][(n - 1) - remainder].second > 0) {
				get_score({ i,(n - 1) - remainder });
				break;
			}
		}
	}
}

void print_situation() {
	cout << "team_board\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << team_board[i][j].second << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	int turn = 1;
	for (int i = 0; i < k; i++) {
		//cout << "turn : " << turn << '\n';
		team_move();
		/*print_situation();
		int de = 1;*/
		throw_ball(turn);
		/*print_situation();
		de = 2;*/
		
		//cout << "answer : " << answer << '\n';
		turn++;
		if (turn == (4 * n + 1))
			turn = 1;
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();
	return 0;
}