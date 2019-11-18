from PIL import Image, ImageDraw, ImageFont
import random

str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

def gencode():
    return random.choice(str) + random.choice(str) + random.choice(str) + random.choice(str)

ans = []

for i in range(1000):
    img = "./codes/%03d.png" % i
    font = "./font.ttf"
    color = "#%02x%02x%02x" % (random.randint(1, 255), random.randint(1, 255), random.randint(1, 255))
    image = Image.open("template.png")
    draw = ImageDraw.Draw(image)
    width = image.size[0]
    font = ImageFont.truetype(font, 64)
    code = gencode()
    draw.text((random.randint(0, 100), random.randint(0, 50)), code, color, font)
    ans.append(code)
    image.save(img, "png")

print(ans)
