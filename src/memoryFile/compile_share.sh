g++ -shared -fPIC leak_finder.cpp -o leakfinder.so -ldl
echo "Compile Success"
export LD_PRELOAD=./leakfinder.so
echo "Preload Success"

# To disable:
# export LC_PRELOAD=
# unset LC_PRELOAD