/*
18:51
*/

#include <iostream>
#include <deque>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

#define X first
#define Y second

int medusa_dx[] = { -1,1,0,0 };
int medusa_dy[] = { 0,0,-1,1 };

int warrior_dx[] = { -1,1,0,0 };
int warrior_dy[] = { 0,0,-1,1 };

int warrior_dx2[] = { 0,0,-1,1 };
int warrior_dy2[] = { -1,1,0,0 };

int N, M;
pair<int, int> medusa_loc;
pair<int, int> park_loc;

struct warrior {
	int x;
	int y;
	bool isdead = false;
	bool isfinded = false;
};

int board[51][51];
queue<int> warrior_index_board[51][51];
warrior warrior_arr[301];
queue<pair<int, int>> stone_warrior_loc_q;

int medusa_sight_board[51][51];
int select_medusa_sight_board[51][51];

vector<pair<int, int>> medusa_route_v;
pair<int, int> route_board[51][51];
bool visited[51][51];

void Input() {
	cin >> N >> M;
	cin >> medusa_loc.X >> medusa_loc.Y >> park_loc.X >> park_loc.Y;

	warrior warrior_info;
	for (int i = 0; i < M; i++) {
		cin >> warrior_info.x >> warrior_info.y;
		warrior_arr[i] = warrior_info;
		warrior_index_board[warrior_info.x][warrior_info.y].push(i);
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
		}
	}
}

void print_situation() {
	cout << "medusa_loc\n";
	cout << medusa_loc.X << " " << medusa_loc.Y << '\n';

	cout << "medusa_sight_board\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << select_medusa_sight_board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';

	cout << "warior_index_board\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << warrior_index_board[i][j].size() << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

bool find_medusa_min_dist() {
	
	queue<pair<int, int>> q;
	q.push(medusa_loc);
	visited[medusa_loc.X][medusa_loc.Y] = 1;
	bool stop_flag = false;

	while (!q.empty()) {
		auto cur = q.front();	q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + medusa_dx[dir];
			int ny = cur.Y + medusa_dy[dir];
			if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
			if (board[nx][ny] == 1 || visited[nx][ny]) continue;
			
			q.push({ nx,ny });
			route_board[nx][ny] = cur;
			visited[nx][ny] = 1;

			if (nx == park_loc.X && ny == park_loc.Y) {
				stop_flag = true;
				break;
			}
		}
		if (stop_flag == true)
			break;
	}

	if (stop_flag) {
		pair<int, int> cur = park_loc;
		while (true) {
			pair<int, int> next = route_board[cur.X][cur.Y];
			if (next.X == medusa_loc.X && next.Y == medusa_loc.Y) break;

			medusa_route_v.push_back(next);
			cur = next;

			
		}
		reverse(medusa_route_v.begin(), medusa_route_v.end());
	}
	
	return stop_flag;
}

void Catch_Warrior(pair<int,int> medusa_loc) {
	int size = warrior_index_board[medusa_loc.X][medusa_loc.Y].size();
	if (size > 0) {
		for (int i = 0; i < size; i++) {
			int warrior_index = warrior_index_board[medusa_loc.X][medusa_loc.Y].front();
			warrior_index_board[medusa_loc.X][medusa_loc.Y].pop();

			warrior_arr[warrior_index].isdead = true;
		}
	}
}

void medusa_sight(int& finded_warrior_cnt) {

	int max_find_cnt = 0;
	int select_dir = 0;
	for (int dir = 0; dir < 4; dir++) {
		int cur_find_cnt = 0;
		memset(medusa_sight_board, 0, sizeof(medusa_sight_board));

		int dx[3] = { medusa_dx[dir]};
		int dy[3] = { medusa_dy[dir]};

		if (dir <= 1) {
			dx[1] = medusa_dx[dir] + medusa_dx[2];
			dy[1] = medusa_dy[dir] + medusa_dy[2];

			dx[2] = medusa_dx[dir] + medusa_dx[3];
			dy[2] = medusa_dy[dir] + medusa_dy[3];
		}
		else {
			dx[1] = medusa_dx[dir] + medusa_dx[0];
			dy[1] = medusa_dy[dir] + medusa_dy[0];

			dx[2] = medusa_dx[dir] + medusa_dx[1];
			dy[2] = medusa_dy[dir] + medusa_dy[1];
		}


		// 상하좌우 중 한방향
		int nx = medusa_loc.X;	int ny = medusa_loc.Y;
		while (true) {
			nx += dx[0];	ny += dy[0];
			if (nx < 0 || ny < 0 || nx >= N || ny >= N) break;

			if (warrior_index_board[nx][ny].size() > 0) {
				cur_find_cnt += warrior_index_board[nx][ny].size();
				medusa_sight_board[nx][ny] = 2;
				break;
			}

			medusa_sight_board[nx][ny] = 1;
		}

		// 대각선 방향
		for (int diagonol_index = 1; diagonol_index <= 2; diagonol_index++) {
			nx = medusa_loc.X;	ny = medusa_loc.Y;
			while (true) {
				nx += dx[diagonol_index];	ny += dy[diagonol_index];
				if (nx < 0 || ny < 0 || nx >= N || ny >= N) break;

				if (warrior_index_board[nx][ny].size() > 0) {
					cur_find_cnt += warrior_index_board[nx][ny].size();
					medusa_sight_board[nx][ny] = 2;
					break;
				}

				medusa_sight_board[nx][ny] = 1;

				// 메두사가 바라보는 방향으로 쭉 서치
				int nx_move = nx, ny_move = ny;
				while (true) {
					nx_move += dx[0];	ny_move += dy[0];
					if (nx_move < 0 || ny_move < 0 || nx_move >= N || ny_move >= N) break;
					if (medusa_sight_board[nx_move][ny_move] != 0) break;

					if (warrior_index_board[nx_move][ny_move].size() > 0) {
						cur_find_cnt += warrior_index_board[nx_move][ny_move].size();
						medusa_sight_board[nx_move][ny_move] = 2;

						// 다시한번 대각선 방향 체크
						while (true) {
							nx_move += dx[diagonol_index];	ny_move += dy[diagonol_index];
							if (nx_move < 0 || ny_move < 0 || nx_move >= N || ny_move >= N) break;
							medusa_sight_board[nx_move][ny_move] = 3;
						}
						break;
					}
					medusa_sight_board[nx_move][ny_move] = 1;
				}
			}
		}

		if (cur_find_cnt > max_find_cnt) {
			max_find_cnt = cur_find_cnt;
			select_dir = dir;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					select_medusa_sight_board[i][j] = medusa_sight_board[i][j];
				}
			}
		}
		int de = 1;
	}
	// 돌로 변한 전사들 체크
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (select_medusa_sight_board[i][j] == 2) {
				int size = warrior_index_board[i][j].size();
				for (int k = 0; k < size; k++) {
					int index = warrior_index_board[i][j].front();	warrior_index_board[i][j].pop();
					warrior_arr[index].isfinded = true;
					warrior_index_board[i][j].push(index);
				}
				stone_warrior_loc_q.push({ i,j });
			}
		}
	}
	finded_warrior_cnt = max_find_cnt;

	int de = 2;
}

void move_warrior(int& move_warrior_dist, int&attack_medusa_warrior_cnt) {
	for (int i = 0; i < M; i++) {
		warrior w = warrior_arr[i];
		if (w.isdead == true || w.isfinded == true) continue;
		warrior_index_board[w.x][w.y].pop();

		// 첫번재 이동

		int min_dist = abs(medusa_loc.X - w.x) + abs(medusa_loc.Y - w.y);
		pair<int, int> select_loc = { w.x, w.y };
		for (int dir = 0; dir < 4; dir++) {
			int nx = w.x + warrior_dx[dir];
			int ny = w.y + warrior_dy[dir];
			if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
			if (select_medusa_sight_board[nx][ny] == 1 || select_medusa_sight_board[nx][ny] == 2) continue;

			int dist = abs(medusa_loc.X - nx) + abs(medusa_loc.Y - ny);
			if (dist < min_dist) {
				min_dist = dist;
				select_loc = { nx,ny };
			}
		}
		if (w.x != select_loc.X || w.y != select_loc.Y)
			move_warrior_dist++;

		w.x = select_loc.X;		w.y = select_loc.Y;

		// 두번째 이동
		min_dist = abs(medusa_loc.X - w.x) + abs(medusa_loc.Y - w.y);
		for (int dir = 0; dir < 4; dir++) {
			int nx = w.x + warrior_dx2[dir];
			int ny = w.y + warrior_dy2[dir];
			if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
			if (select_medusa_sight_board[nx][ny] == 1 || select_medusa_sight_board[nx][ny] == 2) continue;

			int dist = abs(medusa_loc.X - nx) + abs(medusa_loc.Y - ny);
			if (dist < min_dist) {
				min_dist = dist;
				select_loc = { nx,ny };
			}
		}
		if (w.x != select_loc.X || w.y != select_loc.Y)
			move_warrior_dist++;

		w.x = select_loc.X;		w.y = select_loc.Y;

		// 전사의 공격
		if (medusa_loc.X == w.x && medusa_loc.Y == w.y) {
			attack_medusa_warrior_cnt++;
			w.isdead = true;
		}
		else {
			warrior_index_board[w.x][w.y].push(i);
		}
		warrior_arr[i] = w;
	}
}

void recover_warrior() {
	int size = stone_warrior_loc_q.size();
	for (int i = 0; i < size; i++) {
		pair<int, int> loc = stone_warrior_loc_q.front();	stone_warrior_loc_q.pop();
		
		int index_board_size = warrior_index_board[loc.X][loc.Y].size();
		for (int j = 0; j < index_board_size; j++) {
			int index = warrior_index_board[loc.X][loc.Y].front();
			warrior_index_board[loc.X][loc.Y].pop();
			warrior_arr[index].isfinded = false;
			warrior_index_board[loc.X][loc.Y].push(index);
		}
	}
}

void solution() {
	// 메두사의 이동
	bool flag = find_medusa_min_dist();
	if (flag == false) {
		cout << -1;
		return;
	}

	for (int i = 0; i < medusa_route_v.size(); i++) {
		int move_warrior_dist = 0;
		int finded_warrior_cnt = 0;
		int attack_medusa_warrior_cnt = 0;

		medusa_loc = medusa_route_v[i];
		Catch_Warrior(medusa_loc);
		//print_situation();
		medusa_sight(finded_warrior_cnt);
		//print_situation();
		move_warrior(move_warrior_dist, attack_medusa_warrior_cnt);
		//print_situation();
		recover_warrior();

		cout << move_warrior_dist << " " << finded_warrior_cnt << " " << attack_medusa_warrior_cnt << '\n';
	}

	cout << 0;
}

int main(void) {
	Input();
	solution();

	return 0;
}

/*
7 4
1 1 5 0
5 3 4 5 6 3 3 1
0 0 0 0 0 1 0
0 0 0 1 0 1 0
0 1 1 0 0 0 0
1 0 0 0 0 0 0
0 0 1 0 0 0 0
0 0 0 0 0 1 0
0 0 0 0 1 0 0


*/