# Final Fantasy XIV
本来这题是在国际服最新版本的环境下录的包（这样就没有很现成的工具了），然后把flag放在compressed的包里面（这样就不能strings了）。

不过改着改着……就变成了strings题…… 毕竟没有游戏的话直接分析协议又有点麻烦……

然后想了一想，太简单了也不好，于是又把flag base64了一下，这样就好说话了。

那么，只要把Hint给出的FFXIV-Packet-Dissector这一插件安装到wireshark中，就可以用它来过滤数据包了。之后，按照包的长度降序排列，很容易就能找到GroupMessage的包 —— 里面就放着base64后的flag。

为了防止 `grep "=="`，在flag前后加了俩下划线。

下午睡醒发现得降一下难度，不然misc都不快乐了。那我还能怎么办呢？strings题就strings题吧，就把第二版数据包放了上去。

`strings ffxiv.pcapng | grep Kap0k`

不过这么一来引导学习流量分析的效果就不是很好了。如果你有下载Wireshark并折腾一番的话，那也算没有白做。