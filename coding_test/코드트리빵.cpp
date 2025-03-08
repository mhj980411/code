#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,0,0,1 };
int dy[] = { 0,-1,1,0 };

int n, m;

int board[15][15];
pair<int, int> route_board[15][15];
bool visited[15][15];
int dist[15][15];

vector<pair<int, int>> convinient_store_loc_v;
int player_arrive_cnt;

struct player {
	int num;
	int x;
	int y;
};

queue<player> player_loc_q;

void Input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
	pair<int, int> loc;
	for (int i = 0; i < m; i++) {
		cin >> loc.X >> loc.Y;
		loc.X--;	loc.Y--;
		convinient_store_loc_v.push_back(loc);
	}
}

void player_move() {
	vector<pair<int, int>> player_arrive_loc_v;

	int size = player_loc_q.size();
	for (int i = 0; i < size; i++) {
		player p = player_loc_q.front();	player_loc_q.pop();

		memset(visited, 0, sizeof(visited));
		memset(route_board, 0, sizeof(route_board));

		queue<pair<int, int>> q;
		q.push({p.x,p.y});
		visited[p.x][p.y] = 1;
		bool stop_flag = false;

		while (!q.empty()) {
			auto cur = q.front();	q.pop();
			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];
				if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
				if (visited[nx][ny] || board[nx][ny] < 0) continue;

				q.push({ nx,ny });
				visited[nx][ny] = 1;
				route_board[nx][ny] = cur;

				if (nx == convinient_store_loc_v[p.num - 1].X && ny == convinient_store_loc_v[p.num - 1].Y) {
					stop_flag = true;
					break;
				}
			}
		}

		if (stop_flag == true) {
			pair<int, int> cur = convinient_store_loc_v[p.num - 1];
			while (true) {
				pair<int, int> next = route_board[cur.X][cur.Y];
				if (next.X == p.x && next.Y == p.y) break;
				cur = next;
			}
			p.x = cur.X;	p.y = cur.Y;
		}

		if (p.x == convinient_store_loc_v[p.num - 1].X && p.y == convinient_store_loc_v[p.num - 1].Y) {
			player_arrive_loc_v.push_back({ p.x,p.y });
		}
		else{
			player_loc_q.push(p);
		}
	}

	for (int i = 0; i < player_arrive_loc_v.size(); i++) {
		board[player_arrive_loc_v[i].X][player_arrive_loc_v[i].Y] = -1;
		player_arrive_cnt++;
	}
}

void find_basecamp(int time) {
	if (time > m) return;

	memset(dist, 0, sizeof(dist));

	pair<int, int> convinient_loc = convinient_store_loc_v[time - 1];
	queue<pair<int, int>> q;
	q.push(convinient_loc);
	dist[convinient_loc.X][convinient_loc.Y];

	while (!q.empty()) {
		auto cur = q.front();	q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (dist[nx][ny] > 0 || board[nx][ny] < 0) continue;

			q.push({ nx,ny });
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
		}
	}

	pair<int, int> select_loc;
	int min_dist = 21e8;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] == 1) {
				if (dist[i][j] > 0 && min_dist > dist[i][j]) {
					min_dist = dist[i][j];
					select_loc = { i,j };
				}
			}
		}
	}
	board[select_loc.X][select_loc.Y] = -1;
	player p = { time, select_loc.X, select_loc.Y };
	player_loc_q.push(p);
}

void situation() {
	int size = player_loc_q.size();
	int player_board[15][15] = { 0, };
	for (int i = 0; i < size; i++) {
		player p = player_loc_q.front();	player_loc_q.pop();
		player_board[p.x][p.y] = p.num;
		player_loc_q.push(p);
	}
	cout << "player_board\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << player_board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	int time = 1;
	while (true) {
		player_move();
		find_basecamp(time);
		//situation();
		if (m == player_arrive_cnt) break;
		time++;
	}
	cout << time;
}

int main(void) {
	Input();
	simulation();
	return 0;
}