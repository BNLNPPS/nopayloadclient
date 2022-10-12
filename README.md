```
cmake -S . -B build
cmake --build build/
cd build
ctest --verbose

cmake -S . -B build && cmake --build build/ && cd build/ && ctest --verbose && cd ..

curl -X PUT http://localhost:8000/api/cdb_rest/gt_change_status/linos_object/locked --libcurl code.c
```