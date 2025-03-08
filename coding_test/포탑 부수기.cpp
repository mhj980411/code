/*
15:11
*/

#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 0,1,0,-1,1,1,-1,-1 };
int dy[] = { 1,0,-1,0,1,-1,1,-1 };

int N, M, K;
int board[10][10];
bool visited[10][10];
int attack_time_board[10][10];
pair<int, int> route_board[10][10];
bool attack_visit_board[10][10];

void Input() {
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> board[i][j];
		}
	}
}

pair<int, int> find_attacker() {
	pair<int, int> ret = {0,0};
	int min_power = 21e8;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] == 0) continue;

			if (board[i][j] < min_power) {
				min_power = board[i][j];
				ret = { i,j };
			}
			else if (board[i][j] == min_power) {
				if (attack_time_board[i][j] > attack_time_board[ret.X][ret.Y]) 
					ret = { i,j };
				else if (attack_time_board[i][j] == attack_time_board[ret.X][ret.Y]) {
					if (i + j > ret.X + ret.Y) 
						ret = { i,j };
					else if (i + j == ret.X + ret.Y) {
						if (j > ret.Y) 
							ret = { i,j };
					}
				}
			}
		}
	}

	return ret;
}

pair<int, int> find_target() {
	pair<int, int> ret;
	int max_power = -21e8;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] == 0) continue;

			if (board[i][j] > max_power) {
				max_power = board[i][j];
				ret = { i,j };
			}
			else if (board[i][j] == max_power) {
				if (attack_time_board[i][j] < attack_time_board[ret.X][ret.Y])
					ret = { i,j };
				else if (attack_time_board[i][j] == attack_time_board[ret.X][ret.Y]) {
					if (i + j < ret.X + ret.Y) 
						ret = { i,j };
					else if (i + j == ret.X + ret.Y) {
						if (j < ret.Y)
							ret = { i,j };
					}
				}
			}
		}
	}
	return ret;
}

bool laser_attack(pair<int, int> attacker_loc, pair<int, int> target_loc, int time) {

	memset(route_board, 0, sizeof(route_board));
	memset(visited, 0, sizeof(visited));

	queue<pair<int, int>> q;
	visited[attacker_loc.X][attacker_loc.Y] = 1;
	q.push(attacker_loc);
	bool find_target = false;

	while (!q.empty()) {
		auto cur = q.front();	q.pop();

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= N || ny >= M) {
				nx = (nx + N) % N;
				ny = (ny + M) % M;
			}
			if (board[nx][ny] == 0 || visited[nx][ny])
				continue;
			
			q.push({ nx,ny });
			route_board[nx][ny] = cur;
			visited[nx][ny] = 1;

			if (nx == target_loc.X && ny == target_loc.Y) {
				find_target = true;
				break;
			}
		}

		if (find_target == true)
			break;
	}

	if (find_target == true) {
		attack_time_board[attacker_loc.X][attacker_loc.Y] = time;
		attack_visit_board[attacker_loc.X][attacker_loc.Y] = 1;

		pair<int, int> cur = target_loc;
		attack_visit_board[cur.X][cur.Y] = 1;
		board[cur.X][cur.Y] -= board[attacker_loc.X][attacker_loc.Y];
		if (board[cur.X][cur.Y] < 0) board[cur.X][cur.Y] = 0;

		while (true) {
			pair<int, int> next = route_board[cur.X][cur.Y];
			if (next.X == attacker_loc.X && next.Y == attacker_loc.Y) break;
			attack_visit_board[next.X][next.Y] = 1;
			board[next.X][next.Y] -= (board[attacker_loc.X][attacker_loc.Y] / 2);
			if (board[next.X][next.Y] < 0) board[next.X][next.Y] = 0;
			cur = next;
		}
	}

	return find_target;
}

void bomb_attack(pair<int, int> attacker_loc, pair<int, int> target_loc, int time) {
	attack_time_board[attacker_loc.X][attacker_loc.Y] = time;
	attack_visit_board[attacker_loc.X][attacker_loc.Y] = 1;

	pair<int, int> cur = target_loc;
	attack_visit_board[cur.X][cur.Y] = 1;
	board[cur.X][cur.Y] -= board[attacker_loc.X][attacker_loc.Y];
	if (board[cur.X][cur.Y] < 0) board[cur.X][cur.Y] = 0;

	for (int dir = 0; dir < 8; dir++) {
		int nx = cur.X + dx[dir];
		int ny = cur.Y + dy[dir];
		if (nx < 0 || ny < 0 || nx >= N || ny >= M) {
			nx = (nx + N) % N;
			ny = (ny + M) % M;
		}
		if (board[nx][ny] == 0 || nx == attacker_loc.X && ny == attacker_loc.Y) continue;

		attack_visit_board[nx][ny] = 1;
		board[nx][ny] -= (board[attacker_loc.X][attacker_loc.Y] / 2);
		if (board[nx][ny] < 0) board[nx][ny] = 0;
	}
}

void repair_tower() {
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] == 0 || attack_visit_board[i][j] == 1) continue;
			board[i][j]++;
		}
	}
	memset(attack_visit_board, 0, sizeof(attack_visit_board));
}

void print_situation() {
	cout << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	for (int i = 1; i <= K; i++) {
		pair<int, int> attacker_loc = find_attacker();
		pair<int, int> target_loc = find_target();
		
		// 부서지지 않은 포탑이 1개가 된다면 그 즉시 중지
		if (attacker_loc.X == target_loc.X && attacker_loc.Y == target_loc.Y)
			break;

		board[attacker_loc.X][attacker_loc.Y] += (N + M);

		if (laser_attack(attacker_loc, target_loc, i) == false) {
			bomb_attack(attacker_loc, target_loc, i);
		}
		//print_situation();
		repair_tower();
		//print_situation();
		//int de = 1;
	}

	int answer = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] > answer)
				answer = board[i][j];
		}
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();

	return 0;
}

/*
9 10 14
0 0 4877 0 0 310 0 0 0 4220
4248 677 0 2225 1958 0 0 0 2146 319
447 1371 1977 4459 0 0 2792 0 0 4829
0 678 3370 1814 0 1630 0 0 3734 1822
0 4894 0 0 0 0 0 0 0 2987
0 0 1856 0 0 288 0 0 0 4383
0 1794 0 0 0 0 0 2503 0 0
81 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 4398

4269
*/