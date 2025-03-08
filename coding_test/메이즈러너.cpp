#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };

int N, M, K;

int board[10][10];
queue<int> player_index_board[10][10];
int total_move_dist;
int out_cnt;

struct player {
	int x;
	int y;
	int Isout = false;
};

vector<player> player_v;
pair<int, int> exit_loc;

void Input() {
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
		}
	}

	player p;
	for (int i = 0; i < M; i++) {
		cin >> p.x >> p.y;
		p.x--;	p.y--;
		player_v.push_back(p);
		player_index_board[p.x][p.y].push(i);
	}
	cin >> exit_loc.X >> exit_loc.Y;
	exit_loc.X--;	exit_loc.Y--;
	board[exit_loc.X][exit_loc.Y] = -1;
}

void player_move() {
	for (int i = 0; i < M; i++) {
		if (player_v[i].Isout == true) continue;

		player p = player_v[i];
		player_index_board[p.x][p.y].pop();

		pair<int, int> select_loc = { p.x,p.y };
		int min_dist = abs(p.x - exit_loc.X) + abs(p.y - exit_loc.Y);
		
		for (int dir = 0; dir < 4; dir++) {
			int nx = p.x + dx[dir];
			int ny = p.y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
			if (board[nx][ny] > 0) continue;

			int dist = abs(nx - exit_loc.X) + abs(ny - exit_loc.Y);
			if (dist < min_dist) {
				min_dist = dist;
				select_loc = { nx,ny };
			}
		}

		if (select_loc.X != p.x || select_loc.Y != p.y) 
			total_move_dist++;

		p.x = select_loc.X;		p.y = select_loc.Y;

		if (p.x == exit_loc.X && p.y == exit_loc.Y) {
			p.Isout = true;
			out_cnt++;
		}


		if (p.Isout == false) {
			player_index_board[p.x][p.y].push(i);
		}
		player_v[i] = p;
	}
}

void find_min_square() {
	pair<int, int> start_loc;
	int min_length = 10;
	bool find_square = false;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			for (int dist = 2; dist <= N; dist++) {
				bool find_exit = false;
				bool find_player = false;

				if (i + dist > N || j + dist > N) break;

				for (int x = i; x < i + dist; x++) {
					for (int y = j; y < j + dist; y++) {
						if (x == exit_loc.X && y == exit_loc.Y)
							find_exit = true;
						if (player_index_board[x][y].size() > 0)
							find_player = true;
						
					}
				}

				if (find_exit == true && find_player == true) {
					if (dist < min_length) {
						start_loc = { i,j };
						min_length = dist;
						find_square = true;
					}
					break;
				}
			}
		}
	}

	if (find_square == true) {
		int rotate_board[10][10] = { 0, };
		queue<int> rotate_player_index_board[10][10];

		for (int x = 0; x < min_length; x++) {
			for (int y = 0; y < min_length; y++) {
				rotate_board[y + start_loc.X][((min_length - 1) - x) + start_loc.Y] = board[x + start_loc.X][y + start_loc.Y];
				
				if (player_index_board[x + start_loc.X][y + start_loc.Y].size() > 0) {
					int size = player_index_board[x + start_loc.X][y + start_loc.Y].size();
					for (int k = 0; k < size; k++) {
						int index = player_index_board[x + start_loc.X][y + start_loc.Y].front();
						player_index_board[x + start_loc.X][y + start_loc.Y].pop();
						rotate_player_index_board[y + start_loc.X][((min_length - 1) - x) + start_loc.Y].push(index);
					}
				}
			}
		}

		for (int x = start_loc.X; x < start_loc.X + min_length; x++) {
			for (int y = start_loc.Y; y < start_loc.Y + min_length; y++) {
				board[x][y] = rotate_board[x][y];

				if (board[x][y] > 0) board[x][y]--;
				if (board[x][y] == -1) exit_loc = { x,y };

				if (rotate_player_index_board[x][y].size() > 0) {
					int size = rotate_player_index_board[x][y].size();
					for (int k = 0; k < size; k++) {
						int index = rotate_player_index_board[x][y].front();
						player_v[index].x = x;	player_v[index].y = y;
						rotate_player_index_board[x][y].pop();
						player_index_board[x][y].push(index);
					}
				}
			}
		}
	}
}

void print_situation() {
	cout << "board\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << board[i][j] << " ";
		}
		cout << '\n';
	}
	cout << '\n';

	cout << "player_index_board\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << player_index_board[i][j].size() << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	for (int i = 0; i < K; i++) {
		//cout << "time : " << i + 1 << '\n';
		player_move();
		/*print_situation();
		int de = 1;*/
		if (out_cnt == M) break;
		find_min_square();
		/*print_situation();
		de = 2;*/
	}
	cout << total_move_dist << "\n";
	cout << exit_loc.X + 1 << " " << exit_loc.Y + 1;
}


int main(void) {
	Input();
	simulation();
	return 0;
}