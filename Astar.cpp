#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
int N, M;               //방의 크기 N, M, 전역변수로 선언

struct Node {
    Node* parent;       //부모 노드
    int** state;          //노드 상태
    int x, y;             //노드 좌표
    int g, h;            //노드의 함수
    string moves;       //노드의 방향

    Node() {
        moves = "";
        state = new int* [N];
        for (int i = 0; i < N; i++)
            state[i] = new int[M];
    }

};

//새로운 노드 생성 후 반환
Node* newNode(int** state, int x, int y, int dx, int dy, int g, Node* parent) {
    Node* node = new Node;
    node->parent = parent;

    for (int i = 0; i < N; i++) //새로운 노드에 기존 노드를 복사해준다.
        for (int j = 0; j < M; j++)
            node->state[i][j] = state[i][j];

    node->state[x][y] = 0;     //청소한 구역을 청소상태 0으로 만들어준다.
    node->state[dx][dy] = -1;  //청소기가 이동한 곳에 -1(청소기)를 넣어준다.

    node->h = INT_MAX;
    node->g = g;

    node->x = dx;
    node->y = dy;
    return node;
}

//U, L, D, R
int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, -1, 0, 1 };

//최적의 이동경로 저장
string solutionPath = "";

//heuristic 함수 : heuristic은 state(map)에서 청소되지 않은 구역 
//즉, 청소해야 할 구역의 개수를 세어 주었다.
int heuristic(int** state) {
    int h = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (state[i][j] > 0)
                h++;
    return h;
}

//이동 가능 여부 판단 : map의 크기를 넘어서는 경우를 제외시켰다.
bool isValid(int x, int y) {
    return (x >= 0 && x < N&& y >= 0 && y < M);
}

//함수 계산 및 비교 : compare 함수는 node의 평가값을 비교하였다.
struct compare {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return (lhs->h + lhs->g) > (rhs->h + rhs->g);
    }
};

//ASTAR
Node* ASTAR(int** startState, int x, int y) {
    //함수 평가값이 최소인 노드를 찾기 위해 우선순위 queue를 사용하여
    //평가값이 가장 작은 값은 우선시하였다.
    priority_queue<Node*, vector<Node*>, compare> open;

    Node* state = newNode(startState, x, y, x, y, 0, NULL);
    open.push(state);

    string move = "ULDR"; //이동 좌표 인덱스 활용

    while (!open.empty()) {
        Node* currentNode = open.top(); //평가값이 가장 작은 값을 가져온다.
        open.pop();

        if (currentNode->h == 0) //heuristic이 0인 경우 목표 노드 도달하고, 
            return currentNode;

        //노드 expand, 현재 노드에서 위, 왼쪽, 아래, 오른쪽 순으로 평가값을 계산하여 우선순위 queue에 삽입한다.
        for (int i = 0; i < 4; i++) {
            if (!isValid(currentNode->x + dx[i], currentNode->y + dy[i])) continue;

            Node* childNode = newNode( //현재 노드에서 이동할 노드(childNode)를 검사한다.
                currentNode->state,
                currentNode->x,
                currentNode->y,
                currentNode->x + dx[i],
                currentNode->y + dy[i],
                currentNode->g + 1,
                currentNode);

            childNode->h = heuristic(childNode->state); //이동할 노드의 heuristic을 계산.
            childNode->moves = move[i]; //이동 방향을 할당
            open.push(childNode); //우선순위 queue에 push
        }
    }
    return NULL;
}

//노드 state 출력
void printState(int** state) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            cout << state[i][j] << '\t';
        cout << '\n';
    }
}

//목표 노드에 도착하기까지의 과정 출력
void printSolutionPath(Node* node) {
    if (node == NULL) return;
    printSolutionPath(node->parent);
    solutionPath += node->moves + " - ";
    printState(node->state);
    cout << "\n";
}

int main() {
    srand(time(NULL));

    cout << "방의 크기 입력 : ";
    cin >> N >> M;

    //동적 할당
    int** startState = new int* [N];
    for (int i = 0; i < N; i++)
        startState[i] = new int[M];

    //랜덤으로 방의 상태를 입력한다.
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            startState[i][j] = rand() % 2;

    Node* goalNode = ASTAR(startState, 0, 0); //시작 state, 시작 x좌표, y좌표 

    if (goalNode == NULL) cout << "A* Failed \n";
    else {
        printSolutionPath(goalNode);
        cout << "Solution Path" << solutionPath << "Finish\n";
    }
}