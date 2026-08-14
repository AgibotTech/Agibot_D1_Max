## SDK与APP控制权问题详解

**原则**

**1.APP端被允许抢SDK端的控制权；**

**2.SDK端不允许抢APP端的控制权；**

### 情况1：APP先连接，SDK后连接。控制权在APP端，SDK无法控制。 ###

![图片](images/APP-Control_zh.png)

### 情况2：SDK先连接，APP后连接。控制权在SDK端，APP无法控制，但是APP可以强行抢夺SDK控制权。 ###

![图片](images/SDK-Control_zh.png)