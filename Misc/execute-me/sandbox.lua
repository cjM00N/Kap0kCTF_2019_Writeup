-- have fun!

os.execute = false

function getflag()
    return "Kap0k{THE_FLAG_YOU_WANT_IS_NOT_HERE}"
end

function main()
    while true do
        io.write("> ")
        code = io.read()
        if code:find("getflag") then
            print("No flag for you hackers!")
        else
            print(pcall(load(code)))
        end
    end
end

main()
