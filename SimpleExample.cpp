// A Satr ( A* Algorithm)
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
// 0、设置节点结构体
struct Node
{
    Node(): Row(0), Col(0), Parent(nullptr), F(0), H(0), G(0) {};
    Node(int Row_, int Col_): Row(Row_), Col(Col_), Parent(nullptr), F(0), H(0), G(0) {};
    int Row;                   // 表示当前点在地图的位置 Row
    int Col;                   // 表示当前点在地图的位置 Col
    Node* Parent;              // 表示父节点
    double F;
    double H;
    double G;
};
// 1、生成地图 (采用双Vector)
std::vector<std::vector<int>> Create_Map() {
    return {{0, 1, 0, 1, 0, 0, 0, 0},
            {0, 1, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0},};
}
// 2、根据地图生成参考地图, 除起始点外全为0 (用于存储某个栅格是否被遍历过)
std::vector<std::vector<int>> Create_RefMap(int Row, int Col, Node* Start) {
    std::vector<std::vector<int>> ans(Row, std::vector<int>(Col));
    ans[Start->Row][Start->Col] = 1;
    return ans;
}

// 3、计算节点的G值
double Calculate_G(Node* Cur, Node* Goal)
{
    double DiffRow = (Cur->Row > Goal->Row) ? Cur->Row - Goal->Row : Goal->Row - Cur->Row;
    double DiffCol = (Cur->Col > Goal->Col) ? Cur->Col - Goal->Col : Goal->Col - Cur->Col;
    return DiffRow + DiffCol;
}

int main ()
{
    // 1、获取地图
    std::vector<std::vector<int>> Map = Create_Map();
    // 2、给出起点和终点
    Node* Start = new Node(0, 0);
    Node* End   = new Node(0, 7);
    // 3、获取Ref地图
    std::vector<std::vector<int>> RefMap = Create_RefMap(static_cast<int>(Map.size()), static_cast<int>(Map[0].size()), Start);
    // 4、创建一个queue存储已经遍历的节点
    std::vector<Node*> Q;
    Q.push_back(Start);
    // 5、设置搜索方向 8 个方向 和 相应 H 值
    std::vector<std::vector<int>> Direction = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    std::vector<double> H_Value = {1, 1, 1, 1, 1.414, 1.414, 1.414, 1.414};
    // 4、开始搜素
    bool ans = false; Node* CurNode = Start;
    while (!Q.empty()) {
        // 0、获取当前节点
        auto PosMin = Q.begin();
        for (auto Pos = Q.begin(); Pos != Q.end(); ++Pos) {
            PosMin = ((*Pos)->F < (*PosMin)->F) ? Pos : PosMin;
        }
        CurNode = *PosMin;
        Q.erase(PosMin);
        if (CurNode->Row == End->Row && CurNode->Col == End->Col) {
            ans = true;
            break;
        }
        // 1、寻找周围节点，将可行的周围节点推入Q容器中
        for (int i = 0; i < Direction.size(); ++i) {
            int NextRow = CurNode->Row + Direction[i][0];
            int NextCol = CurNode->Col + Direction[i][1];
            if (NextRow < 0 || NextRow >= Map.size() || NextCol < 0 || NextCol >= Map[0].size() || RefMap[NextRow][NextCol] == 1 || Map[NextRow][NextCol] == 1)
                continue;
            Node* NextNode = new Node(NextRow, NextCol);
            NextNode->Parent = CurNode;
            NextNode->H = CurNode->H + H_Value[i];
            NextNode->G = Calculate_G(NextNode,   End);
            NextNode->F = NextNode->H + NextNode->G;
            // 推入总容器中
            Q.push_back(NextNode);
            // 标记已经走过
            RefMap[NextNode->Row][NextNode->Col] = 1;
        }
    }
    // 5、打印信息
    if (ans) {
        std::cout << "[已搜索出路径]: \n";
        while (CurNode) {
            Map[CurNode->Row][CurNode->Col] = 2;
            CurNode = CurNode->Parent;
        }
        Map[Start->Row][Start->Col] = -1;
        Map[End->Row][End->Col] = -2;
        for (int i = 0; i < Map.size(); ++i) {
            for (int j = 0; j < Map[0].size(); ++j) {
                if (Map[i][j] == 1)
                    std::cout << "O";
                else if (Map[i][j] == 0)
                    std::cout << "-";
                else if (Map[i][j] == 2)
                    std::cout << "#";
                else if (Map[i][j] == -1)
                    std::cout << "@";
                else if (Map[i][j] == -2)
                    std::cout << "*";
            }
            std::cout << "\n";
        }
    }
    else
        std::cout << "[寻找路径失败]";
    return 0;
}
