
****
slog
****

.. contents::
   :local:


Introduction
============

This library is a template for what *I* consider a clean and modern C++ project.


Prerequisites
=============

To setup your build environment,
get the external dependencies in place,
use the `conda <https://conda.io>`_ package manager::

  conda env create -f environment.yml

.. tip:: To use an alternative name for the environment,
         use::

           conda env create -n slog-development -f environment.yml

         And to use an alternative path for the environment,
         use::

           conda env create -p conda.env -f environment.yml

         If the package is not found,
         you may need to add the `Sarcasm channel <https://anaconda.org/Sarcasm/repo>`_ first::

            conda config --append channels sarcasm

.. note:: If the package is not found,
          you may need to add the `Sarcasm channel <https://anaconda.org/Sarcasm/repo>`_ first::

            conda config --append channels sarcasm

You now need to source the environment::

  source activate slog-env


Build
=====

To build, type::

  mkdir build
  cd build
  cmake -DCMAKE_PREFIX_PATH=$CONDA_PREFIX ..
  cmake --build . --config Release
  ctest -C Release --output-on-failure
