HttpIntervalStorage
===================

Http server for storage intervals. Operations: Add, Remove, Search


ADD:
    curl -X ADD -d "[a,b]" localhost:8080

REMOVE:
    curl -X REMOVE -d "[a,b]" localhost:8080

SEARCH:
    curl -X SEARCH -d "[a,b]" localhost:8080

where a,b integer.
