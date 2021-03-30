#ifndef GUIDE_SYSTEM_H
#define GUIDE_SYSTEM_H
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#define INF 0x7fffffff
#define MAX 20

using namespace std;

// 地图类
class guideSystem
{
private:
    // 建筑点结构体
    struct building
    {
        int _num;     // 编号
        string _name; // 名称
        string _info; // 介绍
        int _coordx;  // x坐标
        int _coordy;  // y坐标
    };

    // 路径栈(DFS中使用)
    struct pathStack
    {
        int _stack[MAX];   // 路径
        int _top;          // 栈顶
        int _visited[MAX]; // 访问标签
        int _length;       // 路径长度
    };

    // 终点结构体(Floyd中使用)
    struct endPoint
    {
        int _index; // 终点序号
        int _dist;  // 到该终点的路径
    };
    int _buildNum;         // 建筑点数
    building _builds[MAX]; // 建筑点数组
    int _matrix[MAX][MAX]; // 邻接矩阵表示地图
    bool _isOpen;          // 地图文件打开成功标志

    pathStack _pStack;      // DFS路径栈
    pathStack _pArray[MAX]; // 满足长度为n的路径数组
    int _pNum;              // 满足长度为n的路径数目

    int _pathMat[MAX][MAX]; // FLoyd路径
    int _distMat[MAX][MAX]; // Floyd距离

    // 查询建筑点信息
    void searchInfo() const;

    // 打印功能菜单
    static void printMenu();

    // 打印地图
    static void printMap();

    // 初始化_pStack _pArray _pNum
    void pathInit();

    // DFS得到start到end中所有长度为n的路径
    void DFSAllPath(int start, int end, int n);

    // 打印两点间的角度和长度
    void printAngle(int start, int end);

    // 打印满足长度为n的最短路径
    void printShortPath();

    // Floyd算法得到任意点之间的最短路径
    void Floyd();

    // 对Floyd得到的所有最短路径快速排序
    void quickSortFloyd(endPoint *array, int l, int r);

    // 按路径长短打印全部最短路径
    void printFloydPath();

    // 交互界面
    void interface();

public:
    // 根据地图文件构建邻接矩阵，并进入交互界面
    guideSystem(string &map_filename);
};

#endif