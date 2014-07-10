HttpIntervalStorage
===================



Multithreaded HTTP server, that supports commands to add, delete of intervals and search of interval containing the desired interval. 

Searching interval with complexity O(Log(N)). 
Server uses of data structure based on std::map for storage of intervals.
Boost::asio has been used for network comunications.


Using:

ADD:
    curl -X ADD -d "[a,b]" localhost:8080

REMOVE:
    curl -X REMOVE -d "[a,b]" localhost:8080

SEARCH:
    curl -X SEARCH -d "[a,b]" localhost:8080

where a,b integer.
