/*
20:56
*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

int K, M;
int board[5][5];
int rotate_board[5][5];
queue<int> treasure_q;
int answer;

void Input() {
	cin >> K >> M;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> board[i][j];
		}
	}

	int treasure;
	for (int i = 0; i < M; i++) {
		cin >> treasure;
		treasure_q.push(treasure);
	}
}

void print_situation() {
	cout << '\n';
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << rotate_board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void rotate_90(int x, int y, int k) {
	int temp_rotate_board[5][5] = { 0, };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			rotate_board[i][j] = board[i][j];
			temp_rotate_board[i][j] = board[i][j];
		}
	}

	for (int cnt = 0; cnt < k; cnt++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				rotate_board[j + x][(2 - i) + y] = temp_rotate_board[i + x][j + y];
			}
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp_rotate_board[i + x][j + y] = rotate_board[i + x][j + y];
			}
		}
	}
}

vector<vector<pair<int,int>>> obtain_traesure(int& treasure_cnt) {
	bool visited[5][5] = { 0, };
	vector<vector<pair<int, int>>> treasure_loc_v_v;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (visited[i][j]) continue;

			vector<pair<int, int>> loc_v;
			int cur_value = rotate_board[i][j];
			loc_v.push_back({ i,j });

			queue<pair<int, int>> q;
			q.push({ i,j });
			visited[i][j] = 1;

			while (!q.empty()) {
				auto cur = q.front();	q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= 5 || ny >= 5) continue;
					if (visited[nx][ny] || cur_value != rotate_board[nx][ny] || rotate_board[nx][ny] == 0) continue;

					loc_v.push_back({ nx,ny });
					q.push({ nx,ny });
					visited[nx][ny] = 1;

				}
			}

			if (loc_v.size() >= 3) {
				treasure_loc_v_v.push_back(loc_v);
				treasure_cnt += loc_v.size();
			}
		}
	}
	return treasure_loc_v_v;
	
}

bool explotion() {

	int max_find_treasure = 2;
	pair<int, int> select_start_loc = { 0,0 };
	int select_rotate_cnt = 0;

	for (int k = 1; k <= 3; k++) {
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 3; i++) {
			
				rotate_90(i, j, k);
				int find_treasure = 0;
				obtain_traesure(find_treasure);
				if (find_treasure > max_find_treasure) {
					max_find_treasure = find_treasure;
					select_start_loc = { i,j };
					select_rotate_cnt = k;
				}
			}
		}
	}

	if (max_find_treasure == 2) return false;

	// À¯¹° È¹µæ ÈÄ »èÁ¦
	rotate_90(select_start_loc.X, select_start_loc.Y,select_rotate_cnt);
	int find_treasure = 0;
	vector<vector<pair<int, int>>> treasure_loc_v_v = obtain_traesure(find_treasure);
	answer += find_treasure;


	for (int i = 0; i < treasure_loc_v_v.size(); i++) {
		for (int j = 0; j < treasure_loc_v_v[i].size(); j++) {
			pair<int, int> loc = treasure_loc_v_v[i][j];
			rotate_board[loc.X][loc.Y] = 0;
		}
	}

	return true;
}


void chaining_get_treasure() {

	while (true) {
		for (int j = 0; j < 5; j++) {
			for (int i = 4; i >= 0; i--) {
				if (rotate_board[i][j] == 0) {
					int treasure = treasure_q.front();	treasure_q.pop();
					rotate_board[i][j] = treasure;
				}
			}
		}

		//print_situation();
		int find_treasure = 0;
		vector<vector<pair<int, int>>> treasure_loc_v_v = obtain_traesure(find_treasure);
		answer += find_treasure;

		for (int i = 0; i < treasure_loc_v_v.size(); i++) {
			for (int j = 0; j < treasure_loc_v_v[i].size(); j++) {
				pair<int, int> loc = treasure_loc_v_v[i][j];
				rotate_board[loc.X][loc.Y] = 0;
			}
		}

		if (treasure_loc_v_v.size() == 0) break;

	}
}

void update_board() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j] = rotate_board[i][j];
		}
	}
}


void solution() {
	for (int i = 0; i < K; i++) {
		answer = 0;
		bool flag = explotion();
		if (flag == false) break;
		//print_situation();
		chaining_get_treasure();
		update_board();
		cout << answer << " ";
	}
}

int main(void) {
	Input();
	solution();

	return 0;
}