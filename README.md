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
