/*
20:00
*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };

int n, m, k;

struct cmp {
	bool operator() (int left, int right) {
		if (left > right)
			return false;
		if (left < right)
			return true;
		return false;
	}
};

priority_queue<int, vector<int>, cmp> gun_board[20][20];

struct player {
	int x;
	int y;
	int d;
	int s;
	int gun_power = 0;
	int score = 0;
};

vector<player> player_v;
queue<int> player_index_board[20][20];


void Input() {
	cin >> n >> m >> k;

	int gun;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> gun;
			if (gun > 0) {
				gun_board[i][j].push(gun);
			}
		}
	}

	player p;
	for (int i = 0; i < m; i++) {
		cin >> p.x >> p.y >> p.d >> p.s;
		p.x--;	p.y--;
		player_v.push_back(p);
		player_index_board[p.x][p.y].push(i);
	}
}

void pick_up_gun(player& p) {
	if (p.gun_power != 0) {
		gun_board[p.x][p.y].push(p.gun_power);
		p.gun_power = 0;
	}
	if (gun_board[p.x][p.y].size() > 0) {
		int max_gun_power = gun_board[p.x][p.y].top();	gun_board[p.x][p.y].pop();
		p.gun_power = max_gun_power;
	}
}

void loser_process(player& loser_p) {

	if (loser_p.gun_power > 0) {
		gun_board[loser_p.x][loser_p.y].push(loser_p.gun_power);
		loser_p.gun_power = 0;
	}

	int nx = loser_p.x + dx[loser_p.d];
	int ny = loser_p.y + dy[loser_p.d];
	while ((nx < 0 || ny < 0 || nx >= n || ny >= n) || (player_index_board[nx][ny].size() > 0)) {
		loser_p.d = (loser_p.d + 1) % 4;
		nx = loser_p.x + dx[loser_p.d];
		ny = loser_p.y + dy[loser_p.d];
	}
	loser_p.x = nx;	loser_p.y = ny;

	pick_up_gun(loser_p);
}

void fight_player(player& p) {
	int p2_index = player_index_board[p.x][p.y].front();
	player_index_board[p.x][p.y].pop();

	player p2 = player_v[p2_index];

	if (p.s + p.gun_power > p2.s + p2.gun_power) {
		p.score += ((p.s + p.gun_power) - (p2.s + p2.gun_power));
		loser_process(p2);
		pick_up_gun(p);
	}
	else if (p.s + p.gun_power < p2.s + p2.gun_power) {
		p2.score += ((p2.s + p2.gun_power) - (p.s + p.gun_power));
		loser_process(p);
		pick_up_gun(p2);
	}
	else {
		if (p.s > p2.s) {
			p.score += ((p.s + p.gun_power) - (p2.s + p2.gun_power));
			loser_process(p2);
			pick_up_gun(p);
		}
		else if (p.s < p2.s) {
			p2.score += ((p2.s + p2.gun_power) - (p.s + p.gun_power));
			loser_process(p);
			pick_up_gun(p2);
		}
	}

	player_index_board[p2.x][p2.y].push(p2_index);
	player_v[p2_index] = p2;
}

void player_move(player& p) {

	// 1-1
	int nx = p.x + dx[p.d];
	int ny = p.y + dy[p.d];

	if (nx < 0 || ny < 0 || nx >= n || ny >= n) {
		p.d = (p.d + 2) % 4;
		nx = p.x + dx[p.d];
		ny = p.y + dy[p.d];
	}

	p.x = nx;	p.y = ny;

	//2-1
	if (player_index_board[p.x][p.y].size() == 0) {
		pick_up_gun(p);
	}
	else {
		fight_player(p);
	}
}

void situation() {
	cout << "player_index_board\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << player_index_board[i][j].size() << " ";
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	//situation();
	for (int turn = 0; turn < k; turn++) {
		for (int i = 0; i < player_v.size(); i++) {
			player p = player_v[i];
			player_index_board[p.x][p.y].pop();
			player_move(p);
			player_index_board[p.x][p.y].push(i);
			player_v[i] = p;
		}
		//situation();
	}

	for (int i = 0; i < m; i++) {
		cout << player_v[i].score << " ";
	}
}

int main(void) {
	Input();
	simulation();
	return 0;
}

/*
1-1ù ��° �÷��̾���� ���������� ������ ���ϰ� �ִ� ������ 1ĭ �̵�,
���� �ش� �������� ���� �� ���ڸ� ����� ��� ���ݴ� �������� ������ �ٲ㼭 1��ŭ �̵�


2-1 ���� �̵��� ���⿡ �÷��̾ ���ٸ� �ش� ĭ�� ���� �ִ��� Ȯ��, ���� �ִ� ���, �ش� �÷��̾��
���� ȹ��. �÷��̾ ���� �̹� ������ �ִ� ��쿡�� �����ִ� �ѵ�� �÷��̾ ������ �ִ� �� ���
���ݷ��� �� �� ���� ȹ���ϰ�, ������ �ѵ��� �ش� ���ڿ� �Ӵϴ�.

2-2-1 ���� �̵��� ���⿡ �÷��̾ �ִ� ��� �� �÷��̾ �ο�� ��. �ش� �÷��̾��� �ʱ� �ɷ�ġ�� ������
�ִ� ���� ���ݷ��� ���� ���Ͽ� �� ū �÷��̾ �̱�� ��. ���� �� ��ġ�� ���� ��� �÷��̾���
�ʱ� �ɷ�ġ�� ���� �÷��̾ �¸�. �̱� �÷��̾�� �� �÷��̾��� �ʱ� �ɷ�ġ�� ���� ���� ���ݷ��� ����
���̸�ŭ ����Ʈ�� ȹ��

2-2-2 �� �÷��̾�� ������ ������ �ִ� ���� �ش� ���ڿ� �������� �ش� �÷��̾ ���� ������ �ִ�
������ �� ĭ �̵�. ���� �̵��Ϸ��� ĭ�� �ٸ� �÷��̾ �ְų� ���� ���� ���� ��� ����������
90���� ȸ���Ͽ� ��ĭ�� ���̴� ���� �̵�. ���� �ش�ĭ�� ���� �ִٸ�, �ش� �÷��̾�� ���� ���ݷ��� ���� ���� ȹ��
�ϰ� ������ �ѵ��� �ش� ���ڿ� ��������.

2-2-3 �̱� �÷��̾�� �¸��� ĭ�� ������ �ִ� �ѵ�� ���� ��� �ִ� �� �� ���� ���ݷ��� ���� ���� ȹ���ϰ�,
������ �ѵ��� �ش� ���ڿ� ���� ����.

*/