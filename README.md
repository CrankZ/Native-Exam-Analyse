# Native-Exam-Analyse
PC桌面级客户端，电子化考试系统，支持多种题型，选择、填空、阅读理解、选词填空等。考试结果自动分析并保存。

目前传输方式基于局域网，但**题型解析**+**自动阅卷系统**+**成绩分析**+**考试结果保存**等功能是通用的，并不局限于数据传输方式。

## 基于

Qt 5.11.1 (MSVC 2015, 32bit)

## 考试流程

添加题型->设置考试时间与班级->发送试卷->开始考试->考试结束->保存考试结果

![考试流程]()

## 简介

Teacher端主界面

![Teacher主界面]()

Slave端启动并无界面，只有托盘图片。考试时会弹出考试界面

![托盘]()

![考试界面]()

## 主要特性

1：支持多种题型，**选择题**+**填空题**+**阅读理解**+**选词填空** 等

2：**自动阅卷**并**分析考试结果**，如班级平均、及格率、每题正确率等等

![主界面]()

3：考试结果**手动**保存为EXCEL表格的同时会**自动**保存考试源文件到程序目录“考试记录”文件夹下（源文件可拖拽到程序查看学生全部考试信息，详细看动图）

![拖拽考试记录]()

学生端考试结束后，成绩也会自动保存在程序目录下的“考试记录”文件夹下，同样可拖拽到程序内查看

![自动保存考试结果]()

**注意**：仅单选题支持随机

## 其他功能

开机自启
防多开
UAC管理员权限
学生考试期间禁止切换窗口

## 题型举例

### 选择题

#### 题目源文件

```
1.  turkey
A) n. 工具, 器具
B) n. 跳跃; 跳变 v. 跳跃; 惊起; 猛扑
C) n. 量, 数额; 交易
D) n. 火鸡

2.  rebuild
A) v.mod. 应当, 应该, 会 v. aux. 会, 应该（shall的过去时态）
B) vt. 通知;  指示; 教
C) vt. 留下极深的印象
D) vt. 重建

-----答案-----

1)D   2)D
```

#### 题目预览

![预览]()

### 填空题

#### 题目源文件

```
芜湖属于<completion>安徽</completion>省，淮北属于<completion>安徽</completion>省
芜湖属于<completion>安徽</completion>省
芜湖属于<completion>安徽</completion>省
芜湖属于<completion>安徽</completion>省
芜湖属于<completion>安徽</completion>省
```

#### 题目预览

![预览]()

### 选词填空

#### 题目源文件

```
<content>blablablalba
 A.arises
 B.ascends
 C.bound
 D.combination
 E.definite
 F.eventually
 G.interfere
 H.invade
I.manifesting
J.penalties
K.preserving
L.programmed
M.proximately
N.victims
O.widespread
</content>
1.

A)B)C)D)E)F)G)H)I)J)K)L)M)N)O)
2.
A)B)C)D)E)F)G)H)I)J)K)L)M)N)O)
3.
A)B)C)D)E)F)G)H)I)J)K)L)M)N)O)

------答案------

1)A 2)N 3)O
```

#### 题目预览

![预览]()

## Thanks

<https://github.com/stachenov/quazip>

<https://zlib.net/>

https://github.com/dbzhang800/QtXlsxWriter