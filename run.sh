clear
cd "$(dirname "$0")"
mkdir -p build output output/angle
output=$(g++ ./src/*.cpp -o build/projectile.out 2>&1)
if [[ $? != 0 ]]; then
    echo -e "Error:\n$output"
else
    ./build/projectile.out
    python ./src/plot.py
fi