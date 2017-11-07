<img width="350" src ="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/zendapi.png"/>

#### zendAPI 是什么？

zendAPI 是对 Zend Engine 的 C 接口使用 C++ 的最新标准 C++11 进行而面向对象的封装，从而屏蔽了底层 Zend Engine API 的接口复杂性，加快开发 PHP 扩展的效率。从而让 PHP 的扩展开发成为一种享受，不用在考虑不同 PHP 版本带来的差异性，让开发者专注于自身的业务逻辑。

#### What is zendAPI project

zendAPI is a project to wrapper the C API of Zend Engine into object oriented programming interface that implement with modern C++11 programming language. The zendAPI project provide php extension developer with a simple, rubust and OOP style API, so developer don't need learn the ugly and complex raw Zend Engine API, thereby let the php extension development become a simple and rapid task. 

#### zendAPI 的使命

让 PHP 的扩展开发成为一种享受

#### The mission of zendAPI project

let PHP extension development become a kind of enjoyment! 

#### 为什么项目名叫做 zendAPI

考虑到项目本身就是对 PHP 语言引擎的开发接口的封装已经 PHP 的语言引擎叫做 Zend Engine，为了方便大家记住我便将项目命名为 zendAPI，官方网站使用 zendapi.org

#### Why I named the project zendAPI

The main goal of the project is to wrap PHP's raw C API into C++ OOP API, because of the virtual machine of PHP language called Zend Engine and our project is an OOP API on Zend Engine, so I give the name zendAPI to this project, the official website is http://www.zendapi.org

#### zendAPI 的特性

1. 完全面向对象，对 Zend Engine API 进行二次定义
2. 使用现代的 C++11 语法进行开发，便于维护
3. 最大化屏蔽 PHP 版本对扩展开发的影响，zendAPI 将对 Zend Engine API 不同版本带来的差异屏蔽掉
4. 高覆盖的单元测试，保证代码质量
5. 在封装的时候，尽最大能力保证性能
6. 致力于项目库的二进制兼容

#### Features

1. regroup the raw PHP C API according OOP style
2. using modern C++11 standard
3. support all the versions above PHP 7.0
4. well tested by runtime tests and unittests
5. high performance
6. efforts to provide binary compatible

#### 项目说明

为了减少研发精力，在决定开发 zendAPI 的时候，在设计思想上我们参考了国外的同类项目 PHPCPP，非常感谢这个团队将这么卓越的项目带给了 PHP 社区，我们在这里表示感谢，我们也将在 zendAPI 项目中对 PHPCPP 的引用加上版权声明，同时把我们发现的代码 bug 反馈到 PHPCPP。

#### Notes

time saving, when develop zendAPI project, I read the code of PHPCPP project, thanks for the team of this project, in some files I use some classes and functions from PHPCPP project and put the copyright header infomation in these files.

PS:

PHPCPP 的 github 地址是： https://github.com/CopernicaMarketingSoftware/PHP-CPP 大家有兴趣可以去研究。

PS:

The Github reposition of PHPCPP: https://github.com/CopernicaMarketingSoftware/PHP-CPP you can learn it if you like.

#### 国内同类型的项目推荐
目前国内有一个跟 zendAPI 比较类似的项目，这个项目是 swoole 项目作者开发，值得推荐。
项目名字 PHP—X
项目的 github 地址是 https://github.com/swoole/PHP-X 大家有兴趣可以研究。

#### same kind of project in China

At the time of zendAPI developing, the author of PHP network library swoole give us a project named PHP-X, do the same job of zendAPI, here is it Github Repo address:https://github.com/swoole/PHP-X

#### 怎么参与交流
#### Contact me

下面是我们项目的线上交流QQ群和微信的二维码，大家可以扫码加入：

here is my QQ and wechat QR Code, if you interested in zendAPI project, just scan the QR codes:
<div>
<img width = "200" src="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/qq.png"/>
<img width = "20" src="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/blank.png"/>
<img width = "200" src="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/wechat.png"/></div>
</div>

开源中国主页: https://www.oschina.net/p/zendapi

Segmentfault技术圈：https://segmentfault.com/g/1570000010966072

meanwhile，I wrote blog at:

China opensource community: https://www.oschina.net/p/zendapi
Segmentfault's tech group: https://segmentfault.com/g/1570000010966072


#### 特别感谢

360 无线电安全部 独角兽团队 (unicorn team)

#### Special thanks

360 unicornteam for support I develop zendAPI project.

<div style="width:240px; height: 240px;float:left"><img src="https://raw.githubusercontent.com/qcoreteam/zendapi/master/assets/images/unicornteam.png"/></div>
