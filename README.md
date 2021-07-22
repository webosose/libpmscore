Summary
-------
pmscore is library component of power2, which  handles device power states

Description
-----------
this is component framework for PMS. this will load, validate
& handle power states of the device

How to Build on Linux
---------------------

## Dependencies

Below are the tools and libraries (and their minimum versions) required to build sample program:

* cmake (version required by cmake-modules-webos)
* gcc
* make
* cmake-modules-webos
* luna-service2
* glib-2.0
* pmloglib
* libpbnjson
* nyx-lib

## Building

    $ cd build-webos
    $ source oe-init-build-env
    $ bitbake libpmscore

Copyright and License Information
=================================
Unless otherwise specified, all content, including all source code files and
documentation files in this repository are:

Copyright (c) 2021 LG Electronics, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

SPDX-License-Identifier: Apache-2.0
