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

## 说明

如下图测试两幅图片`IMG1`和`IMG2`，分别验证基于`BFmatcher、FlannBasedMatcher`等的SIFT算法，对比其优劣。

(其中左图为`IMG1`,右图为`IMG2`)

![原图](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422164014.png)

## Sift原理

**SIFT算法的实质是在不同的尺度空间上查找关键点(特征点)，并计算出关键点的方向**。SIFT所查找到的关键点是一些十分突出，不会因光照，仿射变换和噪音等因素而变化的点，如角点、边缘点、暗区的亮点及亮区的暗点等。

SIFT算法的目的是让电脑也知道，不同的两张的图片，包含相同的内容。从不同的远近拍，不同的光照条件拍、不同的角度拍、图片拉长、反射变换、旋转，还是能把两张图片的两个点对应起来

### 作用

全景拍照的多张图片，用SIFT算法特征点匹配起来

### 介绍

在David G.Lowe的论文《IJCV2004 Distinctive image features from scale invariant keypoints》中，他给出了Sift算法的四个关键步骤：

- 尺度空间极值检测(Scale-space extrema detection)
- 关键点定位(Keypoint localization)
- 方向赋值(Orientation assignment)
- 关键点描述(Keypoint description)



### 1. 尺度空间极值检测(Scale-space extrema detection)



## Sift处理结果

首先是针对图像的灰度化显示：

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422164300.png)

之后完成特征点的标注，用红色圆圈表示：

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422164325.png)

其中`IMG1`被标注的关键点为3692个，`IMG2`被标注的关键点为2082

### BFMatcher

![Matches](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205318.png)

![good0.75](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205214.png)

good:138

![good0.5](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205415.png)

good:24

### Flann

![matches](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205753.png)

![Flann0.75](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205551.png)

good:138

![Flann 0.5](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190422205632.png)

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
    # print end - start, 's'
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
    # matchesMash = [[0, 0] for i in range(len(matches))]
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