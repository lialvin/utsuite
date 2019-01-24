#show me 

cmake --version  
cd  utcenter  
cmake   .   
need setup boost-log  
apt-get install libboost-log-dev  

need call  getaddrinfo() display  
LookupIntern  
sql need setup  
apt-get install  libmysqlcppconn-dev  

secpk1 support recovery  
./autogen.sh  
./configure --enable-debug --disable-shared --with-pic --with-bignum=no --enable-module-recovery   
make & make install  


boost test single case test  
r --run_test=claimtrie_tests  

this tool need openssl 1.0 version
tar -xvzf openssl-1.0.2q.tar.gz 
cd openssl-1.0.2q/
./config 
make 
make install

ln -s /usr/local/ssl/include/openssl /usr/include/openssl
ln -s /usr/local/ssl/bin/openssl /usr/bin/openssl

http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl-dev_1.0.2g-1ubuntu4.14_amd64.deb
dpkg -i libssl-dev_1.0.2g-1ubuntu4.14_amd64.deb
