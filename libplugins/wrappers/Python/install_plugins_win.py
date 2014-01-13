""" 
Usage:
   python install_rrplugins_module.py
   This file will install files needed to 
   use roadrunner plugins from Python
"""

import site
import shutil
import os.path
import os

rrPackage = 'rrplugins'
def rmrrplugins(path):
    print('deleting ' + rrPackage +' from ' + path)

    files = ['rrPlugins', 'rrPlugins_CAPI', 'rrPluginUtils', 'rrFindSharedLib']
    try:
        for file in files:
            fName = file + '.py'
            os.remove(os.path.join(path, fName))
    except:
		pass
	
    try:
        for file in files:
            fName = file + '.pyd'
            os.remove(os.path.join(path, fName))
    except:
		pass
		
    shutil.rmtree(os.path.join(path, rrPackage), ignore_errors=True)

splist = site.getsitepackages()
spFolder = ''
	
for s in splist:
    if s.find('site-packages') > 0:
        spFolder = s
    	rmrrplugins(spFolder)
		
print('installing ' + rrPackage +' to site packages dir: ' + spFolder)

destination = os.path.join(spFolder, 'rrplugins')

#Figure out where we are
cwd = os.path.dirname(os.path.abspath(__file__))

src = os.path.abspath(os.path.join(cwd, 'site-packages', 'rrplugins'))

print('src directory is :' + src)
print("destination directory is: " + destination)

#Copy the content of the module
shutil.copytree(src, destination)

