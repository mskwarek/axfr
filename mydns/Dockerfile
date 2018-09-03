FROM ubuntu:latest

# instead of calling g++ directly you can also use some build toolkit like make
# install the necessary build tools when needed
#before_script: 
RUN apt update && apt -y install make autoconf cmake lcov build-essential git gcc python lshw llvm-5.0 llvm-5.0-tools clang-5.0
RUN git clone -q https://github.com/google/googletest.git googletest
RUN cd googletest && git checkout tags/release-1.8.0 && mkdir -p googletest/build && cd googletest/build && cmake .. && make && make install
RUN cd googletest/googlemock && mkdir build && cd build && cmake .. && make && make install && cd ../../..
  
RUN mkdir build
#RUN cd build && cmake .. -DCOVERAGE=1 && make -j
#  artifacts:
#    paths:
#      - mydns/build

#test:
#  stage: test
#  before_script:
# apt update && apt -y install gcc lcov
#  dependencies:
#    - build
#  script:
#RUN cd mydns/build
#RUN ./DnsQuery/test/runUnitTests
#    - mkdir _coverage && cd _coverage
#    - find .. -iname "*.gcda" | xargs gcov
#    - lcov --capture --directory .. --output-file coverage.info
#    - lcov --remove coverage.info '/usr/include/*' '/usr/lib/*' '/usr/local/*' -o coverage_filtered.info
#    - genhtml coverage_filtered.info --output-directory ../../../coverage
#    - cd ../../..

