for i = 0, 999 do
    os.execute(("tesseract %03d.png stdout > tmp"):format(i))
    f=io.open("tmp", "r")
    s=f:read("*a")
    f:close()
    z=io.open("test", "a")
    z:write(s:match("([a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9])") or "....")
    z:close()
end
