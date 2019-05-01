#！ python
"""
模拟内置函数
print() --> myprint()
map()  --> mymap()
zip()  --> myzip()  优化，使zip并非截断，而是补充None
"""
import sys

def myprint(*args,**kargs):
    """
    模拟print函数
    Emulate most of the 3.X print function for use in 2.X(and 3.X)
    Call signature: print3(*args,sep=' ',end ='\n', file=sys.stdout)
    """
    sep = kargs.get('sep',' ')
    end = kargs.get('end','\n')
    file = kargs.get('file',sys.stdout)
    output = ''
    first = True
    for arg in args:
        output += ('' if first else sep) + str(arg)
        first = False
    file.write(output+end)

def myprint0(*args,**kargs):
    """
    细致优化：其实上面已经够用了
    Use 2.X/3.X keyword args deletion with defaults
    """
    sep = kargs.pop('sep',' ') #dict.pop() 删除取回的传入项，并检查字典是否为空
    end = kargs.pop('end','\n')
    file = kargs.get('file',sys.stdout)
    if kargs: raise TypeError('extra keywords: %s' %kargs)  # 检查有无多余项报错
    output = ''
    first = True
    for arg in args:
        output += ('' if first else sep) + str(arg)
        first = False
    file.write(output+end)

myprint("fanfamsf","dsakda",sep='...')
myprint0("fanfamsf","dsakda",sep='...')


def mymap(func,*seqs):
    res = []
    for args in zip(*seqs):
        res.append(func(*args))
    return res

print(mymap(abs,[-2,-1,0,1,2]))
print(mymap(pow,[1,2,3],[2,3,4,5]))

def myzip(*seqs,pad=None):
    seqs = [list(S) for S in seqs]
    res = []
    while any(seqs): # any--->all时为正常的zip()
        #res.append(tuple(S.pop(0) for S in seqs))
        res.append(tuple((S.pop(0) if S else pad)for S in seqs))
    return res

S1,S2 = 'abc','xyz123'
print(myzip(S1,S2))

