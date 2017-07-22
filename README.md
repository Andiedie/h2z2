![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/85793256.jpg)

# H2Z2
现代操作系统应用开发期末项目

## 简介
一款在线多人生存游戏，玩家在游戏地图中随机出生，通过捡取的武器相互战斗，最后存活的玩家获得胜利。

## 安装
```bash
git clone https://github.com/Andiedie/h2z2.git
```

### 服务器
服务器需要[Node.js](https://nodejs.org/en/)运行环境
```bash
cd back-end
npm i --production
npm start
```

服务器默认监听端口`3000`

### 客户端
编译客户端安装需要[Cocos](http://www.cocos.com/) 3.15以上版本
```bash
cocos new -l cpp -p com.zyuco.h2z2 front-end
```
需要保证`front-end`文件夹中有cocos新建的目录`cocos2d`

使用[visual studio](https://www.visualstudio.com/)打开`front-end/proj.win32/h2z2.sln`并编译项目

### 可执行文件
你可以使用我们的测试服务器`119.29.252.110:3000`, 但是这个服务器不保证长期开启

编译好的客户端：http://pan.baidu.com/s/1cMtTSE 密码：ytru

运行客户端前请保证电脑已安装各版本的`Microsoft Visual C++ Redistributable`, 可以使用[DiretX修复工具](http://pan.baidu.com/s/1bX8VXs)快速安装。

## 详细介绍
### 登录
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/38474401.jpg)

输入服务器地址和端口，即可连接到服务器。之后打开客户端自动填充上次连接使用的服务器和端口。

### 等待
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/29385107.jpg)

连入服务器的玩家进入等待列表。由于当前服务器限制，同时只能支持一场游戏。
- 当前没有游戏进行时：点击开始，所有等待的玩家一同加入游戏。
- 当前正在进行一场游戏：等待上一场结束后一同进入下一场游戏。

### 开始
开始游戏时，在等待区的所有玩家会一同加入游戏，随机出生在一张地图上。

#### UI
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/90432265.jpg)

#### 操作
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/91610331.jpg)

### 地图
#### 玩家
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/28281241.jpg)

玩家出生在地图上，颜色与位置完全随机，初始拥有200生命值

#### 枪械
玩家初始没有任何攻击手段，需要通过接触的方式，拾取地图上的武器。拥有武器的玩家右下角会显示弹药信息。
##### 白
较高攻击，较高射速

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/96198801.jpg)

##### 黄
高攻击，较低射速

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/32735286.jpg)

##### 红
极高攻击，极低射速

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/95612702.jpg)

##### 蓝
高攻击，较低射速

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/45808790.jpg)

##### 黑
低攻击，极高射速

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/40475451.jpg)

#### 血包
地图上会随机分布适宜数量的血包，玩家可以拾取血包补充生命值

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/90117620.jpg)


#### 障碍物
地图上会随机出现一些障碍物，玩家与子弹都无法穿过障碍物

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/29426763.jpg)

#### 毒圈
地图四个角落会有逐渐扩大的有毒区域，进入毒圈范围的玩家会持续收到伤害，毒圈越大伤害越高。
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/63644919.jpg)

### 观战
死亡的玩家进入观战模式，左键可以切换观战对象。

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/21000389.jpg)

### 胜利
最后存活的玩家获得胜利

![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/78506684.jpg)

## 素材来源
### 游戏名
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/18607460.jpg)

本作的很多玩法都是源于游戏[H1Z1: King of the Kill](https://www.h1z1.com/king-of-the-kill/home)，因此取名`H2Z2`


### 玩家
![](http://ocphk5wc7.bkt.clouddn.com//17-7-22/65887227.jpg)

玩家的素材来源于游戏[NieR:Automata](http://www.jp.square-enix.com/nierautomata/)的Hacking模式。

### 声音
取自Flash游戏[僵尸危机3](http://www.4399.com/flash/5931.htm)

## 已知问题
- 偶尔出现的崩溃与卡顿
- 中文路径下没有游戏声音
- 可能出现的武器命中不同步
- 切换观战对象时可能出现的崩溃
- 在等待界面直接结束游戏可能出现Socket不中断的错误
- 高延迟下多个玩家可以同时拾取同一个武器或血包

## LICENSE
MIT
