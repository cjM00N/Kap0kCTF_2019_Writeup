# フラグはどこ？
我自己觉得这是一道比较普通的隐写题 - RePKG给出来之后其实就没有什么难度了，不给提示的话可能要发现那是Wallpaper Engine的pkg还需要点脑洞。

有同学到最后一步不知道那是个zip。binwalk / foremost / file / zsteg 在做隐写题的时候算是比较常见的工具了，建议了解一下；010 Editor也是可以用的。

## Structure
(RePKG) scene.pkg
- (RePKG) Cleasky.tex
  - scene.json: ctf.lsb_key = "idolmaster"
  - (cloacked-pixel - idolmaster) Cleasky.png
    - (password - Cleasky) flag.zip (comment)
