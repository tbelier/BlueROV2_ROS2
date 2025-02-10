from setuptools import find_packages
from setuptools import setup

setup(
    name='message2',
    version='0.0.0',
    packages=find_packages(
        include=('message2', 'message2.*')),
)
