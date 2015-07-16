# 简介 #

该页是编译使用OpenCLAS的常见问题汇总。


# 问题 #

## Windows平台 ##

## Linux平台 ##

### 如何给系统安装GB2312, GBK, BIG5等编码？ ###

首先使用 locale -a 命令列出所有支持的locale。如果发现确实不存在上述编码，那么使用下列命令：
```
$ sudo locale-gen zh_＊.*
Generating locales...
  zh_CN.GB18030... done
  zh_CN.GB2312... done
  zh_CN.GBK... done
  zh_CN.UTF-8... up-to-date
  zh_HK.BIG5-HKSCS... done
  zh_HK.UTF-8... up-to-date
  zh_SG.GB2312... done
  zh_SG.GBK... done
  zh_SG.UTF-8... up-to-date
  zh_TW.BIG5... done
  zh_TW.EUC-TW... done
  zh_TW.UTF-8... up-to-date
Generation complete.
$ locale -a
C
POSIX
zh_CN
zh_CN.gb18030
zh_CN.gbk
zh_CN.utf8
zh_HK
zh_HK.utf8
zh_SG
zh_SG.gbk
zh_SG.utf8
zh_TW
zh_TW.euctw
zh_TW.utf8
```

### 如何判断文件是否是使用UTF-8 BOM存储的？ ###

执行下面的命令：

```
$ cat cpp/src/unit_test/unit_test.cpp |hd -n 10
00000000  ef bb bf 2f 2a 2a 2a 2a  2a 2a                    |.../******|
0000000a
```

如果开头三个字节分别是 **ef bb bf** ，那么就说明这个文件使用的是UTF-8 with BOM编码，否则就不带BOM（但是否是UTF-8不可确定）。

### 用g++编译的时候碰到UTF-8 BOM错误怎么办？ ###

```
$ g++ -I../../include unit_test.cpp -o unit_test
unit_test.cpp:1: 错误： 程序中有游离的‘\357’
unit_test.cpp:1: 错误： 程序中有游离的‘\273’
unit_test.cpp:1: 错误： 程序中有游离的‘\277’
In file included from unit_test.cpp:63:
...
```

或在英文系统下：

```
$ g++ -I../../include unit_test.cpp -o unit_test
unit_test.cpp:1: error: stray '\357' in program
unit_test.cpp:1: error: stray '\273' in program
unit_test.cpp:1: error: stray '\277' in program
In file included from unit_test.cpp:63:
...
```

这是由于OpenCLAS的源文件使用的是UTF-8 with BOM的格式保存。目前g++不支持这种文件格式，使用bomstrip或者其他软件去除源文件的UTF-8 BOM即可编译。