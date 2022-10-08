# Client server application: Currency Exchange 

Libraries were used to create it boost::asio и nlohmann::json.

Exchange with client-server architecture. The exchange will only trade dollars (USD) for rubles (RUB).
The server must accept requests to buy or sell dollars for rubles from different clients at a certain price.


Build guide:

git clone git@github.com:Add-JDHero/client-server-market.git  
cd client-server-market  
mkdir build && cd ./build  
cmake ..  
cmake —build .  

After building: write ./Server or ./Client



