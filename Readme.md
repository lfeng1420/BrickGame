方块游戏（BrickGame）
===

进度
----
A	坦克大战	  （已完成）<br>
B	青蛙过河	  （已完成）<br>
C	赛车	  （已完成）<br>
D	匹配	 （已完成） <br>
E	贪吃蛇	 （已完成）<br>
F	弹球	  （已完成）<br>
G	俄罗斯方块	  （已完成）<br>
H	FlappyBird	（已完成）<br>
I	俄罗斯方块[包含特殊方块]  	（已完成）<br>
J	滚动版弹球  	（已完成）<br>
K	打方块	（已完成）<br>
L	加方块	（已完成）<br>



截图
----
![](https://github.com/lfeng1420/BrickGame/blob/master/Images/1.png)
![](https://github.com/lfeng1420/BrickGame/blob/master/Images/2.png)
![](https://github.com/lfeng1420/BrickGame/blob/master/Images/3.png)
![](https://github.com/lfeng1420/BrickGame/blob/master/Images/4.png)



发布记录
----
Windows Phone & Android
---
### 	  2018/05/09	  V2.0.1.0
新增：方向按钮支持自定义缩放倍率，调整设置中的方向按钮缩放值即可，默认为100%；<br>
调整：俄罗斯方块(G/I)和加方块(L)左右移动检查间隔修改，修复按一次按钮有时会触发移动两格的问题；<br>
调整：振动持续时间微调；<br>
修复：坦克大战中BOSS阶段我方坦克可以自由移动的问题。<br>

### 	  2018/05/06	  V2.0.0.0
代码重构，UI和逻辑拆分，降低耦合<br>
新增：坦克大战中我方坦克解锁哲♂学体位；<br>
新增：俄罗斯方块存档支持保存当前分数、当前形状和下一个形状，恢复时完整还原现场；<br>
新增：俄罗斯方块手动存档成功时将打开自动恢复功能；<br>
新增：俄罗斯方块(I)炸弹支持按A按钮触发爆炸；<br>
新增：方块偏移，曲面屏手机可通过该选项调整游戏区域偏移，仅在竖屏时有效；<br>
新增：游戏结束时选择界面将自动显示之前选的游戏（例如选了游戏G开始之后到结束返回选择界面时，仍然显示游戏G，而不是游戏A）；<br>
UI改进：设置界面换回列表样式，点击时选项背景替换为蓝色；<br>
UI改进：横屏状态下底部四个按钮移到右侧，相应地，右手模式下移到左侧，方向按钮和A按钮下移；<br>
UI改进：横屏状态下速度和等级标签恢复显示；<br>
修复：第一次从游戏结束界面到选择游戏界面时可能出现方块状态错乱的问题。<br>

### 	  2017/12/26	  V1.11.1.0
修复在同时按下方向按钮的任意两个后移动再放开会出现按钮一直处于按下状态的问题，感谢 @leoxxx 的反馈<br>

### 	  2017/12/12	  V1.11.0.0
新增手柄支持（仅Android和UWP版）<br>

### 	  2017/06/27	  V1.10.1.0
修复上一个版本中只显示英文版本的问题。<br>

### 	  2017/06/27	  V1.10.0.0
1、设置界面更换，使用图标+标签形式；<br>
2、新增音量控制，支持设置背景音乐和音效音量；<br>
3、控制按钮重绘。<br>

### 	  2017/03/07	  V1.9.1.0
恢复方向按钮按下效果。<br>

### 	  2017/03/05	  V1.9.0.0
新增右手模式，开启时方向按钮将和A按钮对调显示，在设置中可以开启或关闭。<br>

### 	  2017/02/09	  V1.8.6.0
振动调整，按钮振动持续时间调短，游戏结束和通关增加持续1秒振动。<br>

### 	  2017/02/08	  V1.8.5.0
1、调整了坦克大战中的BOSS难度。<br>
2、微调了游戏图标。 <br>

### 	  2017/02/08	  V1.8.4.0
1、修复暂停游戏之后按设置再返回游戏时暂停失效的问题；<br>
2、替换了游戏图标。<br>

### 	  2017/01/25	  V1.8.3.0
1、修复首屏BG不居中问题；<br>
2、新增振动功能，支持方向键和A键振动，可在设置中开启或关闭（是的，新加了一个权限）；<br>
3、移除语言选项，将由系统语言决定。<br>

### 	  2017/01/19	  V1.8.2.0
1、游戏H增加长按上键（A键）支持；<br>
2、俄罗斯方块和青蛙过河游戏中部分代码存在逻辑问题，会导致游戏崩溃，现已修复；<br>
3、替换一版图标。 <br>

### 	  2016/12/11	  V1.8.0.0
1、新增设置界面，包括自定义俄罗斯方块上键功能、即时存档等；<br>
2、替换底部最后一个图标，删除底部所有按钮附加功能。 <br>

### 	  2016/10/23	  V1.7.1.0
1、修改俄罗斯方块上键为旋转，调整按下键时方块的下落速度；<br>
2、进行俄罗斯方块时点开始按钮两次可以立即存档。<br>

### 	  2016/10/23	  V1.7.0.0
1、新增两个游戏：打方块/加方块（我也不知道该叫什么= =）；<br>
2、修复横屏下分数不会更新一直为0的bug；<br>
3、微调了俄罗斯方块左右移动时间间隔。 <br>

### 	  2016/10/11	  V1.6.0.0
1、新增夜间模式，按两次心形按钮即可切换；<br>
2、微调了俄罗斯方块按钮持续检查时间，按住方向键时方块将比之前移动得快一些。 <br>

### 	  2016/10/01	  V1.5.0.0
1、新增横屏模式，点两次声音按钮可切换。<br>

### 	  2016/09/22	  V1.4.6.0
1、修复之前版本滑动点按钮时前一个按钮一直触发的情况；<br>2、俄罗斯方块增加方块下落音效； <br>3、修改另一个背景颜色为深灰色，以及第二屏一个细节。<br>

### 	  2016/09/22	  V1.4.4.0
1、修复方向键的问题，靠近中心位置触发的方向可能不正确（误触问题已经修复^ ^）； <br>
2、删除原先的背景图片（2M多），换用纯色背景，减小包大小。<br>

### 	  2016/08/25	  V1.4.2.0
1、俄罗斯方块上键增加存档功能，可以随时按上键保存；<br>
2、修复打砖块F滚动的问题；<br>
3、修复打砖块挡板边界判断相关问题，球到挡板边界时将视情况回弹。 <br>

### 	  2016/08/07	  V1.4.0.0
1、新增滚动版打砖块（J）；<br>
2、特殊方块小点调整：当前列无空白位置填充时，默认停在当前列最后一个空白位置；<br> 
3、特殊方块炸弹调整：爆炸时将停留闪烁几次； <br>
4、贪吃蛇反向移动bug修复。 <br>

### 	  2016/08/02	  V1.3.12.0
1、俄罗斯方块新增特殊方块“小点”，设定是仅在某个位置空白且上下方都已填充的情况下才会填充（不太清楚以前的规则，所以先这样了）；<br>
2、加方块的特殊方块增加一个格子，便于和减方块的特殊方块区分；<br>
3、打砖块和贪吃蛇初始方向随机；<br>
4、贪吃蛇支持反向移动；<br>
5、微调FlappyBird中小鸟的下降速度；<br>

### 	  2016/07/26	  V1.3.9.0
1、修复了存档不会保存上一次分数的问题；<br>
2、调整了方向键缩放倍数，根据分辨率自动调整缩放大小；<br>
3、坦克大战中我方坦克增加闪烁标记；<br>
4、修复坦克大战中BOSS会出屏幕的问题，微调了BOSS发射子弹时间间隔以及我方坦克发射子弹时间间隔；<br>
5、调整了FlappyBird中小鸟的下降速度；<br>

### 	  2016/07/23	  V1.3.4.0 
1、俄罗斯方块存档功能增加开启和关闭提示，游戏退出时也增加了提示；<br>
2、修复上一个版本中未调整的坦克发射子弹速度问题。<br>

### 	  2016/07/22	  V1.3.3.0 
1、新增俄罗斯方块存档功能，1秒之内点击重置按钮（底部第三个按钮）两次即可开启/关闭自动恢复存档，在正常游戏（支持暂停状态）时点击重置按钮或按返回键退出游戏（非强制结束游戏进程）会自动保存存档，如果开启了自动恢复存档，则在下一次进入俄罗斯方块（G或I）时恢复；<br> 
2、修复了误触返回键游戏退出的问题； <br>
3、调整了坦克大战中坦克发射子弹的速度。<br>

### 	  2016/07/16	  V1.3.1.0 
1、修复坦克大战中子弹不会抵消问题；<br>
2、修改进入游戏后的首屏界面。<br>

### 	  2016/03/20	  V1.3.0.0 
1、新增包含特殊方块的俄罗斯方块游戏；<br>
2、坦克大战增加关卡结束前打BOSS功能；<br>
3、改进按钮响应机制，坦克大战发射子弹不再出现不触发情况，俄罗斯方块左右按钮不再出现移动两列情况。<br>


Windows Phone
----
### 	  2016/03/07	  V1.2.0.0 
1、新增Flappy Bird仿制版游戏； <br>
2、修复贪吃蛇中一个导致崩溃的Bug； <br>
3、增加换背景功能，双击爱心按钮即可更换。 <br>

### 	  2016/02/21	  V1.1.2.0
1、修复一个可能导致严重逻辑错误的Bug；<br>
2、调整贪吃蛇游戏逻辑，改为无限模式；<br>
3、更换评分图标，调整磁贴。<br>

### 	  2016/02/20	  V1.1.1.0
删除无用权限，修复评分（底部第四个按钮）不正确的Bug。

### 	  2016/02/19	  V1.1.0.0
1、新增游戏：<br>
　　俄罗斯方块；<br>
2、Bug修复：<br>
　　修复点击暂停后再点重置按钮游戏卡住的Bug；<br>
　　修复游戏结束时爆炸效果不见的Bug；<br>
　　修复青蛙过河中通过的青蛙位置可以相同的Bug；<br>
3、UI更新：<br>
　　更换方向键以及开始、声音按钮，增加背景；<br>
4、调整：<br>
　　调整部分逻辑，减少不必要的屏幕刷新过程；<br>
　　调整坦克大战中创建坦克的时间间隔，将随着速度的增加而缩短；<br>
　　调整打砖块小球移动时间间隔。<br>

### 	  2016/02/11	  V1.0.0.0
方块游戏第一版。<br>
链接：https://www.microsoft.com/zh-cn/store/games/方块游戏/9nblggh5h3fk


Android
----
### 	  2016/02/22	  V1.0.2.0
1、修复一个可能导致严重逻辑错误的Bug；<br>
2、调整贪吃蛇游戏逻辑，改为无限模式；<br>
酷安：http://www.coolapk.com/game/com.lfeng.brickgame<br>
已上线应用宝、91、百度、安卓hiapk<br>
应用宝：http://a.app.qq.com/o/simple.jsp?pkgname=com.lfeng.brickgame<br>
91：http://apk.91.com/Soft/Android/com.lfeng.brickgame-1-1.0.2.html<br>
百度：http://shouji.baidu.com/game/item?docid=8949108<br>
安卓hiapk：http://apk.hiapk.com/appinfo/com.lfeng.brickgame/1<br>

### 	  2016/02/19	  V1.0.1.0
删除无用的网络权限。<br>

### 	  2016/02/18	  V1.0.0.0
方块游戏第一版已上线360手机助手。<br>
360手机助手：http://zhushou.360.cn/detail/index/soft_id/3222557<br>
下载地址：http://openbox.mobilem.360.cn/index/d/sid/3222557<br>



开源许可协议
----
MIT License

Copyright (c) 2017 lfeng

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
