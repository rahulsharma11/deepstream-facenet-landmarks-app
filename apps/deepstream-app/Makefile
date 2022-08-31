################################################################################
# Copyright (c) 2019-2020, NVIDIA CORPORATION. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
################################################################################

CUDA_VER?=11.6
ifeq ($(CUDA_VER),)
  $(error "CUDA_VER is not set")
endif
CC=g++
APP:= deepstream-app
CFLAGS+=-fpermissive -std=c++14 -ggdb -g $(LOG_LEVEL) $(BUILD_CONFIG) -Wnarrowing 
#CFLAGS+= -D_GLIBCXX_USE_CXX11_ABI=1 -Wno-sign-compare -Wno-deprecated-declarations

TARGET_DEVICE = $(shell gcc -dumpmachine | cut -f1 -d -)

NVDS_VERSION:=6.1

LIB_INSTALL_DIR?=/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)/lib/
APP_INSTALL_DIR?=/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)/bin/

ifeq ($(TARGET_DEVICE),aarch64)
  CFLAGS:= -DPLATFORM_TEGRA
endif

SRCS:= $(wildcard *.cpp)
SRCS+= $(wildcard ../apps-common/src/*.cpp ../apps-common/src/deepstream-yaml/*.cpp)

INCS:= $(wildcard *.h)

PKGS:= gstreamer-1.0 gstreamer-video-1.0 x11 json-glib-1.0 opencv4

OBJS:= $(SRCS:.cpp=.o)

CFLAGS+= -I./ -I../apps-common/includes  -I/usr/include/gstreamer-1.0/ \
		 -I../../includes -DDS_VERSION_MINOR=1 -DDS_VERSION_MAJOR=5 \
		 -I ../../includes/cvcore_headers/ \
		 -I /usr/local/cuda-$(CUDA_VER)/include -I/usr/include

LIBS:= -L/usr/local/cuda-$(CUDA_VER)/lib64/ -lcudart -L/usr/lib -lm -lyaml-cpp

LIBS+= -L$(LIB_INSTALL_DIR) -lnvdsgst_meta -lnvds_meta -L/opt/nvidia/deepstream/deepstream-6.1/lib/cvcore_libs -lnvcv_faciallandmarks -lnvdsgst_helper -L/opt/TensorRT-8.2.0/lib -L../../launchConfig/nvdsinfer_custom_impl_Centerface/ -lnvdsinfer_custom_impl_Centerface \
	   -lnvdsgst_smartrecord -lnvds_utils -lnvds_msgbroker -lm \
       -lcuda -lgstrtspserver-1.0  -lopencv_core -lopencv_highgui -lopencv_imgproc -ldl -Wl,-rpath,$(LIB_INSTALL_DIR)

CFLAGS+= $(shell pkg-config --cflags $(PKGS))

LIBS+= $(shell pkg-config --libs $(PKGS))

all: $(APP)

%.o: %.cpp $(INCS) Makefile
	$(CC) -c -o $@ $(CFLAGS) $<

$(APP): $(OBJS) Makefile
	$(CC) -o $(APP) $(OBJS) $(LIBS)

install: $(APP)
	cp -rv $(APP) $(APP_INSTALL_DIR)

clean:
	rm -rf $(OBJS) $(APP)
