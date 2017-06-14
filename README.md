# ZendApi
### 介绍

该项目是对Zend Engine的C接口使用C++11进行而面向对象的封装，从而屏蔽了底层Zend Engine API的接口复杂性，加快开发PHP扩展的效率。

项目官网地址：www.zendapi.org

### 设计目标

让php扩展开发更有趣<br/><br/>
你是否在开发扩展的时候遇到想使用PHP的一个接口，但是就是苦于不知道在哪里定义，就算找到了在哪里定义看到的定义是否让你抓狂，各种指针，各种宏定义
让你的生活变得不美好，哈哈，是时候尝试ZendApi啦，我们让你无忧无虑的调用底层的Zend Engine API, 安装语义对一维的C API进行二次组织，精心的
设计没一个接口，并且提供关于接口的详细文档与使用范例，还等什么，赶快到碗里来吧。

### 特性

1. 完全面向对象，对Zend Engine API进行二次定义
2. 使用现代的C++11语法进行开发，便于维护
3. 最大化屏蔽PHP版本对扩展开发的影响，ZendApi将对Zend Engine API不同版本带来的差异屏蔽掉
4. 高覆盖的单元测试，保证代码质量
5. 在封装的时候，尽最大能力保证性能

### 文档相关

用户手册: www.zendapi.org/documents</br>
API手册: www.zendapi.org/api

### 说明

当前ZendApi项目处于非常前期阶段，很多接口都不稳定，甚至会经常有大的变化，请不要用于实际项目，现阶段只适合学习用途。
项目参考 PHP-CPP进行实现，大家要是对PHP-CPP感兴趣，请点击 https://github.com/CopernicaMarketingSoftware/PHP-CPP

**声明：**</br>
ZendApi项目第一版中有些部分跟PHP-CPP将会高度一样，文件头也会加上PHP-CPP信息，一旦完成后ZendApi项目将会独立发展，当然适当的时机
我也会将代码反馈给PHP-CPP项目。

### 简单入门
#### 项目环境要求
暂时只支持linux服务器环境，php版本7.0以上，标准C++11编译器, cmake 3.5.0以上版本

#### 安装

TODO

### 贡献者列表

ZZU_SOFTBOY <zzu_softboy@163.com></br>
冰冻三尺，非一日之寒。欢迎大家加入，一起玩。

有什么建议，大家可以加这个微信，一起讨论</br></br>


<img width=200px src="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/zzusoftboy.png">

### License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html

### 特别感谢
Unicorn Team，感谢团队的自我驱动和包容的文化，让我开发ZendApi项目。

<img style = "width:100px;height:100px;float:left;display:block" src="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/unicornteam.png"/>
