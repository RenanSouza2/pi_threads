set -e
rm -rf thread_log/*
ulimit -s unlimited
make build
time ./src/main.o $@ PI
