from  Crypto.Cipher import AES
def aes_decrypt(plain,key,iv=None):
    k = AES.new(key,AES.MODE_ECB);
    # k = AES.new(key,AES.MODE_CBC,iv);
    cipher = k.decrypt(plain);
    return cipher;
def aes_encrypt(plain,key,iv=None):
    k = AES.new(key,AES.MODE_ECB);
    # k = AES.new(key,AES.MODE_CBC,iv);
    cipher = k.encrypt(plain);
    return cipher;

key = [ 0x24, 0x7e, 0x1, 0x11,0x28, 0xae, 0xd2, 0xa6,0xab, 0xf, 0x15, 0x88,0x09, 0xf, 0x4f, 0x3c ];
key = ''.join( chr(i) for i in key );
cipher = [100, 157, 209, 60, 122, 17, 59, 225, 6, 80, 66, 132, 163, 17, 176, 183, 53, 0, 74, 24, 63, 49, 32, 79, 18, 189, 112, 50, 87, 30, 220, 78]
cipher = ''.join( chr(i) for i in cipher );
print aes_decrypt(cipher,key);
a = aes_encrypt('kap0k{B4by_Aes_1s_1nterest1ng}'.ljust(32,'\x00'),key);
print [ ord(i) for i in a]
print len('kap0k{B4by_Aes_1s_1nterest1ng}')
# kap0k{B4by_Aes_1s_funny}