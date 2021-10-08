import os

def extract_zlib_license():
    zlib_h=os.path.join(os.getenv('DGCODE_DIR'),
                        'packages/Framework/Utils/MiniZLib/libinc/zlib.h')
    out = []
    for l in open(zlib_h):
        l=l.rstrip()
        out += [l]
        if 'madler@' in l and 'jloup@' in l:
            break;
    out+=['*/']
    return out

#for l in extract_zlib_license():
#    print l
