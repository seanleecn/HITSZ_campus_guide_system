#include "./include/guide_system.h"

guideSystem::guideSystem(string &filename)
{
    int Count = 0; // 建筑点的个数
    int idx = 0;   // 文件行数

    ifstream campusMap;
    campusMap.open(filename, ios::in);
    if (!campusMap.is_open())
    {
        cerr << "地图信息文件打开失败，请检查根目录下的" << filename << "文件" << endl;
        _isOpen = false;
    }
    _isOpen = true;
    string mapLine;
    int tmp1, tmp2, tmpDist;
    while (getline(campusMap, mapLine))
    {
        idx++;
        if (idx > 1 && idx < 19)
        {                                  // 2-18行是建筑点信息
            stringstream mapWord(mapLine); // 将mapLine按照空格分成mapWord，赋给相应的成员
            mapWord >> _builds[idx - 2]._num >> _builds[idx - 2]._name >> _builds[idx - 2]._info >> _builds[idx - 2]._coordx >> _builds[idx - 2]._coordy;
            Count++;
        }
        if (idx > 19)
        { // 19行以后存储的是边的信息
            stringstream mapWord(mapLine);
            mapWord >> tmp1 >> tmp2 >> tmpDist;
            _matrix[tmp1 - 1][tmp2 - 1] = _matrix[tmp2 - 1][tmp1 - 1] = tmpDist;
        }
    }
    _buildNum = Count; // 建筑点个数
    // 邻接矩阵中自身到自身权重为0，不可达的边为INF
    for (int i = 0; i < _buildNum; i++)
    {
        for (int j = 0; j < _buildNum; j++)
        {
            if (i == j)
                _matrix[i][j] = 0;
            else if (_matrix[i][j] == 0)
                _matrix[i][j] = INF;
        }
    }

    // 进入交互界面
    interface();
}

void guideSystem::printMenu()
{
    cout << "****************哈工大(深圳)校园导游咨询系统****************\n"
         << "                1.哈深建筑点信息查询\n"
         << "                2.多个建筑点最短路径\n"
         << "                3.指定建筑点到任意建筑点问路查询\n"
         << "                0.退出系统\n"
         << "************************************************************" << endl;
}

void guideSystem::printMap()
{
    cout << "*********************哈工大(深圳)校园景点地图*************************" << endl;
    cout << "   ◎(1)E栋--◎(2)D栋--◎(3)F栋--◎(4)C栋" << endl;
    cout << "      /       |                  \\" << endl;
    cout << "     /      ◎(5)球场         ◎(6)A栋--◎(7)B栋" << endl;
    cout << "    /         |             /            \\" << endl;
    cout << "  ◎(8)操场    |           /            ◎(9)G栋" << endl;
    cout << "          \\   |          /            /    \\" << endl;
    cout << "           ◎(10)食堂------------------     ◎(11)J栋" << endl;
    cout << "                    \\                          \\" << endl;
    cout << "                     \\             ◎(12)T3栋--◎(13)H栋" << endl;
    cout << "                      \\               |        |       \\" << endl;
    cout << "                      ◎(14)宿舍--◎(15)T4栋--◎(16)T5栋--◎(17)T6栋" << endl;
    cout << "*********************************************************************" << endl;
}

void guideSystem::searchInfo() const
{
    int choice;
    while (true)
    {
        cout << "请输入您想了解的建筑编号,输入0退出" << endl;
        cin >> choice;
        if (choice < 0 || choice > _buildNum)
        {
            cout << "输入的有误，请重新输入" << endl;
            continue;
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            cout << "您查询的是: " << _builds[choice - 1]._name << " 它是: " << _builds[choice - 1]._info
                 << " 坐标: (" << _builds[choice - 1]._coordx << "," << _builds[choice - 1]._coordy << ")" << endl;
        }
    }
}

void guideSystem::pathInit()
{
    _pStack._top = 0;
    _pStack._length = 0;
    _pNum = 0;
    for (int i = 0; i < MAX; i++)
    {
        _pStack._stack[i] = 0;
        _pStack._visited[i] = 0;
    }
    for (int i = 0; i < MAX; i++)
    {
        _pArray[i]._top = 0;
        _pArray[i]._length = 0;
        for (int j = 0; j < MAX; j++)
        {
            _pArray[i]._stack[j] = 0;
            _pArray[i]._visited[j] = 0;
        }
    }
}

void guideSystem::DFSAllPath(int start, int end, int n)
{
    _pStack._stack[_pStack._top] = start;
    _pStack._top++;
    _pStack._visited[start] = 1;
    _pStack._length = 0;
    for (int i = 0; i < _buildNum; i++)
    {
        if (!_pStack._visited[i] && _matrix[start][i] != INF && _matrix[start][i] > 0)
        {
            if (i == end)
            { // 到达目标点
                // 统计路径长度
                for (int j = 0; j < _pStack._top - 1; j++)
                {
                    _pStack._length += _matrix[_pStack._stack[j]][_pStack._stack[j + 1]];
                }
                _pStack._length += _matrix[_pStack._stack[_pStack._top - 1]][end];
                if (n == _pStack._top + 1)
                { // 满足路径长度
                    // 将满足n的路径保存到_pArray
                    for (int k = 0; k < _pStack._top; k++)
                    {
                        _pArray[_pNum]._stack[k] = _pStack._stack[k];
                    }
                    _pArray[_pNum]._stack[_pStack._top] = end;
                    _pArray[_pNum]._top = _pStack._top;
                    _pArray[_pNum]._length = _pStack._length;
                    _pNum++; // 路径数+1
                }
            }
            else
            {
                DFSAllPath(i, end, n);
                _pStack._top--;
                _pStack._visited[i] = 0;
            }
        }
    }
}

void guideSystem::printAngle(int start, int end)
{
    float angle =
        atan2(_builds[end]._coordy - _builds[start]._coordy, _builds[end]._coordx - _builds[start]._coordx) * 180 / 3.14159265;
    cout << _builds[end]._name << "在" << _builds[start]._name << "的"
         << angle << "度方向,距离" << _matrix[start][end] << endl;
}

void guideSystem::printShortPath()
{
    int start, end, n, choice;
    while (true)
    {
        cout << "请输入起点编号(1-17)" << endl;
        cin >> start;
        if (start < 1 || start > 17)
        {
            cout << "输入编号有误，请重新输入" << endl;
            continue;
        }
        cout << "请输入终点编号(1-17)" << endl;
        cin >> end;
        if (end < 1 || end > 17)
        {
            cout << "输入编号有误，请重新输入" << endl;
            continue;
        }
        cout << "请输入路径上的建筑数(2-17)" << endl;
        cin >> n;
        if (n < 2 || n > 17)
        {
            cout << "路径上的建筑数错误" << endl;
            continue;
        }
        start -= 1;
        end -= 1;
        pathInit();                // 路径栈初始化
        DFSAllPath(start, end, n); // 找到全部满足长度为n的路径
        if (_pNum == 0)
        {
            cout << "没有满足您要求的路径,重新输入请按1,退出请按0" << endl;
            cin >> choice;
            if (choice == 0)
                break;
            else if (choice == 1)
                continue;
        }
        // 找到_pArray中最短的一条
        pathStack shortPath;
        shortPath._length = _pArray[0]._length;
        shortPath._top = _pArray[0]._top;
        for (int i = 0; i < MAX; i++)
        {
            shortPath._stack[i] = _pArray[0]._stack[i];
        }
        for (int i = 0; i < _pNum; i++)
        {
            if (_pArray[i]._length < shortPath._length)
            {
                shortPath._length = _pArray[i]._length;
                shortPath._top = _pArray[i]._top;
                for (int j = 0; j < MAX; j++)
                {
                    shortPath._stack[j] = _pArray[i]._stack[j];
                }
            }
        }
        // 打印输出最短路径
        cout << _builds[start]._name << "到" << _builds[end]._name << ",经过" << n << "个建筑的最短路径为: ";
        for (int i = 0; i < shortPath._top; i++)
        {
            cout << _builds[shortPath._stack[i]]._name << "-->";
        }
        cout << _builds[end]._name << endl;
        for (int i = 0; i < shortPath._top; i++)
        {
            cout << i + 1 << ". ";
            printAngle(shortPath._stack[i], shortPath._stack[i + 1]);
        }
        cout << "该最短路径长度:" << shortPath._length << endl;
        cout << "继续查询别的路径请按1,退出请按0" << endl;
        cin >> choice;
        if (choice == 0)
            break;
    }
}

void guideSystem::Floyd()
{
    int tmp;
    // 初始化_dist和_path矩阵
    for (int i = 0; i < _buildNum; i++)
    {
        for (int j = 0; j < _buildNum; j++)
        {
            _distMat[i][j] = _matrix[i][j];
            _pathMat[i][j] = j;
        }
    }
    // 三重循环更新_dist和_path矩阵
    for (int k = 0; k < _buildNum; k++)
    {
        for (int i = 0; i < _buildNum; i++)
        {
            for (int j = 0; j < _buildNum; j++)
            {
                tmp = (_distMat[i][k] == INF || _distMat[k][j] == INF) ? INF : (_distMat[i][k] + _distMat[k][j]); // 防止int越界
                if (_distMat[i][j] > tmp)
                {
                    _distMat[i][j] = tmp;
                    _pathMat[i][j] = _pathMat[i][k];
                }
            }
        }
    }
}

void guideSystem::quickSortFloyd(endPoint *array, int l, int r)
{
    if (l < r)
    {
        int i = l;
        int j = r;
        endPoint pivot = array[i];
        while (i < j)
        {
            while (i < j && array[j]._dist > pivot._dist)
                j--;
            if (i < j)
                array[i++] = array[j];
            while (i < j && array[i]._dist < pivot._dist)
                i++;
            if (i < j)
                array[j--] = array[i];
        }
        array[i] = pivot;
        quickSortFloyd(array, l, i - 1);
        quickSortFloyd(array, i + 1, r);
    }
}

void guideSystem::printFloydPath()
{
    int start;
    while (true)
    {
        cout << "请输入起点建筑的序号,输入0退出" << endl;
        cin >> start;
        if (start < 0 || start > 17)
        {
            cout << "你输入的建筑物编号有误，请重新输入" << endl;
            continue;
        }
        if (start == 0)
            break;
        else
        {
            endPoint endArray[_buildNum];
            for (int i = 0; i < _buildNum; i++)
            {
                endArray[i]._index = i;
                endArray[i]._dist = _distMat[start - 1][i];
            }
            // 对路径排序
            quickSortFloyd(endArray, 0, _buildNum - 1);
            cout << "从" << _builds[start - 1]._name << "到其他建筑点的最短路径由小到大排序如下:" << endl;
            for (int i = 1; i < _buildNum; i++)
            {
                cout << setw(2) << i << ": " << _builds[start - 1]._name << "-->"
                     << _builds[endArray[i]._index]._name
                     << " 最短路径长度 " << endArray[i]._dist << " 路径详细: ";
                int k = _pathMat[start - 1][endArray[i]._index];
                cout << _builds[start - 1]._name; // 打印起点
                while (k != endArray[i]._index)
                {
                    cout << "-->" << _builds[k]._name;
                    k = _pathMat[k][endArray[i]._index];
                }
                cout << "-->" << _builds[endArray[i]._index]._name << endl; // 打印终点
            }
        }
    }
}

void guideSystem::interface()
{
    int choice;
    while (_isOpen)
    {
        printMenu();
        cout << "请输入需要操作的命令: " << endl;
        cin >> choice;
        switch (choice)
        {
        case 1: // 建筑点相关信息查询
            printMap();
            searchInfo();
            break;
        case 2: // n个节点最短路径
            // DFS找全部路径，选出满足n的路径，输出最短路径
            printMap();
            printShortPath();
            break;
        case 3: // 从指定点到任意点的最短路径并排序
            printMap();
            Floyd();          // Floyd找到全部最短路径
            printFloydPath(); // 从小到大输出start到其他建筑点的最短路径
            break;
        case 0:
            cout << "欢迎下次使用,我是李翔,学号19S053032 :)" << endl;
            _isOpen = false;
            break;
        default:
            cerr << "输入有误,请重新输入" << endl;
            break;
        }
    }
}

int main()
{
    string map_filename = "../mapfile/hitszmap.txt"; // 存储地图数据的文件名
    guideSystem *campus = new guideSystem(map_filename);
    delete campus;
}