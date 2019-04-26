## 作业

    在下列知识（技术）点中各选一个，通过查阅文献网页，掌握相关技术原理并用有关平台或库使用验证。
-----特征点检测------
sift
harris
fast
surf
Shi-Tomasi
obr
BRIEF

AKAZE

LBP
GLCM
Gabor

---------特征点检测-----------
BFMatcher.match() 和 BFMatcher.knnMatch()
findContour，拟合、参数计算
matchShapes

要求：
  1、整理出有关技术原理的ppt（10页左右）
  2、用python语言采用opencv/PIL/skimage等库来完成有关验证实例，自己找有特点、能够说明效果的原始图像。
  3、用10分钟左右介绍所作的工作。



## Sift原理

尺度不变特征转换(Scale-invariant feature transform或SIFT)是一种电脑视觉的算法用来侦测与描述影像中的局部性特征，它在空间尺度中寻找极值点，并提取出其位置、尺度、旋转不变量，此算法由 David Lowe在1999年所发表，2004年完善总结。

其应用范围包含物体辨识、机器人地图感知与导航、影像缝合、3D模型建立、手势辨识、影像追踪和动作比对。

此算法有其专利，专利拥有者为英属哥伦比亚大学。

局部影像特征的描述与侦测可以帮助辨识物体，SIFT 特征是基于物体上的一些局部外观的兴趣点而与影像的大小和旋转无关。对于光线、噪声、些微视角改变的容忍度也相当高。基于这些特性，它们是高度显著而且相对容易撷取，在母数庞大的特征数据库中，很容易辨识物体而且鲜有误认。使用 SIFT特征描述对于部分物体遮蔽的侦测率也相当高，甚至只需要3个以上的SIFT物体特征就足以计算出位置与方位。在现今的电脑硬件速度下和小型的特征数据库条件下，辨识速度可接近即时运算。SIFT特征的信息量大，适合在海量数据库中快速准确匹配

SIFT算法的目的是让电脑也知道，不同的两张的图片，包含相同的内容。从不同的远近拍，不同的光照条件拍、不同的角度拍、图片拉长、反射变换、旋转，还是能把两张图片的两个点对应起来

### 介绍

在David G.Lowe的论文《IJCV2004 Distinctive image features from scale invariant keypoints》中，他给出了Sift算法的四个关键步骤：

- 尺度空间极值检测(Scale-space extrema detection)
- 关键点定位(Keypoint localization)
- 方向赋值(Orientation assignment)
- 关键点描述(Keypoint description)

### 1. 尺度空间极值检测(Scale-space extrema detection)

要说明尺度空间，首先要从高斯卷积核讲起：

SIFT算法是在不同的尺度空间上查找关键点，而尺度空间的获取需要使用高斯模糊来实现。高斯模糊是一种图像滤波器，它使用正态分布(高斯函数)计算模糊模板，并使用该模板与原图像做卷积运算，达到模糊图像的目的。

尺度空间极值检测的第一步是获取图片的高斯金字塔，高斯金字塔是图片在不同尺寸下进行一个高斯模糊操作后的图片，可以看到每一组的金字塔里面又有多个层的图片，这里面各个层的图片的区别在于它们使用的高斯卷积核的模糊程度是不同的，也就是$\sigma$是不一样的

#### 1.1尺度空间表示



一个图像的尺度空间,$L(x,y,\sigma)$定义为一个变化尺度的高斯函数$G(x,y,\sigma)$与原图像$I(x,y)$的卷积

​						$L(x,y,\sigma) = G(x,y,\sigma)*I(x,y)​$

其中，\* 表示卷积运算

​						$G(x,y,\sigma) = \frac{1}{2π\sigma^2}e^{-\frac{(x-m/2)^2+(y-n/2)^2}{2\sigma^2}}$

其中m,n表示高斯模板的维度（由$(6\sigma+1)\times(6\sigma+1)$确定）。(x,y)代表图像的像素位置。$\sigma$是尺度空间银子，值越小表示图像被平滑的越小，相应的尺度也就越小。大尺度对于图像的概貌特征，小尺度对应于图像的细节特征

#### 1.2建立高斯金字塔（尺度空间）

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190423211653.png)

- 同样大小的图片为一组，一组中的各个图片又以层分（同一大小的图片论层，不同大小论组）

  ​					$O=log_2(min(M,N))-3$

其中$O$为推荐组数，$M,N$分别为图片的长宽

​						$S=n+3$

其中$S$为推荐层数，$n$为希望检测图片特征张数

上面一组是通过对下面一组图片的降采样得到的像素点（隔点采样）。

**每层的细节**

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424095055.png)

$k=2^{1/n}$

$\sigma_0=\sqrt{1.6^2 - 0.5^2} = 1.52$

论文中认为照相机模糊尺度是0.5（一个经验值），作者希望一次高斯核卷积之后尺度能有1.6，即我用1.52尺度的高斯核进行卷积得到的效果和1.6是一样的



#### 1.3建立高斯差分金字塔

为了寻找图片的特征点，我们要找到图片在不同尺度空间里的极值，这些极值通常是在图片的边缘或者是灰度突变的地方，所以要通过高斯金字塔两两**差分**得到**高斯差分金字塔**。在实际计算时，使用高斯金字塔每组中相邻上下两层图像相减，得到高斯差分图像，如下图所示，进行极值检测。

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424093243.png)

**在高斯差分金字塔的三个方向$( x , y , \sigma)$中找极值点**

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424143412.png)

极值点寻找的方法：图中x的位置是我们想要确定的极值点，然后需要寻找它上一层和下一层相邻的两层高斯高斯模糊差分图像，看这个点周围26个邻居像素点。如果该点（差分值）是这26个相邻像素点的最大/小值，那么这个点就是一个潜在的**特征极值点。**



### 2.关键点定位(Keypoint localization)

以上方法检测到的极值点是离散空间的极值点，以下通过拟合三维二次函数来精确确定关键点的位置和尺度，同时去除低对比度的关键点和不稳定的边缘响应点，以增强匹配稳定性、提高抗噪声能力。

#### 2.1 在高斯差分金字塔中找极值

离散空间的极值点并不是真正的极值点，下图右显示了二维函数离散空间得到的极值点与连续空间极值点的差别。利用已知的离散空间点插值得到的连续空间极值点的方法叫做子像素插值`（Sub-pixel Interpolation）`。

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424103205.png)



#### 2.2 调整极值点位置

$X(x,y,\sigma)^T$

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424104234.png)

在$X_0$处做泰勒展开,找$f(x)$的极值点

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424104721.png)

三个分量都要小于0.5

#### 2.3舍去低对比度的点

若 $|f(X)| < \frac{T}{n}，T\in[0.4,0.8]$，则舍去点$X$

#### 2.4 边缘效应的去除

一个定义不好的高斯差分[算子](https://baike.baidu.com/item/%E7%AE%97%E5%AD%90/970194?fr=aladdin)(~~对任何函数进行某一项操作都可以认为是一个算子，甚至包括求幂次，开方都可以认为是一个算子，只是有的算子我们用了一个符号来代替他所要进行的运算~~)的极值在横跨边缘的地方有较大的主曲率，而在垂直边缘的方向有较小的主曲率。

DOG算子会产生较强的边缘响应，需要剔除不稳定的边缘响应点。获取特征点处的Hessian矩阵，主曲率通过一个2x2 的Hessian矩阵H求出：

$H=\begin{bmatrix}D_{xx}(x,y) & D_{xy}(x,y)\\D_{xy}(x,y) & D_{yy}(x,y)\end{bmatrix}$

H的特征值$\alpha和\beta$代表x和y方向的梯度

$Tr(H) = D_{xx}+D_{yy} = \alpha+\beta$

$Det(H) = D_{xx}D_{yy} - (D_{xy})^2 = \alpha\beta$

其中:$\alpha > \beta 且 \alpha = \gamma\beta$

则

$\frac{(Tr(H))^2}{Det(H)} = \frac{(\alpha + \beta)^2}{\alpha\beta} = \frac{(\gamma\beta + \beta)^2}{\gamma\beta^2} = \frac{(\gamma+1)^2}{\gamma} ，建议\gamma取10.0$

D的主曲率和H的特征值成正比，令为α最大特征值，β为最小的特征值，则公式的值在两个特征值相等时最小，随着的增大而增大。值越大，说明两个特征值的比值越大，即在某一个方向的梯度值越大，而在另一个方向的梯度值越小，而边缘恰恰就是这种情况。所以为了剔除边缘响应点，需要让该比值小于一定的阈值，因此，为了检测主曲率是否在某域值r下，只需检测

$\frac{(Tr(H))^2}{Det(H)} = \frac{(\gamma+1)^2}{\gamma} $

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424124321.png)

用差商代替微分

### 3.方向赋值(Orientation assignment)

位置找到之后我们要找一下这个极值点的主方向

首先会计算关键点附近各个像素之间梯度方向和梯度幅值。从图中可以到一些箭头，箭头方向是梯度方向，箭头长度代表了梯度幅值大小，最后会用一个半径为$3*1.52\sigma$(lowa建议)的圆圈作为窗口，统计窗口内这些梯度方向上幅值的累计值。右图所示，幅值在各个梯度方向上的累计值，可以看到总共有8个方向，他们的累计值第5个方向是最高的，这方向可以作为该特征点的主方向（这里给了45°一柱，所以有8柱；原文是10°，共36柱）。值得一提的是特征点可能不止一个方向，有可能会有一个辅峰，如第2个方向的幅值与第5个幅值差别并不太大，所以有时候会把第2个方向作为辅方向，有时候你会看到结果在同一个位置有两个方向。

~~统计以特征点为圆心，以该特征点所在的高斯图像的尺度的1.5倍为半径的圆内的所有的像素的梯度方向及其梯度幅值，并做$1.5\sigma $的高斯滤波~~

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424140723.png)

### 4.关键点描述(Keypoint description)

在两张图片分别找到他们的关键点，然后把他们对应的关键点要匹配起来。匹配的过程需要描述符（用的是KNN），描述符是一个128维的向量（4x4格子*8个方向的向量）

这个区域内我们分成了$4*4$的小区域，每个小区域我们要统计梯度在不同方向的幅值的累计直方图，直方图共8个方向。之后统计这些区域内梯度的方向。可以看到左上角的这个小区域，它在往正上的方向是比较明显的。所以对这个关键点来说，构造了128维的描述器

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190424140907.png)

## Sift处理结果

如下图测试两幅图片`IMG1`和`IMG2`，分别验证基于`BFmatcher、FlannBasedMatcher`等的SIFT算法，对比其优劣。

(其中左图为`IMG1`,右图为`IMG2`)

![原图](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422164014.png)

首先是针对图像的灰度化显示：

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422164300.png)

之后完成关键点的定位，用红色圆圈表示：

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422164325.png)

其中`IMG1`被标注的关键点为3692个，`IMG2`被标注的关键点为2082

### BFMatcher

![good0.75](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205214.png)

good:138

![good0.5](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205415.png)

good:24

## 源代码

> 环境: python2.7+Anaconda2+win10
>
> 关键库:  opencv-python3.4.2.16  + opencv-contrib-python3.4.2.16

```python
# -*- coding: utf-8 -*-
import numpy as np
import cv2
from matplotlib import pyplot as plt
import time

def DrawPic(img_1, img_2):
    hmerge = np.hstack((img_1, img_2))
    cv2.imshow("Src", hmerge)
    cv2.waitKey(0)


def DrawGrayPic(img_1, img_2):
    """灰度显示图像"""
    
    img1_gray = cv2.cvtColor(img_1, cv2.COLOR_BGR2GRAY)  # 灰度处理图像
    img2_gray = cv2.cvtColor(img_2, cv2.COLOR_BGR2GRAY)
    hmerge = np.hstack((img1_gray, img2_gray))  # 水平拼接
    cv2.imshow("gray", hmerge)  # 拼接显示为gray
    cv2.waitKey(0)


def DrawKeyPoints(img_1, keypoint_1, img_2, keypoint_2):
    """利用Sift算法得到的kp画出特征点，并显示为红色元圆圈"""

    img1_kp = cv2.drawKeypoints(img_1, keypoint_1, img_1, color=(255, 0, 255))  # 画出特征点，并显示为红色元圆圈
    img2_kp = cv2.drawKeypoints(img_2, keypoint_2, img_2, color=(255, 0, 255))
    hmerge = np.hstack((img1_kp, img2_kp))  # 水平拼接
    cv2.imshow("point", hmerge)  # 拼接显示为point
    cv2.waitKey(0)


def DrawMatchesLine_BFMatcher(img_1, keypoint_1, description_1, img_2, keypoint_2, description_2):
    """BFMatcher解决匹配"""
    
    start = time.time()
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(description_1, description_2, k=2)

    print 'matches...', len(matches)
    # 调整ratio
    good = []
    for m, n in matches:
        if m.distance < RATIO * n.distance:
            good.append([m])

    print 'good...', len(good)

    # good OR matches
    img5 = cv2.drawMatchesKnn(img_1, keypoint_1, img_2, keypoint_2, good, None, flags=2)
    end = time.time()
    print 'speed... ', round(end-start, 3), 's'
    cv2.imshow("BFMatch", img5)
    cv2.waitKey(0)


def DrawMatchesLine_Flann(img_1, keypoint_1, description_1, img_2, keypoint_2, description_2):
    """Flann解决匹配"""

    start = time.time()

    FLANN_INDEX_KDTREE = 0
    index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
    search_params = dict(checks=50)
    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(description_1, description_2, k=2)
    print 'matches...', len(matches)

    good = []
    for m, n in matches:
        if m.distance < RATIO * n.distance:
            good.append([m])

    print 'good...', len(good)

    img5 = cv2.drawMatchesKnn(img_1, keypoint_1, img_2, keypoint_2, good, None, flags=2)
    end = time.time()
    print 'speed... ', round(end - start, 3), 's'
    # print end - start, 's'
    cv2.imshow("FLANN", img5)
    cv2.waitKey(0)


# 900 x 600
IMG1 = './img/IMG_1.jpg'
IMG2 = './img/IMG_2.jpg'
RATIO = 0.75    # RATIO调整参数
if __name__ == '__main__':

    sift = cv2.xfeatures2d.SIFT_create()

    img1 = cv2.imread(IMG1)
    kp1, des1 = sift.detectAndCompute(img1, None)  # des是描述子

    img2 = cv2.imread(IMG2)
    kp2, des2 = sift.detectAndCompute(img2, None)  # des是描述子

    print "kp1: ", len(kp1), "\nkp2: ", len(kp2)

    # 图像灰度处理
    DrawGrayPic(img1, img2)

    # 画出特征点
    DrawKeyPoints(img1, kp1, img2, kp2)

    # BFMatcher 和 Flann 两个方法匹配连线
    DrawMatchesLine_BFMatcher(img1, kp1, des1, img2, kp2, des2)
    # DrawMatchesLine_Flann(img1, kp1, des1, img2, kp2, des2)
    cv2.destroyAllWindows()
```

## 引用列表

IJCV2004 Distinctive image features from scale invariant keypoints --David G.Lowe

https://blog.csdn.net/zddblog/article/details/7521424 SIFT算法详解

https://baike.baidu.com/item/%E7%AE%97%E5%AD%90/970194?fr=aladdin 百度百科——算子