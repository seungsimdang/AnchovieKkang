#include <bits/stdc++.h>
#define xx first
#define yy second
using namespace std;
using pii = pair<int, int>;
using dpi = pair<pii, pii>;

int N, M;
int m[200][200];             //맵
bool vi[200][200][1 << 16];  //위치 + 청소할 칸의 정보를 bit masking으로 표현
int dy[4] = {-1, 1, 0, 0};
int dx[4] = {0, 0, -1, 1};

int bfs(int sty, int stx, int K) {
    // cout << "ck " << (1 << K) - 1;
    memset(vi, 0, sizeof(vi));     //배열을 0으로 초기화
    queue<dpi> q;                  //큐 선언
    q.push({{0, 0}, {sty, stx}});  //queue에 0,0과 로봇의 위치 삽입
    vi[sty][stx][0] = false;       //로봇의 위치는 가지 않았다고 설정

    while (!q.empty()) {
        int cd = q.front().xx.xx;  //queue의 첫번째, 이동한 횟수
        int ck = q.front().xx.yy;  //queue의 두번째, 확인된 청소할 좌표 수
        int cy = q.front().yy.xx;  //queue의 세번째, 현재 y좌표
        int cx = q.front().yy.yy;  //queue의 네번째, 현재 x좌표
        q.pop();
        //총 4개라면 10000-1 => 1111
        //cout << "ck " << ck << " " << ((1 << K) - 1) << " " << (1 << 16) << endl;
        //모든 청소할 위치를 지났으면 ck의 상태는 1이 K개 반복된 2진수 형태
        //ex) 청소할 좌표수가 4개 -> 1111
        if (ck == (1 << K) - 1) {
            return cd;  //몇 번 이동했는지 return
        }
        for (int i = 0; i < 4; i++) {  // 4방향 이동
            int nd = cd + 1;           //이동한 횟수 1증가
            int nk = ck;               //다시 nk를 복구 -> 4방향 보기 위해서
            int ny = cy + dy[i];       //다음 이동할 위치
            int nx = cx + dx[i];
            //못 가는 곳이라면 continue
            if (ny < 0 || nx < 0 || ny >= N || nx >= M || m[ny][nx] == -1) continue;
            //다음 확인할 위치가 청소되었다면 or 연산을 통해 청소된 위치를 표시
            //ex)지금까지 1번째, 2번째 청소된 위치를 확인하고 3번째를 찾았다면
            //0011 | (1 << 3-1) = 0011 | (1 << 2) = 0011 | 0100 => 0111
            //즉, 0번째를 제외한 1, 2, 3 번째 청소된 위치를 확인
            if (m[ny][nx] > 0) nk |= (1 << (m[ny][nx] - 1));
            if (vi[ny][nx][nk]) continue;  //이미 간 곳이라면 continue
            q.push({{nd, nk}, {ny, nx}});  //queue 에 push
            // cout << "nk " << nk << " " << nx << " " << ny << endl;
            vi[ny][nx][nk] = true;  //간 곳이라고 표시해줌
        }
    }
    return N * M - 1;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int tmp = 0;
    int cnt = 0;
    while (1) {
        cnt = 0;
        cin >> N >> M;  //행, 열 수
        if (!N) break;
        memset(m, -1, sizeof(m));  //m 배열 초기화
        int sty, stx, K = 0;       //로봇 위치, 청소된 위치 수
        string str;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                m[i][j] = tmp = rand() % 2;  //랜덤 생성
                /* 청소된 좌표 갯수 리미트
                /if (tmp > 0) cnt++;
                if (cnt > 20) {
                    m[i][j] = 0;
                    tmp = 0;
                }*/
                cout << m[i][j] << " ";                 //방 상태 출력
                if ((tmp > 0) && (i != 0 || j != 0)) {   //시작위치가 아니고 청소되었다면
                    m[i][j] = ++K;                     //몇 번째로 확인했는지 입력
                }
            }
            cout << endl;
        }
        sty = 0, stx = 0, m[0][0] = 0;  //로봇 위치 설정
        cout << "출력 " << endl;
        cout << bfs(sty, stx, K) << endl;
    }
}