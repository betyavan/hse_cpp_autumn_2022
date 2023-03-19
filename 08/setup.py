from distutils.core import setup, Extension

setup(name='cjson', version='1.0', ext_modules=[Extension('cjson', ['cjson.c'])])