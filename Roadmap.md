# The way to 1.0 #

## Milestone 1 ##
  * ~~重写旧版本rev37中的所有类，凡是涉及不合理的，进行重构。所有重写的代码放至namespace openclas下。~~  (**Done**)

| **namespace ictclas** | **namespace openclas** | **status** |
|:----------------------|:-----------------------|:-----------|
| `Edge<T>`             | `Edge<EdgeValueType>`  | **Done**   |
| `TagEdge<T>`          | `Word`                 | **Done**   |
| `AdjacencyList<T, E>` | `boost::adjacency_list<>` | **Done**   |
| `WordEntry`           | `DiskEntry`            | **Done**   |
| `WordTag`             | `TagItem`              | **Done**   |
| `Dictionary`          | `Dictionary`           | **Done**   |
| `Encoding`            | `"utility.h"`          | **Done**   |
| `WordIdPair`          | -                      | -          |
| `Atom`                | `Word`                 | **Done**   |
| `Segment<T>`          | `Segment`              | **Done**   |
| `KShortestPath<T, G>` | `"kshortestpath.h"`    | **Done**   |
| `Serializer`          | `"serialization.h"`    | **Done**   |
| `ICT_Serializer`      | `"serialization.h"`    | **Done**   |
| `OpenClasSerializer`  | `"serialization.h"`    | **Done**   |
| `Utility`             | `"utility.h"`          | **Done**   |

  * ~~重新组织目录结构，使之更接近标准项目~~  (**Done**)
  * ~~对已经实现的所有功能，添加test cases，确保Milestone 1的程序正常工作。~~  (**Done**)

## Milestone 2 ##

  * 继续未完成的任务，将ICTCLAS剩下的内容完整实现出来。

| **Class** | **Status** |
|:----------|:-----------|
| `ContextStat` | **Done**   |
| `Dictionary` | **Done**   |
| `DynamicArray` | **Done**   |
| `NShortestPath` | **Done**   |
| `Queue`   | -          |
| `Result`  | **Done**   |
| `SegGraph` | **Done**   |
| `Segment` | **Done**   |
| `Span`    | Waiting    |
| `UnknowWord` | Waiting    |
| `Utility` | **Done**   |

  * 对已经实现的所有功能，添加test cases，确保Milestone 2的程序正常工作。

## Milestone 3 ##

  * 制作使用的用户界面
    1. 命令行工具
    1. 图形界面工具
    1. 动态链接库(Windows, Linux)

  * 制作词典工具。包括
    1. 词典编辑工具
    1. 词典导入工具(源自wikipedia,中文词典等)
    1. 词典导入、生成工具（源自语料库）

  * 实现其它语言版本
    1. C# 版本
    1. Java 版本
    1. Python 版本

# 1.x #

  * 改进算法
  * ~~看需求是不是开设一个open dictionary的项目(?)~~，可以考虑使用搜狗发起的[细胞词典](http://pinyin.sogou.com/dict/index.php)