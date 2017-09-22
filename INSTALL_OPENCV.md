# Dependencies

```
	sudo apt-get install build-essential checkinstall cmake pkg-config libjpeg8-dev libpng-dev libtiff5-dev libavcodec-dev libavformat-dev \
	libswscale-dev libdc1394-22-dev libxine2-dev libv4l-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libtbb-dev libgtk-3-dev  \
 	libatlas-base-dev gfortran

```

# Clone opencv from git

```
	git clone https://github.com/opencv/opencv.git
	
	cd opencv

	git checkout 3.3.0

```

# Build and install

```
	mkdir build

	cd build

	cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_EXAMPLES=ON ..

	make -j4

	sudo make install

	sudo ldconfig

```

# Test 

```
	./opencv_test_core

```