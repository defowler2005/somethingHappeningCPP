echo "Started building the somethingHappening binary...";

g++ -std=c++20 ./main.cpp -o somethingHappening -lssl -lcrypto

if [ $? -eq 0 ]; then echo "Nice the binary was successfully built!";
else echo "\nBinary failed to build binary";
fi
./somethingHappening