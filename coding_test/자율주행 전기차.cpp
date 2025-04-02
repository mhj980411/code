#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

struct Car {
	int x;
	int y;
	int cur_fuel;
};

Car car_info;

int n, m, c;

int board[20][20];
int person_board[20][20];
vector<pair<int, int>> destination_loc_v;
int dist[20][20];

void Input() {
	cin >> n >> m >> c;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
	cin >> car_info.x >> car_info.y;
	car_info.x--;	car_info.y--;	car_info.cur_fuel = c;

	pair<int, int> person_loc;	pair<int, int> destination_loc;
	for (int i = 0; i < m; i++) {
		cin >> person_loc.X >> person_loc.Y >> destination_loc.X >> destination_loc.Y;
		person_loc.X--;	person_loc.Y--;	destination_loc.X--; destination_loc.Y--;
		person_board[person_loc.X][person_loc.Y] = i + 1;
		destination_loc_v.push_back(destination_loc);
	}
}

int pick_up_person() {

	memset(dist, 0, sizeof(dist));

	queue<pair<int, int>> q;
	q.push({ car_info.x, car_info.y });
	dist[car_info.x][car_info.y] = 1;

	while (!q.empty()) {
		auto cur = q.front();	q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (board[nx][ny] == 1 || dist[nx][ny] > 0) continue;
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx,ny });
		}
	}

	pair<int, int> select_person_loc;
	int min_dist = 21e8;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (person_board[i][j] > 0 && dist[i][j] > 0) {
				if (min_dist > dist[i][j]) {
					min_dist = dist[i][j];
					select_person_loc = { i,j };
				}
			}
		}
	}
	
	int ret = -1;
	if (min_dist != 21e8) {
		car_info.x = select_person_loc.X;	car_info.y = select_person_loc.Y;
		car_info.cur_fuel -= (dist[select_person_loc.X][select_person_loc.Y] - 1);
		if (car_info.cur_fuel >= 0) {
			ret = person_board[select_person_loc.X][select_person_loc.Y];
		}
		person_board[select_person_loc.X][select_person_loc.Y] = 0;
	}

	return ret;
}

bool move_destination(int person_num) {

	memset(dist, 0, sizeof(dist));


	queue<pair<int, int>> q;
	q.push({ car_info.x, car_info.y });
	dist[car_info.x][car_info.y] = 1;
	pair<int, int> select_loc = { -1,-1 };

	while (!q.empty()) {
		auto cur = q.front();	q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (board[nx][ny] == 1 || dist[nx][ny] > 0) continue;
			dist[nx][ny] = dist[cur.X][cur.Y] + 1;
			q.push({ nx,ny });
			if (nx == destination_loc_v[person_num-1].X && ny == destination_loc_v[person_num - 1].Y) {
				select_loc = { nx,ny };
				break;
			}
		}
		if (select_loc.X != -1) break;
	}

	bool ret = false;
	if (select_loc.X != -1) {
		int use_fuel = dist[select_loc.X][select_loc.Y] - 1;
		car_info.cur_fuel -= use_fuel;
		if (car_info.cur_fuel >= 0) {
			car_info.cur_fuel += (use_fuel * 2);
			ret = true;
		}
		car_info.x = select_loc.X;	car_info.y = select_loc.Y;
		
		
	}

	return ret;
}


void simulation() {
	for (int i = 0; i < m; i++) {
		int person_num = pick_up_person();
		if (person_num == -1) {
			car_info.cur_fuel = -1;
			break;
		}
		bool flag = move_destination(person_num);
		if (flag == false) {
			car_info.cur_fuel = -1;
			break;
		}
	}
	cout << car_info.cur_fuel;
}

int main(void) {
	Input();
	simulation();


	return 0;
}