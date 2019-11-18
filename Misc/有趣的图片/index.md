## 有趣的图片

一张完整的二维码（256*256）分割成256份，写个脚本按文件名拼接即可

```python
from PIL import *
from PIL import Image
import glob

def cut(filename):
    flag = Image.open(filename)
    print(flag.size)
    num = 0
    for i in range(flag.size[0]//16):
        for j in range(flag.size[1]//16):
            im = flag.crop((16 * i, 16 * j, 16 * i + 16, 16 * j + 16))
            im.save("res/{}.png".format(num))
            num += 1
def add():
    img_path = glob.glob("./res/*.png") 
    img_path = [int(i[6:-4]) for i in img_path]
    img_path.sort()
    result = Image.new('RGB', (256, 256), (0, 0, 0))
    num = 0
    for i in range(0, 256, 16):
        for j in range(0, 256, 16):
            result.paste(Image.open("./res/" + str(img_path[num]) + ".png"), (i, j))
            num += 1
    #print(num)
    result.show()


if __name__ == '__main__':
    filename = "flag.png"
    #cut(filename)
    add()

```


