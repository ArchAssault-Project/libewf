#!/usr/bin/env python
import glob
import platform
import os
import shutil
import sys

from distutils import sysconfig
from distutils.command.bdist import bdist
from distutils.command.sdist import sdist
from distutils.core import setup, Extension


# Custom handler for the setup.py bdist_rpm command.
class custom_bdist_rpm(bdist):
	def run(self):
		print "'setup.py bdist_rpm' command not supported use 'rpmbuild' instead."
		sys.exit(1)


# Custom handler for the setup.py sdist command.
class custom_sdist(sdist):
	def run(self):
		print "'setup.py sdist' command not supported use 'make dist' instead."
		sys.exit(1)


LIBRARY_NAME = "libewf"
LIBRARY_VERSION = "20140608"
MODULE_NAME = "py%s" % (LIBRARY_NAME[3:])
# bdist_msi does not support the library version, neither a date as a version.
MODULE_VERSION = "20140608.1"
PROJECT_URL = "http://code.google.com/p/%s/" % (LIBRARY_NAME)

DIRECTORY_NAMES = [
	"common",
	"include",
	"libcstring",
	"libcerror",
	"libcdata",
	"libclocale",
	"libcsplit",
	"libuna",
	"libcfile",
	"libcpath",
	"libbfio",
]

INCLUDE_DIRS = []

for directory_name in DIRECTORY_NAMES:
	INCLUDE_DIRS.append(os.path.join("..", directory_name))

LIBRARY_NAMES = [
	"libcstring",
	"libcerror",
	"libcdata",
	"libclocale",
	"libcsplit",
	"libuna",
	"libcfile",
	"libcpath",
	"libbfio",
	"%s" % (LIBRARY_NAME),
]

LIBRARIES = []
LIBRARY_DIRS = []
LIBRARY_DATA_FILES = []

PYTHON_LIBRARY_DIRECTORY = sysconfig.get_python_lib(True)
_, _, PYTHON_LIBRARY_DIRECTORY = PYTHON_LIBRARY_DIRECTORY.rpartition(sysconfig.PREFIX)

if PYTHON_LIBRARY_DIRECTORY.startswith(os.sep):
	PYTHON_LIBRARY_DIRECTORY = PYTHON_LIBRARY_DIRECTORY[1:]

if platform.system() == "Windows":
	DEFINE_MACROS = [
		("HAVE_LOCAL_LIBCSTRING", ""),
		("HAVE_LOCAL_LIBCERROR", ""),
		("HAVE_LOCAL_LIBCDATA", ""),
		("HAVE_LOCAL_LIBCLOCALE", ""),
		("HAVE_LOCAL_LIBCSPLIT", ""),
		("HAVE_LOCAL_LIBUNA", ""),
		("HAVE_LOCAL_LIBCFILE", ""),
		("HAVE_LOCAL_LIBCPATH", ""),
		("HAVE_LOCAL_LIBBFIO", ""),
		("%s_DLL_IMPORT" % (LIBRARY_NAME.upper()), ""),
	]
	for library_name in LIBRARY_NAMES:
		LIBRARIES.append(library_name)

	library_directory = os.path.join("..", "msvscpp", "Release")

	LIBRARY_DIRS.append(library_directory)

	LIBRARY_DLL_FILES = [
		"%s.dll" % (LIBRARY_NAME),
		"zlib.dll",
	]
	for library_dll_filename in LIBRARY_DLL_FILES:
		library_dll_file = os.path.join(library_directory, library_dll_filename)

		if not os.path.exists(library_dll_file):
			print "No such file: %s" % (library_dll_file)
			sys.exit(1)

		LIBRARY_DATA_FILES.append(library_dll_file)

else:
	DEFINE_MACROS = [
		("HAVE_CONFIG_H", ""),
	]
	for library_name in LIBRARY_NAMES:
		library_directory = os.path.join("..", library_name)

		LIBRARY_DIRS.append(library_directory)

# Add the LICENSE file to the distribution.
copying_file = os.path.join("..", "COPYING")
license_file = "LICENSE.%s" % (LIBRARY_NAME)

shutil.copyfile(copying_file, license_file)

LIBRARY_DATA_FILES.append(license_file)

# TODO: what about description and platform in egg file

setup(
	name = MODULE_NAME,
	url = PROJECT_URL,
	version = MODULE_VERSION,
	description = "Python bindings module for %s" % (MODULE_NAME),
	author = "Joachim Metz",
	author_email = "joachim.metz@gmail.com",
	cmdclass = {
		"bdist_rpm": custom_bdist_rpm,
		"sdist": custom_sdist,
	},
	ext_modules = [
		Extension(
			MODULE_NAME,
			glob.glob("*.c"),
			define_macros = DEFINE_MACROS,
			include_dirs = INCLUDE_DIRS,
			libraries = LIBRARIES,
			library_dirs = LIBRARY_DIRS,
		),
	],
	data_files = [(PYTHON_LIBRARY_DIRECTORY, LIBRARY_DATA_FILES)],
)

