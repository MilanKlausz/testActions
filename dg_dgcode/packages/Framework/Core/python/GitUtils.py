import os
import pathlib
import subprocess

#Duplicated from .githooks/hooks.py:
def _strbytes2path(strbytes_path):
    try:
        s = os.fsdecode(strbytes_path) if isinstance(strbytes_path,bytes) else strbytes_path
        fpath = pathlib.Path(s)
        str(fpath).encode('utf8')#check that it can be encoded in utf8 (TODO: Require ASCII?)
    except (UnicodeDecodeError,UnicodeEncodeError):
        raise SystemExit('Forbidden characters in filename detected! : "%s"'%fn)
    return fpath

#Duplicated from .githooks/hooks.py:
#We set GIT_CONFIG=/dev/null to avoid user/system cfg messing with the
#result. We also set GIT_OPTIONAL_LOCKS=0 instead of using
#--no-optional-locks, since the functionality is not available in all git
#versions we want to support:
_safe_git_cmd = '/usr/bin/env GIT_CONFIG=/dev/null GIT_OPTIONAL_LOCKS=0 git'.split()

#Duplicated from .githooks/hooks.py:
def git_status_iter(extra_args=[],staged_only=True,status_filter=''):
    #We set GIT_CONFIG=/dev/null to avoid user/system cfg messing with the
    #result. We also set GIT_OPTIONAL_LOCKS=0 instead of using
    #--no-optional-locks, since the functionality is not available in all git
    #versions we want to support:
    cmd = _safe_git_cmd + ['diff','--name-status','--no-renames','-z']
    if staged_only:
        cmd += ['--cached']
    if status_filter:
        cmd += ['--diff-filter=%s'%status_filter]
    if extra_args:
        cmd += ['--']
        cmd += extra_args
    b=subprocess.run(cmd,check=True, stdout=subprocess.PIPE).stdout
    parts=b.split(b'\x00')
    while parts:
        statusflag=parts.pop(0).decode('ascii')
        if not parts:
            if not statusflag:
                break#guard against trailing empty entry
            raise RuntimeError('Unexpected output of git diff command (%s)'%(cmd))
        fpath = _strbytes2path(parts.pop(0))
        yield statusflag,fpath

def isGit():
    #Did we already migrate to Git and are we in a Git development environment?
    dd=os.environ.get('DGCODE_DIR',None)
    return dd and pathlib.Path(dd).joinpath('.git').is_dir()
