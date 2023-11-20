#include <ctime>
#include <iostream>
#define MAXINT 101
using namespace std;

int N;
struct point {
    int x, y;     // x, y 값
    float angle;  //수평각
    char c;
    point(int x = 0, int y = 0, char c = ' ') {
        this->x = x;
        this->y = y;
        this->c = c;
    }
};

struct line {
    struct point p1, p2;
};
void Swap(struct point* a, struct point* b) {  //값 바꾸기
    struct point t;
    t = *a;
    *a = *b;
    *b = t;
}
float ComputeAngle(struct point p1, struct point p2) {  //수평각 계산
    int dx, dy, ax, ay;                                 // x길이 ,y길이, x길이 절댓값, y길이 절댓값
    float t;                                            //각
    dx = p2.x - p1.x;                                   // x길이
    ax = abs(dx);                                       //절댓값
    dy = p2.y - p1.y;                                   // y길이
    ay = abs(dy);                                       //절댓값

    t = (ax + ay == 0) ? 0 : (float)dy / (ax + ay);  // 1사분면 or 같은 위치
    if (dx < 0)
        t = 2 - t;    // 2사분면
    else if (dy <= 0)  // 4사분면
        t = 4 + t;

    return t * 90.0;  //각 리턴
}


int partition(struct point a[], int l, int r) {  //퀵정렬
    int i, j;                                    //교환 할 수 찾고 바꾸기
    int v;                                       //키 값
    if (r > l) {
        v = a[l].x;  //제일 왼쪽 값을 키 값으로
        i = l;       // i의 초기값을 제일 왼쪽 인덱스로 설정
        j = r + 1;   // j의 초기값을 제일 오른쪽 인덱스에서 한칸 옆으로 설정
        for (;;) {   //값이 범위를 벗어나지 않도록 함
            while (i + 1 < N && a[++i].x < v)
                ;  //키 값보다 큰 값을 만날 때까지 i값 증가
            while (a[--j].x > v)
                ;                //키 값보다 작은 값을 만날 때까지 i값 증가
            if (i >= j)          //만약 i가 j보다 커지면 역전했으므로
                break;           //나간다
            Swap(&a[i], &a[j]);  // a[i]와 a[j]값을 바꿔준다
        }
    }
    Swap(&a[j], &a[l]);  //키값과 a[j]값을 바꿔준다.
    return j;            //다음 퀵정렬 기준이 되는 인덱스 리턴
}

void quicksort(struct point a[], int l, int r) {  //퀵정렬 x좌표 기준으로 정렬
    int j;
    if (r > l)  //아직 오른쪽 인덱스가 왼쪽 인덱스 보다 오른쪽에 있다면
    {
        j = partition(a, l, r);  //교환할 수 찾기
        quicksort(a, l, j - 1);  //구간 나눠서 퀵정렬 실행
        quicksort(a, j + 1, r);
    }
}

void printMap(int** map, int row, int col) {
    for (int j = 0; j < row; j++) {
        for (int k = 0; k < col; k++) {
            cout << map[j][k] << '\t';
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    int miny = 100, checki;
    int x, y = 0, result, row, col, count = 1;
    char c;
    srand(time(NULL));
    cout << "방의 크기 입력 : ";
    cin >> row >> col;
    int** map = new int* [row];
    for (int i = 0; i < row; i++) {
        map[i] = new int[col];
    }

    N = col * row;
    struct point* polygon = new struct point[N + 2], z;
    checki = 0;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            map[i][j] = rand() % 2;
            cout << map[i][j] << "\t";

            if (map[i][j] && !(i == 0 && j == 0)) {
                if (miny > j) {
                    miny = y;
                    checki = count;
                }
                else if (j == miny && polygon[checki].x > i)  // y값이 같다면 x값이 가장 작은 점을 찾는다
                    checki = count;
                polygon[count++] = point(i, j, 'A');
            }
        }
        cout << '\n';
    }

    for (int i = 1; i < count; i++) {  //각각의 수평각 계산
        polygon[i].angle = ComputeAngle(polygon[checki], polygon[i]);
    }

    quicksort(polygon, 0, count - 1);  //수평각을 기준으로 정렬

    cout << endl;
    for (int i = 0; i < count; i++) {  //다각형 순서대로 출력
        
        map[polygon[i].x][polygon[i].y] = -1;
        printMap(map, row, col);
        map[polygon[i].x][polygon[i].y] = 0;
    }
    cout << "Solution Path";

    for (int i = 0; i < count; i++) {
        cout << " - (" << polygon[i].x << ", " << polygon[i].y << ")";
    }
}