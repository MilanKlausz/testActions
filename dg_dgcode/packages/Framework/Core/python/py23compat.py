"""Temporary module used during python2->python3 migration"""
from __future__ import print_function
_all__=['pickle','range','py2str','input']

#python2's cPickle essentially became python3's pickle module
try:
    import cPickle as _
    #python2:
    pickle=_
except ImportError:
    #python3, pickle already the right module
    import pickle as _
    pickle=_

#python2's xrange is (to some degree) a substitute for python3's range
try:
    xrange
    _range=xrange
except NameError:
    _range=range
range = _range

def py2str(o):
    if isinstance(o,float):
        #in py3 str(nbr)=repr(nbr), but in py2 str(nbr) is shortened a'la "%.12g":
        return '%.12g'%o
    else:
        return str(o)

_printorig=print
print=_printorig
if str==bytes:
    #py2: even with "from __future__ import print_function", the print fct lacks the flush option
    def _print(*args,**kwargs):
        doflush=False
        if 'flush' in kwargs:
            doflush=kwargs['flush']
            del kwargs['flush']
        _printorig(*args,**kwargs)
        if doflush:
            import sys
            sys.stdout.flush()
    print=_print

try:
    #python2
    raw_input
    input=raw_input
except NameError:
    #python3
    input=input
