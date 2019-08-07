# Find QQ number by mobile QQ header image file name.
# 通过手机QQ头像文件名找回QQ号。速度很快，默认8线程，普通桌面i5 4代cpu大概每秒钟可以算500w多个QQ号，几分钟就能算出来。

# Usage: findqqnum.exe [-b <begin qq num, default 100000000>] [-e <end qq num, default 2999999999>] [-q <qq num, output qq num hash>] [-t <thread num, default 8>] [-h <hash>]
# Ex: findqqnum.exe -h DEA8D90E3170BCBEACE4786B4CB21AF7
#     findqqnum.exe -q 529698939
#     findqqnum.exe -h DEA8D90E3170BCBEACE4786B4CB21AF7 -b 529600000 -e 529699999
