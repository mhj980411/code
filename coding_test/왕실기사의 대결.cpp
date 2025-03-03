/*
18:25
*/

#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <unordered_set>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };

int board[40][40];
int knight_board[40][40];
bool visited[40][40];

int L, N, Q;

struct knight {
	int r;
	int c;
	int h;
	int w;
	int k;
	int cur_k;
};

vector<knight> knight_v;
vector<pair<int, int>> order_v;

void Input() {
	cin >> L >> N >> Q;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cin >> board[i][j];
		}
	}

	knight k;
	for (int i = 0; i < N; i++) {
		cin >> k.r >> k.c >> k.h >> k.w >> k.k;
		k.r--;	k.c--;
		k.cur_k = k.k;
		knight_v.push_back(k);

		for (int x = k.r; x < k.r + k.h; x++) {
			for (int y = k.c; y < k.c + k.w; y++) {
				knight_board[x][y] = i + 1;
			}
		}
	}
	
	pair<int, int> order;
	for (int i = 0; i < Q; i++) {
		cin >> order.first >> order.second;
		order_v.push_back(order);
	}
}

void knight_move_and_fight_damage(pair<int,int> order) {
	// 체스판에서 사라진 기사에게 명령을 내리면 아무 반응 X
	if (knight_v[order.first - 1].cur_k <= 0) return;

	memset(visited, 0, sizeof(visited));

	knight kn = knight_v[order.first - 1];

	unordered_set<int> knight_num_s;
	knight_num_s.insert(order.first);

	queue<pair<int, int>> q;
	q.push({ kn.r, kn.c });
	visited[kn.r][kn.c] = 1;

	bool move_flag = true;

	while (!q.empty()) {
		auto cur = q.front();	q.pop();

		int move_nx = cur.X + dx[order.second];
		int move_ny = cur.Y + dy[order.second];

		if ((move_nx < 0 || move_ny < 0 || move_nx >= L || move_ny >= L)
			|| (board[move_nx][move_ny] == 2)) {
			move_flag = false;
			break;
		}

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			
			if (nx < 0 || ny < 0 || nx >= L || ny >= L) continue;
			if (visited[nx][ny]) continue;
			if (knight_board[nx][ny] != knight_board[cur.X][cur.Y]) {
				if (knight_board[nx][ny] > 0 && dir == order.second) {
					if (knight_num_s.find(knight_board[nx][ny]) == knight_num_s.end()) {
						knight_num_s.insert(knight_board[nx][ny]);
					}
				}
				else
					continue;
			}
			visited[nx][ny] = 1;
			q.push({ nx,ny });
		}
	}

	if (move_flag == true) {
		for (auto i = knight_num_s.begin(); i != knight_num_s.end(); i++) {
			int knight_num = *i;
			knight move_knight = knight_v[knight_num - 1];

			for (int x = move_knight.r; x < move_knight.r + move_knight.h; x++) {
				for (int y = move_knight.c; y < move_knight.c + move_knight.w; y++) {
					knight_board[x][y] = 0;
				}
			}
		}

		for (auto i = knight_num_s.begin(); i != knight_num_s.end(); i++) {
			int knight_num = *i;
			knight move_knight = knight_v[knight_num - 1];
			
			move_knight.r += dx[order.second];
			move_knight.c += dy[order.second];

			for (int x = move_knight.r; x < move_knight.r + move_knight.h; x++) {
				for (int y = move_knight.c; y < move_knight.c + move_knight.w; y++) {
					if (board[x][y] == 1 && knight_num != order.first) {
						move_knight.cur_k--;
					}
				}
			}

			if (move_knight.cur_k > 0) {
				for (int x = move_knight.r; x < move_knight.r + move_knight.h; x++) {
					for (int y = move_knight.c; y < move_knight.c + move_knight.w; y++) {
						knight_board[x][y] = knight_num;
					}
				}
			}



			knight_v[knight_num - 1] = move_knight;
		}
	}
}

void print_situation() {
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cout << knight_board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}
// 상우하좌
void simulation() {
	for (int i = 0; i < Q; i++) {
		pair<int, int> order = order_v[i];
		knight_move_and_fight_damage(order);
		//print_situation();
		//int de = 1;
	}

	int answer = 0;
	for (int i = 0; i < N; i++) {
		if (knight_v[i].cur_k > 0) {
			answer += (knight_v[i].k - knight_v[i].cur_k);
		}
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();
	return 0;
}