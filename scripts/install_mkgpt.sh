sudo apt install autoconf automake
git clone https://github.com/jncronin/mkgpt.git
cd mkgpt
automake --add-missing
autoreconf
./configure
make
sudo make install
