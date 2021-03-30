# 哈尔滨工业大学（深圳）校园导游系统

数据结构与算法分析(C++)的课程作业。

<img src="pic\image-20210330170638020.png" alt="校园建筑点图结构" style="zoom: 80%;" />

## 实现的功能：

1. 景点信息查询功能
2. 输入起点、终点、途径的景点个数；输出满足上述条件的全部路径中最短的一条
3. 输入起点；输出从该点到其他点的最短路径，并按照路径长度排序

## 使用的方法

1. 邻接矩阵存储无向图
2. DFS遍历图
3. Floyd算法找到全部路径点+快速排序对路径进行排序